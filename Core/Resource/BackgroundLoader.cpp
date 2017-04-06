
#include "../../Sapphire/Predefined.h"

#ifdef SAPPHIRE_THREADING



#include "../Core/Context.h"
#include "../Core/Profiler.h"
#include "../IO/Log.h"
#include "../Resource/BackgroundLoader.h"
#include "../Resource/ResourceCache.h"
#include "../Resource/ResourceEvents.h"

#include "../DebugNew.h"

namespace Sapphire
{

	BackgroundLoader::BackgroundLoader(ResourceCache* owner) :
		owner_(owner)
	{
	}

	void BackgroundLoader::ThreadFunction()
	{
		//循环加载
		while (shouldRun_)
		{
			backgroundLoadMutex_.Acquire();

			// 查询没有被加载的队列资源项
			HashMap<Pair<StringHash, StringHash>, BackgroundLoadItem>::Iterator i = backgroundLoadQueue_.Begin();
			while (i != backgroundLoadQueue_.End())
			{
				if (i->second_.resource_->GetAsyncLoadState() == ASYNC_QUEUED)
					break;
				else
					++i;
			}
			//没找到
			if (i == backgroundLoadQueue_.End())
			{
				//没有资源需要被加载
				backgroundLoadMutex_.Release();
				Time::Sleep(5);
			}
			else
			{   //取得该后台加载项目
				BackgroundLoadItem& item = i->second_;
				Resource* resource = item.resource_;
				// 需确保项目未从队列中移除，状态是queue或load
				backgroundLoadMutex_.Release();

				bool success = false;
				//从该加载器所属的资源缓存中获取文件
				SharedPtr<File> file = owner_->GetFile(resource->GetName(), item.sendEventOnFailure_);
				if (file)
				{
					//加载状态
					resource->SetAsyncLoadState(ASYNC_LOADING);
					//开始加载文件
					success = resource->BeginLoad(*file);
				}

				// 现在处理依赖关系
				// 当操作别的入口需要对队列加锁
				Pair<StringHash, StringHash> key = MakePair(resource->GetType(), resource->GetNameHash());
				backgroundLoadMutex_.Acquire();
				if (item.dependents_.Size())
				{
					//遍历该资源依赖的所有资源
					for (HashSet<Pair<StringHash, StringHash> >::Iterator i = item.dependents_.Begin();
						i != item.dependents_.End(); ++i)
					{
						//在后台加载队列中寻找依赖项
						HashMap<Pair<StringHash, StringHash>, BackgroundLoadItem>::Iterator j = backgroundLoadQueue_.Find(*i);
						if (j != backgroundLoadQueue_.End())   //该项依赖资源在后台已有加载项
							j->second_.dependencies_.Erase(key);    //移除从属资源
					}

					item.dependents_.Clear();
				}
				//设置加载状态
				resource->SetAsyncLoadState(success ? ASYNC_SUCCESS : ASYNC_FAIL);
				backgroundLoadMutex_.Release();
			}
		}
	}

	//加资源加入加载队列
	bool BackgroundLoader::QueueResource(StringHash type, const String& name, bool sendEventOnFailure, Resource* caller)
	{
		StringHash nameHash(name);
		Pair<StringHash, StringHash> key = MakePair(type, nameHash);

		MutexLock lock(backgroundLoadMutex_);

		// 是否不存在
		if (backgroundLoadQueue_.Find(key) != backgroundLoadQueue_.End())
			return false;

		//取出该后台下载项
		BackgroundLoadItem& item = backgroundLoadQueue_[key];
		//设置否发送失败事件
		item.sendEventOnFailure_ = sendEventOnFailure;

		// 确定指针非空并且是资源的子类
		item.resource_ = DynamicCast<Resource>(owner_->GetContext()->CreateObject(type));
		if (!item.resource_)
		{
			SAPPHIRE_LOGERROR("Could not load unknown resource type " + String(type));

			if (sendEventOnFailure && Thread::IsMainThread())
			{
				using namespace UnknownResourceType;

				VariantMap& eventData = owner_->GetEventDataMap();
				eventData[P_RESOURCETYPE] = type;
				owner_->SendEvent(E_UNKNOWNRESOURCETYPE, eventData);
			}

			backgroundLoadQueue_.Erase(key);
			return false;
		}

		SAPPHIRE_LOGDEBUG("Background loading resource " + name);

		item.resource_->SetName(name);
		item.resource_->SetAsyncLoadState(ASYNC_QUEUED);

		// If this is a resource calling for the background load of more resources, mark the dependency as necessary
		if (caller)
		{
			Pair<StringHash, StringHash> callerKey = MakePair(caller->GetType(), caller->GetNameHash());
			HashMap<Pair<StringHash, StringHash>, BackgroundLoadItem>::Iterator j = backgroundLoadQueue_.Find(callerKey);
			if (j != backgroundLoadQueue_.End())
			{
				BackgroundLoadItem& callerItem = j->second_;
				item.dependents_.Insert(callerKey);
				callerItem.dependencies_.Insert(key);
			}
			else
				SAPPHIRE_LOGWARNING("Resource " + caller->GetName() +
				" requested for a background loaded resource but was not in the background load queue");
		}

		// Start the background loader thread now
		if (!IsStarted())
			Run();

		return true;
	}

	void BackgroundLoader::WaitForResource(StringHash type, StringHash nameHash)
	{
		backgroundLoadMutex_.Acquire();

		// Check if the resource in question is being background loaded
		Pair<StringHash, StringHash> key = MakePair(type, nameHash);
		HashMap<Pair<StringHash, StringHash>, BackgroundLoadItem>::Iterator i = backgroundLoadQueue_.Find(key);
		if (i != backgroundLoadQueue_.End())
		{
			backgroundLoadMutex_.Release();

			{
				Resource* resource = i->second_.resource_;
				HiresTimer waitTimer;
				bool didWait = false;

				for (;;)
				{
					unsigned numDeps = i->second_.dependencies_.Size();
					AsyncLoadState state = resource->GetAsyncLoadState();
					if (numDeps > 0 || state == ASYNC_QUEUED || state == ASYNC_LOADING)
					{
						didWait = true;
						Time::Sleep(1);
					}
					else
						break;
				}

				if (didWait)
					SAPPHIRE_LOGDEBUG("Waited " + String(waitTimer.GetUSec(false) / 1000) + " ms for background loaded resource " +
					resource->GetName());
			}

			// This may take a long time and may potentially wait on other resources, so it is important we do not hold the mutex during this
			FinishBackgroundLoading(i->second_);

			backgroundLoadMutex_.Acquire();
			backgroundLoadQueue_.Erase(i);
			backgroundLoadMutex_.Release();
		}
		else
			backgroundLoadMutex_.Release();
	}

	void BackgroundLoader::FinishResources(int maxMs)
	{
		if (IsStarted())
		{
			HiresTimer timer;

			backgroundLoadMutex_.Acquire();

			for (HashMap<Pair<StringHash, StringHash>, BackgroundLoadItem>::Iterator i = backgroundLoadQueue_.Begin();
				i != backgroundLoadQueue_.End();)
			{
				Resource* resource = i->second_.resource_;
				unsigned numDeps = i->second_.dependencies_.Size();
				AsyncLoadState state = resource->GetAsyncLoadState();
				if (numDeps > 0 || state == ASYNC_QUEUED || state == ASYNC_LOADING)
					++i;
				else
				{
					// Finishing a resource may need it to wait for other resources to load, in which case we can not
					// hold on to the mutex
					backgroundLoadMutex_.Release();
					FinishBackgroundLoading(i->second_);
					backgroundLoadMutex_.Acquire();
					i = backgroundLoadQueue_.Erase(i);
				}

				// Break when the time limit passed so that we keep sufficient FPS
				if (timer.GetUSec(false) >= maxMs * 1000)
					break;
			}

			backgroundLoadMutex_.Release();
		}
	}

	unsigned BackgroundLoader::GetNumQueuedResources() const
	{
		MutexLock lock(backgroundLoadMutex_);
		return backgroundLoadQueue_.Size();
	}

	void BackgroundLoader::FinishBackgroundLoading(BackgroundLoadItem& item)
	{
		Resource* resource = item.resource_;

		bool success = resource->GetAsyncLoadState() == ASYNC_SUCCESS;
		// If BeginLoad() phase was successful, call EndLoad() and get the final success/failure result
		if (success)
		{
#ifdef SAPPHIRE_PROFILING
			String profileBlockName("Finish" + resource->GetTypeName());

			Profiler* profiler = owner_->GetSubsystem<Profiler>();
			if (profiler)
				profiler->BeginBlock(profileBlockName.CString());
#endif
			SAPPHIRE_LOGDEBUG("Finishing background loaded resource " + resource->GetName());
			success = resource->EndLoad();

#ifdef SAPPHIRE_PROFILING
			if (profiler)
				profiler->EndBlock();
#endif
		}
		resource->SetAsyncLoadState(ASYNC_DONE);

		if (!success && item.sendEventOnFailure_)
		{
			using namespace LoadFailed;

			VariantMap& eventData = owner_->GetEventDataMap();
			eventData[P_RESOURCENAME] = resource->GetName();
			owner_->SendEvent(E_LOADFAILED, eventData);
		}

		// Send event, either success or failure
	{
		using namespace ResourceBackgroundLoaded;

		VariantMap& eventData = owner_->GetEventDataMap();
		eventData[P_RESOURCENAME] = resource->GetName();
		eventData[P_SUCCESS] = success;
		eventData[P_RESOURCE] = resource;
		owner_->SendEvent(E_RESOURCEBACKGROUNDLOADED, eventData);
	}

	// Store to the cache; use same mechanism as for manual resources
	if (success || owner_->GetReturnFailedResources())
		owner_->AddManualResource(resource);
	}

}

#endif
#include "../../Sapphire/Predefined.h"

#include "../Core/Context.h"
#include "../Core/CoreEvents.h"
#include "../Core/Profiler.h"
#include "../Core/WorkQueue.h"
#include "../IO/FileSystem.h"
#include "../IO/FileWatcher.h"
#include "../IO/Log.h"
#include "../IO/PackageFile.h"
#include "../Resource/BackgroundLoader.h"
#include "../Resource/Image.h"
#include "../Resource/JSONFile.h"
#include "../Resource/PListFile.h"
#include "../Resource/ResourceCache.h"
#include "../Resource/ResourceEvents.h"
#include "../Resource/XMLFile.h"

#include "../DebugNew.h"

#include <cstdio>


namespace  Sapphire
{
	//资源目录名
	static const char* checkDirs[] =
	{
		"Fonts",
		"Materials",
		"Models",
		"Music",
		"Objects",
		"Particle",
		"PostProcess",
		"RenderPaths",
		"Scenes",
		"Scripts",
		"Sounds",
		"Shaders",
		"Techniques",
		"Textures",
		"UI",
		0
	};

	static const SharedPtr<Resource> noResource;

	ResourceCache::ResourceCache(Context* context) :
		Object(context),
		autoReloadResources_(false),
		returnFailedResources_(false),
		searchPackagesFirst_(true),
		isRouting_(false),
		finishBackgroundResourcesMs_(5)
	{
		// 注册资源库对象工厂
		RegisterResourceLibrary(context_);

#ifdef SAPPHIRE_THREADING
		//创建后台加载器，它的线程将会在第一次后台请求时启动
		backgroundLoader_ = new BackgroundLoader(this);
#endif

		// 订阅帧开始事件处理目录观察者和后台资源加载
		SubscribeToEvent(E_BEGINFRAME, SAPPHIRE_HANDLER(ResourceCache, HandleBeginFrame));
	}

	ResourceCache::~ResourceCache()
	{
#ifdef SAPPHIRE_THREADING
		backgroundLoader_.Reset();
#endif
	}

	bool ResourceCache::AddResourceDir(const String& pathName, unsigned priority)
	{
		MutexLock lock(resourceMutex_);

		FileSystem* fileSystem = GetSubsystem<FileSystem>();
		if (!fileSystem || !fileSystem->DirExists(pathName))
		{
			SAPPHIRE_LOGERROR("Could not open directory " + pathName);
			return false;
		}

		// 转为绝对路径
		String fixedPath = SanitateResourceDirName(pathName);

		// 检查是否有同样的目录
		for (unsigned i = 0; i < resourceDirs_.Size(); ++i)
		{
			if (!resourceDirs_[i].Compare(fixedPath, false))
				return true;
		}
		//按优先级派
		if (priority < resourceDirs_.Size())
			resourceDirs_.Insert(priority, fixedPath);
		else
			resourceDirs_.Push(fixedPath); //放最后

		// 如果开启自动重载， 为这个目录创建一个文件观察者
		if (autoReloadResources_)
		{
			//创建文件观察者
			SharedPtr<FileWatcher> watcher(new FileWatcher(context_));
			watcher->StartWatching(fixedPath, true);
			fileWatchers_.Push(watcher);
		}

		SAPPHIRE_LOGINFO("Added resource path " + fixedPath);
		return true;
	}

	bool ResourceCache::AddPackageFile(PackageFile* package, unsigned priority)
	{
		MutexLock lock(resourceMutex_);

		if (!package || !package->GetNumFiles())
		{
			SAPPHIRE_LOGERRORF("Could not add package file %s due to load failure", package->GetName().CString());
			return false;
		}

		if (priority < packages_.Size())
			packages_.Insert(priority, SharedPtr<PackageFile>(package));
		else
			packages_.Push(SharedPtr<PackageFile>(package));

		SAPPHIRE_LOGINFO("Added resource package " + package->GetName());
		return true;
	}

	bool ResourceCache::AddPackageFile(const String& fileName, unsigned priority)
	{
		SharedPtr<PackageFile> package(new PackageFile(context_));
		return package->Open(fileName) && AddPackageFile(package);
	}

	bool ResourceCache::AddManualResource(Resource* resource)
	{
		if (!resource)
		{
			SAPPHIRE_LOGERROR("Null manual resource");
			return false;
		}

		const String& name = resource->GetName();
		if (name.Empty())
		{
			SAPPHIRE_LOGERROR("Manual resource with empty name, can not add");
			return false;
		}

		resource->ResetUseTimer();  //重载使用计时器 
		resourceGroups_[resource->GetType()].resources_[resource->GetNameHash()] = resource;
		UpdateResourceGroup(resource->GetType());
		return true;
	}

	void ResourceCache::RemoveResourceDir(const String& pathName)
	{
		MutexLock lock(resourceMutex_);

		String fixedPath = SanitateResourceDirName(pathName);  //获得绝对路径

		for (unsigned i = 0; i < resourceDirs_.Size(); ++i)
		{
			if (!resourceDirs_[i].Compare(fixedPath, false))
			{
				resourceDirs_.Erase(i);
				// 移除对应目录的filewatcher
				for (unsigned j = 0; j < fileWatchers_.Size(); ++j)
				{
					if (!fileWatchers_[j]->GetPath().Compare(fixedPath, false))
					{
						fileWatchers_.Erase(j);
						break;
					}
				}
				SAPPHIRE_LOGINFO("Removed resource path " + fixedPath);
				return;
			}
		}
	}

	void ResourceCache::RemovePackageFile(PackageFile* package, bool releaseResources, bool forceRelease)
	{
		MutexLock lock(resourceMutex_);

		for (Vector<SharedPtr<PackageFile> >::Iterator i = packages_.Begin(); i != packages_.End(); ++i)
		{
			if (*i == package)
			{
				if (releaseResources)
					ReleasePackageResources(*i, forceRelease);
				SAPPHIRE_LOGINFO("Removed resource package " + (*i)->GetName());
				packages_.Erase(i);
				return;
			}
		}
	}

	void ResourceCache::RemovePackageFile(const String& fileName, bool releaseResources, bool forceRelease)
	{
		MutexLock lock(resourceMutex_);

		// 比较文件名和扩展名
		String fileNameNoPath = GetFileNameAndExtension(fileName);

		for (Vector<SharedPtr<PackageFile> >::Iterator i = packages_.Begin(); i != packages_.End(); ++i)
		{
			if (!GetFileNameAndExtension((*i)->GetName()).Compare(fileNameNoPath, false))
			{
				if (releaseResources)
					ReleasePackageResources(*i, forceRelease);
				SAPPHIRE_LOGINFO("Removed resource package " + (*i)->GetName());
				packages_.Erase(i);
				return;
			}
		}
	}

	void ResourceCache::ReleaseResource(StringHash type, const String& name, bool force)
	{
		StringHash nameHash(name);
		const SharedPtr<Resource>& existingRes = FindResource(type, nameHash);
		if (!existingRes)
			return;

		// 如果另外的引用存在，不要释放，除非force == true
		if ((existingRes.Refs() == 1 && existingRes.WeakRefs() == 0) || force)
		{
			resourceGroups_[type].resources_.Erase(nameHash);
			UpdateResourceGroup(type);
		}
	}

	void ResourceCache::ReleaseResources(StringHash type, bool force)
	{
		bool released = false;

		HashMap<StringHash, ResourceGroup>::Iterator i = resourceGroups_.Find(type);
		if (i != resourceGroups_.End())
		{
			for (HashMap<StringHash, SharedPtr<Resource> >::Iterator j = i->second_.resources_.Begin();
				j != i->second_.resources_.End();)
			{
				HashMap<StringHash, SharedPtr<Resource> >::Iterator current = j++;
				// 如果另外的引用存在，不要释放，除非force == true
				if ((current->second_.Refs() == 1 && current->second_.WeakRefs() == 0) || force)
				{
					i->second_.resources_.Erase(current);
					released = true;
				}
			}
		}

		if (released)
			UpdateResourceGroup(type);
	}

	void ResourceCache::ReleaseResources(StringHash type, const String& partialName, bool force)
	{
		bool released = false;

		HashMap<StringHash, ResourceGroup>::Iterator i = resourceGroups_.Find(type);
		if (i != resourceGroups_.End())
		{
			for (HashMap<StringHash, SharedPtr<Resource> >::Iterator j = i->second_.resources_.Begin();
				j != i->second_.resources_.End();)
			{
				HashMap<StringHash, SharedPtr<Resource> >::Iterator current = j++;
				if (current->second_->GetName().Contains(partialName))
				{
					// 如果另外的引用存在，不要释放，除非force == true
					if ((current->second_.Refs() == 1 && current->second_.WeakRefs() == 0) || force)
					{
						i->second_.resources_.Erase(current);
						released = true;
					}
				}
			}
		}

		if (released)
			UpdateResourceGroup(type);
	}

	void ResourceCache::ReleaseResources(const String& partialName, bool force)
	{
		// 有些资源会引用其他的，想材质与纹理。要确保两种都释放
	    // 除非force==true
		unsigned repeat = force ? 1 : 2;

		while (repeat--)
		{
			for (HashMap<StringHash, ResourceGroup>::Iterator i = resourceGroups_.Begin(); i != resourceGroups_.End(); ++i)
			{
				bool released = false;

				for (HashMap<StringHash, SharedPtr<Resource> >::Iterator j = i->second_.resources_.Begin();
					j != i->second_.resources_.End();)
				{
					HashMap<StringHash, SharedPtr<Resource> >::Iterator current = j++;
					if (current->second_->GetName().Contains(partialName))
					{
						// 如果有别的引用存在，释放，除非强制
						if ((current->second_.Refs() == 1 && current->second_.WeakRefs() == 0) || force)
						{
							i->second_.resources_.Erase(current);
							released = true;
						}
					}
				}
				if (released)
					UpdateResourceGroup(i->first_);
			}
		}
	}

	void ResourceCache::ReleaseAllResources(bool force)
	{
		unsigned repeat = force ? 1 : 2;

		while (repeat--)
		{
			for (HashMap<StringHash, ResourceGroup>::Iterator i = resourceGroups_.Begin();
				i != resourceGroups_.End(); ++i)
			{
				bool released = false;

				for (HashMap<StringHash, SharedPtr<Resource> >::Iterator j = i->second_.resources_.Begin();
					j != i->second_.resources_.End();)
				{
					HashMap<StringHash, SharedPtr<Resource> >::Iterator current = j++;
					// 如果有别的引用存在，释放，除非强制
					if ((current->second_.Refs() == 1 && current->second_.WeakRefs() == 0) || force)
					{
						i->second_.resources_.Erase(current);
						released = true;
					}
				}
				if (released)
					UpdateResourceGroup(i->first_);
			}
		}
	}

	//重载资源
	bool ResourceCache::ReloadResource(Resource* resource)
	{
		if (!resource)
			return false;

		//发送资源重载事件
		resource->SendEvent(E_RELOADSTARTED);

		bool success = false;
		SharedPtr<File> file = GetFile(resource->GetName());
		if (file)
			success = resource->Load(*(file.Get()));

		if (success)
		{
			resource->ResetUseTimer();   //重置使用计时器
			UpdateResourceGroup(resource->GetType());
			resource->SendEvent(E_RELOADFINISHED);    //重载完成
			return true;
		}

		// 如果重载失败，不从缓存中移除这个资源，要准备尝试再次加重
		resource->SendEvent(E_RELOADFAILED);
		return false;
	}

	void ResourceCache::ReloadResourceWithDependencies(const String& fileName)
	{
		StringHash fileNameHash(fileName);
		// 查找资源
		const SharedPtr<Resource>& resource = FindResource(fileNameHash);
		if (resource)
		{
			SAPPHIRE_LOGDEBUG("Reloading changed resource " + fileName);
			ReloadResource(resource);
		}
		// 通过加载资源的xml文件来检查该资源的依赖关系
		if (!resource || GetExtension(resource->GetName()) == ".xml")
		{
			// 如果是依赖的资源，重载它
			HashMap<StringHash, HashSet<StringHash> >::ConstIterator j = dependentResources_.Find(fileNameHash);
			if (j != dependentResources_.End())
			{
				//重载一个资源可能会修改依赖跟踪结构，因此收集这个资源前我们需要重载
				Vector<SharedPtr<Resource> > dependents;
				dependents.Reserve(j->second_.Size());

				for (HashSet<StringHash>::ConstIterator k = j->second_.Begin(); k != j->second_.End(); ++k)
				{
					const SharedPtr<Resource>& dependent = FindResource(*k);
					if (dependent)
						dependents.Push(dependent);
				}

				for (unsigned k = 0; k < dependents.Size(); ++k)
				{
					SAPPHIRE_LOGDEBUG("Reloading resource " + dependents[k]->GetName() + " depending on " + fileName);
					ReloadResource(dependents[k]);
				}
			}
		}
	}

	void ResourceCache::SetMemoryBudget(StringHash type, unsigned long long budget)
	{
		resourceGroups_[type].memoryBudget_ = budget;
	}

	//设置自动重载资源
	void ResourceCache::SetAutoReloadResources(bool enable)
	{
		if (enable != autoReloadResources_)
		{
			if (enable)
			{
				//为每个目录增加fileWatcher
				for (unsigned i = 0; i < resourceDirs_.Size(); ++i)
				{
					SharedPtr<FileWatcher> watcher(new FileWatcher(context_));
					watcher->StartWatching(resourceDirs_[i], true);
					fileWatchers_.Push(watcher);
				}
			}
			else
				fileWatchers_.Clear();

			autoReloadResources_ = enable;
		}
	}

	//添加router
	void ResourceCache::AddResourceRouter(ResourceRouter* router, bool addAsFirst)
	{
		// 检查重复的
		for (unsigned i = 0; i < resourceRouters_.Size(); ++i)
		{
			if (resourceRouters_[i] == router)
				return;
		}

		if (addAsFirst)
			resourceRouters_.Insert(0, SharedPtr<ResourceRouter>(router));
		else
			resourceRouters_.Push(SharedPtr<ResourceRouter>(router));
	}

	void ResourceCache::RemoveResourceRouter(ResourceRouter* router)
	{
		for (unsigned i = 0; i < resourceRouters_.Size(); ++i)
		{
			if (resourceRouters_[i] == router)
			{
				resourceRouters_.Erase(i);
				return;
			}
		}
	}

	//取得文件 （需要ResourceRouter）
	SharedPtr<File> ResourceCache::GetFile(const String& nameIn, bool sendEventOnFailure)
	{
		//上锁
		MutexLock lock(resourceMutex_);

		String name = SanitateResourceName(nameIn);
		//是否使用Routing
		if (!isRouting_)
		{
			isRouting_ = true;
			for (unsigned i = 0; i < resourceRouters_.Size(); ++i)
				resourceRouters_[i]->Route(name, RESOURCE_GETFILE); 
			isRouting_ = false;
		}

		if (name.Length())
		{
			File* file = 0;
			//首先搜索资源包
			if (searchPackagesFirst_)
			{
				file = SearchPackages(name);
				if (!file)
					file = SearchResourceDirs(name);
			}
			else
			{
				//再次搜索目录
				file = SearchResourceDirs(name);
				if (!file)
					file = SearchPackages(name);
			}
			//如果找到返回文件
			if (file)
				return SharedPtr<File>(file);
		}
		//否则发送错误
		if (sendEventOnFailure)
		{
			if (resourceRouters_.Size() && name.Empty() && !nameIn.Empty())
				SAPPHIRE_LOGERROR("Resource request " + nameIn + " was blocked");
			else
				SAPPHIRE_LOGERROR("Could not find resource " + name);

			if (Thread::IsMainThread())
			{
				using namespace ResourceNotFound;

				VariantMap& eventData = GetEventDataMap();
				eventData[P_RESOURCENAME] = name.Length() ? name : nameIn;
				SendEvent(E_RESOURCENOTFOUND, eventData);
			}
		}
		//返回空文件资源
		return SharedPtr<File>();
	}

	Resource* ResourceCache::GetExistingResource(StringHash type, const String& nameIn)
	{
		String name = SanitateResourceName(nameIn);

		if (!Thread::IsMainThread())
		{
			SAPPHIRE_LOGERROR("Attempted to get resource " + name + " from outside the main thread");
			return 0;
		}

		//如果名字为空,返回空指针
		if (name.Empty())
			return 0;

		StringHash nameHash(name);

		const SharedPtr<Resource>& existing = FindResource(type, nameHash);
		return existing;
	}

	Resource* ResourceCache::GetResource(StringHash type, const String& nameIn, bool sendEventOnFailure)
	{
		String name = SanitateResourceName(nameIn);

		if (!Thread::IsMainThread())
		{
			SAPPHIRE_LOGERROR("Attempted to get resource " + name + " from outside the main thread");
			return 0;
		}

		// 如果为空，返回空指针
		if (name.Empty())
			return 0;

		StringHash nameHash(name);

#ifdef SAPPHIRE_THREADING
		// 检查资源是否被后台加载，而现在立即就需要， 会卡主线程
		backgroundLoader_->WaitForResource(type, nameHash);
#endif
		//查找资源
		const SharedPtr<Resource>& existing = FindResource(type, nameHash);
		if (existing)
			return existing;

		SharedPtr<Resource> resource;
		// 确定指针非空并且是一个资源的子类
		resource = DynamicCast<Resource>(context_->CreateObject(type));  //创建指定类型的对象
		if (!resource)
		{
			//未知的资源类型
			SAPPHIRE_LOGERROR("Could not load unknown resource type " + String(type));

			if (sendEventOnFailure)
			{
				//发送错误事件
				using namespace UnknownResourceType;

				VariantMap& eventData = GetEventDataMap();
				eventData[P_RESOURCETYPE] = type;
				SendEvent(E_UNKNOWNRESOURCETYPE, eventData);
			}

			return 0;
		}

		// 尝试加载这个资源
		SharedPtr<File> file = GetFile(name, sendEventOnFailure);
		if (!file)
			return 0;   // 错误

		SAPPHIRE_LOGDEBUG("Loading resource " + name);
		resource->SetName(name);
		//加载资源
		if (!resource->Load(*(file.Get())))
		{
			// 发送错误事件
			if (sendEventOnFailure)
			{
				using namespace LoadFailed;

				VariantMap& eventData = GetEventDataMap();
				eventData[P_RESOURCENAME] = name;
				SendEvent(E_LOADFAILED, eventData);
			}

			if (!returnFailedResources_)
				return 0;
		}

		// 保存到缓存， 重置用户计时器
		resource->ResetUseTimer();
		resourceGroups_[type].resources_[nameHash] = resource;  //资源组
		UpdateResourceGroup(type);  //更新资源组

		return resource;
	}

	//后台加载资源
	bool ResourceCache::BackgroundLoadResource(StringHash type, const String& nameIn, bool sendEventOnFailure, Resource* caller)
	{
#ifdef SAPPHIRE_THREADING
		// 如果名字为空，立即失败
		String name = SanitateResourceName(nameIn);
		if (name.Empty())
			return false;

		// 首先检查是否已经加载
		StringHash nameHash(name);
		if (FindResource(type, nameHash) != noResource)
			return false;
		//队列化资源
		return backgroundLoader_->QueueResource(type, name, sendEventOnFailure, caller);
#else
		// 不支持线程，回退到同步加载
		return GetResource(type, nameIn, sendEventOnFailure);
#endif
	}
	//获取临时资源
	SharedPtr<Resource> ResourceCache::GetTempResource(StringHash type, const String& nameIn, bool sendEventOnFailure)
	{
		String name = SanitateResourceName(nameIn);

		// 如果文件名为空，返回一个空指针
		if (name.Empty())
			return SharedPtr<Resource>();

		SharedPtr<Resource> resource;
		// 确定指针非空并且是一个资源的子类
		resource = DynamicCast<Resource>(context_->CreateObject(type));
		if (!resource)
		{
			SAPPHIRE_LOGERROR("Could not load unknown resource type " + String(type));

			if (sendEventOnFailure)
			{
				using namespace UnknownResourceType;

				VariantMap& eventData = GetEventDataMap();
				eventData[P_RESOURCETYPE] = type;
				SendEvent(E_UNKNOWNRESOURCETYPE, eventData);
			}

			return SharedPtr<Resource>();
		}

		// 尝试加载这个资源
		SharedPtr<File> file = GetFile(name, sendEventOnFailure);
		if (!file)
			return SharedPtr<Resource>();  // Error is already logged

		SAPPHIRE_LOGDEBUG("Loading temporary resource " + name);
		resource->SetName(file->GetName());

		if (!resource->Load(*(file.Get())))
		{
			// 加载不成功
			if (sendEventOnFailure)
			{
				using namespace LoadFailed;

				VariantMap& eventData = GetEventDataMap();
				eventData[P_RESOURCENAME] = name;
				SendEvent(E_LOADFAILED, eventData);
			}

			return SharedPtr<Resource>();
		}

		return resource;
	}

	//获取后台加载资源数
	unsigned ResourceCache::GetNumBackgroundLoadResources() const
	{
#ifdef SAPPHIRE_THREADING
		return backgroundLoader_->GetNumQueuedResources();
#else
		return 0;
#endif
	}

	void ResourceCache::GetResources(PODVector<Resource*>& result, StringHash type) const
	{
		result.Clear();
		//到资源组中查找类型
		HashMap<StringHash, ResourceGroup>::ConstIterator i = resourceGroups_.Find(type);
		if (i != resourceGroups_.End())
		{
			//遍历该资源组的资源，添加到result
			for (HashMap<StringHash, SharedPtr<Resource> >::ConstIterator j = i->second_.resources_.Begin();
				j != i->second_.resources_.End(); ++j)
				result.Push(j->second_);
		}
	}

	bool ResourceCache::Exists(const String& nameIn) const
	{
		MutexLock lock(resourceMutex_);

		String name = SanitateResourceName(nameIn);
		//是否Routing
		if (!isRouting_)
		{
			isRouting_ = true;
			for (unsigned i = 0; i < resourceRouters_.Size(); ++i)
				resourceRouters_[i]->Route(name, RESOURCE_CHECKEXISTS);
			isRouting_ = false;
		}

		if (name.Empty())
			return false;

		for (unsigned i = 0; i < packages_.Size(); ++i)
		{
			//包中是否存在
			if (packages_[i]->Exists(name))
				return true;
		}

		FileSystem* fileSystem = GetSubsystem<FileSystem>();
		//遍历资源目录
		for (unsigned i = 0; i < resourceDirs_.Size(); ++i)
		{
			if (fileSystem->FileExists(resourceDirs_[i] + name))
				return true;
		}

		// 回退到使用绝对目录
		return fileSystem->FileExists(name);
	}

	//获取内存预算
	unsigned long long ResourceCache::GetMemoryBudget(StringHash type) const
	{
		HashMap<StringHash, ResourceGroup>::ConstIterator i = resourceGroups_.Find(type);
		return i != resourceGroups_.End() ? i->second_.memoryBudget_ : 0;
	}

	unsigned long long ResourceCache::GetMemoryUse(StringHash type) const
	{
		//统计某一类型的资源的内存占用
		HashMap<StringHash, ResourceGroup>::ConstIterator i = resourceGroups_.Find(type);
		return i != resourceGroups_.End() ? i->second_.memoryUse_ : 0;
	}

	unsigned long long ResourceCache::GetTotalMemoryUse() const
	{
		//遍历所有资源统计总的内存占用
		unsigned long long total = 0;
		for (HashMap<StringHash, ResourceGroup>::ConstIterator i = resourceGroups_.Begin(); i != resourceGroups_.End(); ++i)
			total += i->second_.memoryUse_;
		return total;
	}

	String ResourceCache::GetResourceFileName(const String& name) const
	{
		MutexLock lock(resourceMutex_);

		FileSystem* fileSystem = GetSubsystem<FileSystem>();
		//查找文件名
		for (unsigned i = 0; i < resourceDirs_.Size(); ++i)
		{
			if (fileSystem->FileExists(resourceDirs_[i] + name))
				return resourceDirs_[i] + name;
		}

		return String();
	}

	ResourceRouter* ResourceCache::GetResourceRouter(unsigned index) const
	{
		return index < resourceRouters_.Size() ? resourceRouters_[index] : (ResourceRouter*)0;
	}

	String ResourceCache::GetPreferredResourceDir(const String& path) const
	{
		String fixedPath = AddTrailingSlash(path);

		bool pathHasKnownDirs = false;
		bool parentHasKnownDirs = false;

		FileSystem* fileSystem = GetSubsystem<FileSystem>();

		for (unsigned i = 0; checkDirs[i] != 0; ++i)
		{
			if (fileSystem->DirExists(fixedPath + checkDirs[i]))
			{
				pathHasKnownDirs = true;
				break;
			}
		}
		if (!pathHasKnownDirs)
		{
			//获得父目录
			String parentPath = GetParentPath(fixedPath);
			for (unsigned i = 0; checkDirs[i] != 0; ++i)
			{
				if (fileSystem->DirExists(parentPath + checkDirs[i]))
				{
					parentHasKnownDirs = true;
					break;
				}
			}
			// 如果目录没有已知的子目录，用父目录代替
			if (parentHasKnownDirs)
				fixedPath = parentPath;
		}

		return fixedPath;
	}

	String ResourceCache::SanitateResourceName(const String& nameIn) const
	{
		// 从这个资源名清理不支持的构造构造器
		String name = GetInternalPath(nameIn);
		name.Replace("../", "");
		name.Replace("./", "");

		// 如果路径引用到资源目录之一， 正规化资源名
		FileSystem* fileSystem = GetSubsystem<FileSystem>();
		if (resourceDirs_.Size())
		{
			//获取完整路径中的路径部分
			String namePath = GetPath(name);
			//获得程序目录
			String exePath = fileSystem->GetProgramDir();
			for (unsigned i = 0; i < resourceDirs_.Size(); ++i)
			{
				//取得资源目录i相对路径
				String relativeResourcePath = resourceDirs_[i];
				//是否是以程序目录开头
				if (relativeResourcePath.StartsWith(exePath))
					relativeResourcePath = relativeResourcePath.Substring(exePath.Length());
				//是否以资源目录开头
				if (namePath.StartsWith(resourceDirs_[i], false))
					namePath = namePath.Substring(resourceDirs_[i].Length());
				else if (namePath.StartsWith(relativeResourcePath, false))
					namePath = namePath.Substring(relativeResourcePath.Length());
			}
			//加上文件名和扩展名
			name = namePath + GetFileNameAndExtension(name);
		}

		return name.Trimmed();
	}

	String ResourceCache::SanitateResourceDirName(const String& nameIn) const
	{
		String fixedPath = AddTrailingSlash(nameIn);
		if (!IsAbsolutePath(fixedPath))
			fixedPath = GetSubsystem<FileSystem>()->GetCurrentDir() + fixedPath;

		fixedPath.Replace("/./", "/");

		return fixedPath.Trimmed();
	}

	//保存资源依赖
	void ResourceCache::StoreResourceDependency(Resource* resource, const String& dependency)
	{
		if (!resource)
			return;

		MutexLock lock(resourceMutex_);
		//获取资源名
		StringHash nameHash(resource->GetName());
		HashSet<StringHash>& dependents = dependentResources_[dependency];
		dependents.Insert(nameHash);
	}

	//重置依赖关系
	void ResourceCache::ResetDependencies(Resource* resource)
	{
		if (!resource)
			return;

		MutexLock lock(resourceMutex_);

		StringHash nameHash(resource->GetName());

		for (HashMap<StringHash, HashSet<StringHash> >::Iterator i = dependentResources_.Begin(); i != dependentResources_.End();)
		{
			HashSet<StringHash>& dependents = i->second_;
			dependents.Erase(nameHash);
			if (dependents.Empty())
				i = dependentResources_.Erase(i);
			else
				++i;
		}
	}

	//打印内存使用
	String ResourceCache::PrintMemoryUsage() const
	{
		String output = "Resource Type                 Cnt       Avg       Max    Budget     Total\n\n";
		char outputLine[256];

		unsigned totalResourceCt = 0;
		unsigned long long totalLargest = 0;
		unsigned long long totalAverage = 0;
		unsigned long long totalUse = GetTotalMemoryUse();

		for (HashMap<StringHash, ResourceGroup>::ConstIterator cit = resourceGroups_.Begin(); cit != resourceGroups_.End(); ++cit)
		{
			const unsigned resourceCt = cit->second_.resources_.Size();
			unsigned long long average = 0;
			if (resourceCt > 0)
				average = cit->second_.memoryUse_ / resourceCt;
			else
				average = 0;
			unsigned long long largest = 0;
			for (HashMap<StringHash, SharedPtr<Resource> >::ConstIterator resIt = cit->second_.resources_.Begin(); resIt != cit->second_.resources_.End(); ++resIt)
			{
				if (resIt->second_->GetMemoryUse() > largest)
					largest = resIt->second_->GetMemoryUse();
				if (largest > totalLargest)
					totalLargest = largest;
			}

			totalResourceCt += resourceCt;

			const String countString(cit->second_.resources_.Size());
			const String memUseString = GetFileSizeString(average);
			const String memMaxString = GetFileSizeString(largest);
			const String memBudgetString = GetFileSizeString(cit->second_.memoryBudget_);
			const String memTotalString = GetFileSizeString(cit->second_.memoryUse_);
			const String resTypeName = context_->GetTypeName(cit->first_);

			memset(outputLine, ' ', 256);
			outputLine[255] = 0;
			sprintf(outputLine, "%-28s %4s %9s %9s %9s %9s\n", resTypeName.CString(), countString.CString(), memUseString.CString(), memMaxString.CString(), memBudgetString.CString(), memTotalString.CString());

			output += ((const char*)outputLine);
		}

		if (totalResourceCt > 0)
			totalAverage = totalUse / totalResourceCt;

		const String countString(totalResourceCt);
		const String memUseString = GetFileSizeString(totalAverage);
		const String memMaxString = GetFileSizeString(totalLargest);
		const String memTotalString = GetFileSizeString(totalUse);

		memset(outputLine, ' ', 256);
		outputLine[255] = 0;
		sprintf(outputLine, "%-28s %4s %9s %9s %9s %9s\n", "All", countString.CString(), memUseString.CString(), memMaxString.CString(), "-", memTotalString.CString());
		output += ((const char*)outputLine);

		return output;
	}

	//查找资源
	const SharedPtr<Resource>& ResourceCache::FindResource(StringHash type, StringHash nameHash)
	{
		MutexLock lock(resourceMutex_);

		HashMap<StringHash, ResourceGroup>::Iterator i = resourceGroups_.Find(type);
		if (i == resourceGroups_.End())
			return noResource;
		HashMap<StringHash, SharedPtr<Resource> >::Iterator j = i->second_.resources_.Find(nameHash);
		if (j == i->second_.resources_.End())
			return noResource;

		return j->second_;
	}

	const SharedPtr<Resource>& ResourceCache::FindResource(StringHash nameHash)
	{
		MutexLock lock(resourceMutex_);

		for (HashMap<StringHash, ResourceGroup>::Iterator i = resourceGroups_.Begin(); i != resourceGroups_.End(); ++i)
		{
			HashMap<StringHash, SharedPtr<Resource> >::Iterator j = i->second_.resources_.Find(nameHash);
			if (j != i->second_.resources_.End())
				return j->second_;
		}

		return noResource;
	}

	//释放包资源
	void ResourceCache::ReleasePackageResources(PackageFile* package, bool force)
	{
		HashSet<StringHash> affectedGroups;

		const HashMap<String, PackageEntry>& entries = package->GetEntries();
		for (HashMap<String, PackageEntry>::ConstIterator i = entries.Begin(); i != entries.End(); ++i)
		{
			StringHash nameHash(i->first_);

			//不知道实际的资源类型，所以搜索所有的类型容器
			for (HashMap<StringHash, ResourceGroup>::Iterator j = resourceGroups_.Begin(); j != resourceGroups_.End(); ++j)
			{
				HashMap<StringHash, SharedPtr<Resource> >::Iterator k = j->second_.resources_.Find(nameHash);
				if (k != j->second_.resources_.End())
				{
					// 如果其他引用存在，不要释放，除非强制
					if ((k->second_.Refs() == 1 && k->second_.WeakRefs() == 0) || force)
					{
						j->second_.resources_.Erase(k);
						affectedGroups.Insert(j->first_);
					}
					break;
				}
			}
		}
		//更新资源组
		for (HashSet<StringHash>::Iterator i = affectedGroups.Begin(); i != affectedGroups.End(); ++i)
			UpdateResourceGroup(*i);
	}

	//更新资源组
	void ResourceCache::UpdateResourceGroup(StringHash type)
	{
		HashMap<StringHash, ResourceGroup>::Iterator i = resourceGroups_.Find(type);
		if (i == resourceGroups_.End())
			return;

		for (;;)
		{
			//重新计算内存占用
			unsigned totalSize = 0;
			unsigned oldestTimer = 0;
			HashMap<StringHash, SharedPtr<Resource> >::Iterator oldestResource = i->second_.resources_.End();

			//重新统计资源大小，使用时间
			for (HashMap<StringHash, SharedPtr<Resource> >::Iterator j = i->second_.resources_.Begin();
				j != i->second_.resources_.End(); ++j)
			{
				totalSize += j->second_->GetMemoryUse();
				unsigned useTimer = j->second_->GetUseTimer();
				if (useTimer > oldestTimer)
				{
					oldestTimer = useTimer;
					oldestResource = j;
				}
			}

			i->second_.memoryUse_ = totalSize;

			// 如果内存预算超出， 移除最老的资源并重复
			if (i->second_.memoryBudget_ && i->second_.memoryUse_ > i->second_.memoryBudget_ &&
				oldestResource != i->second_.resources_.End())
			{
				SAPPHIRE_LOGDEBUG("Resource group " + oldestResource->second_->GetTypeName() + " over memory budget, releasing resource " +
					oldestResource->second_->GetName());
				i->second_.resources_.Erase(oldestResource);
			}
			else
				break;
		}
	}

	//每帧开始的操作
	void ResourceCache::HandleBeginFrame(StringHash eventType, VariantMap& eventData)
	{
		for (unsigned i = 0; i < fileWatchers_.Size(); ++i)
		{
			String fileName;
			while (fileWatchers_[i]->GetNextChange(fileName))
			{
				//重新载入资源和依赖
				ReloadResourceWithDependencies(fileName);

				// 发送文件改变事件
				using namespace FileChanged;

				VariantMap& eventData = GetEventDataMap();
				eventData[P_FILENAME] = fileWatchers_[i]->GetPath() + fileName;
				eventData[P_RESOURCENAME] = fileName;
				SendEvent(E_FILECHANGED, eventData);
			}
		}

		// 检查后台加载资源是否可以完成
#ifdef SAPPHIRE_THREADING
	{
		SAPPHIRE_PROFILE(FinishBackgroundResources);
		backgroundLoader_->FinishResources(finishBackgroundResourcesMs_);
	}
#endif
	}

	File* ResourceCache::SearchResourceDirs(const String& nameIn)
	{
		FileSystem* fileSystem = GetSubsystem<FileSystem>();
		for (unsigned i = 0; i < resourceDirs_.Size(); ++i)
		{
			if (fileSystem->FileExists(resourceDirs_[i] + nameIn))
			{
				// 构造文件首先要用完整路径， 重命名不能包含资源路径，所以文件名可以用于GetFile()
				File* file(new File(context_, resourceDirs_[i] + nameIn));
				file->SetName(nameIn);
				return file;
			}
		}

		// 回退到使用绝对路径
		if (fileSystem->FileExists(nameIn))
			return new File(context_, nameIn);

		return 0;
	}

	File* ResourceCache::SearchPackages(const String& nameIn)
	{
		for (unsigned i = 0; i < packages_.Size(); ++i)
		{
			if (packages_[i]->Exists(nameIn))
				return new File(context_, packages_[i], nameIn);
		}

		return 0;
	}

	void RegisterResourceLibrary(Context* context)
	{
		//注册Image对象工厂
		Image::RegisterObject(context);
		//注册Json对象工厂
		JSONFile::RegisterObject(context);
		//注册PListFile对象工厂
		PListFile::RegisterObject(context);
		//注册XMLFile对象工厂
		XMLFile::RegisterObject(context);
	}

}
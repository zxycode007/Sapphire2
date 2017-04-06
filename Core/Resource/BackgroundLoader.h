#pragma once

#include "../Container/HashMap.h"
#include "../Container/HashSet.h"
#include "../Core/Mutex.h"
#include "../Container/Ptr.h"
#include "../Container/RefCounted.h"
#include "../Core/Thread.h"
#include "../Math/StringHash.h"

namespace Sapphire
{

	class Resource;
	class ResourceCache;

	/// 后台加载一个资源的队列项目
	struct BackgroundLoadItem
	{
		/// 资源指针.
		SharedPtr<Resource> resource_;
		/// 这个资源依赖的资源
		HashSet<Pair<StringHash, StringHash> > dependencies_;
		/// 依赖这个资源的资源
		HashSet<Pair<StringHash, StringHash> > dependents_;
		/// 是否发生失败事件
		bool sendEventOnFailure_;
	};

	/// 资源后台加载器，属于资源缓存ResourceCache.
	class BackgroundLoader : public RefCounted, public Thread
	{
	public:
		 
		BackgroundLoader(ResourceCache* owner);

		/// 资源后台加载循环
		virtual void ThreadFunction();

		/// 队列化一个资源加载。 名字必须确保一致的格式。如果队列化返回true (没有重复并且资源时已知的类型)
		bool QueueResource(StringHash type, const String& name, bool sendEventOnFailure, Resource* caller);
		///  等待并完成从缓存中请求加载的资源
		void WaitForResource(StringHash type, StringHash nameHash);
		/// 处理准备完成的资源
		void FinishResources(int maxMs);

		/// 返回在加载队列的资源数量
		unsigned GetNumQueuedResources() const;

	private:
		/// 终结一个后台加载资源项
		void FinishBackgroundLoading(BackgroundLoadItem& item);

		/// 资源缓存
		ResourceCache* owner_;
		/// 访问后台加载队列的线程安全互斥量
		mutable Mutex backgroundLoadMutex_;
		/// 后台加载的队列化的资源
		// pair<资源类型，资源名>
		HashMap<Pair<StringHash, StringHash>, BackgroundLoadItem> backgroundLoadQueue_;
	};
}
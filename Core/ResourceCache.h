#pragma once

#include "../Container/HashSet.h"
#include "../Container/List.h"
#include "../Core/Mutex.h"
#include "../IO/File.h"
#include "../Resource/Resource.h"

namespace Sapphire
{
	class BackgroundLoader;
	class FileWatcher;
	class PackageFile;

	// 设置优先级以便将一个包货文件推到数组的末尾
	static const unsigned PRIORITY_LAST = 0xffffffff;

	/// 特定类型的资源容器
	struct ResourceGroup
	{
		ResourceGroup() :
			memoryBudget_(0),
			memoryUse_(0)
		{
		}

		/// 内存预算
		unsigned long long memoryBudget_;
		/// 当前内存使用量
		unsigned long long memoryUse_;
		/// 资源.
		HashMap<StringHash, SharedPtr<Resource> > resources_;
	};

	/// 资源请求类型
	enum ResourceRequest
	{
		RESOURCE_CHECKEXISTS = 0,   //检查存在
		RESOURCE_GETFILE = 1       //获取文件
	};

	/// 可选资源请求处理器。可以拒绝请求，重新查询资源文件名或者处理另外的请求
	class SAPPHIRE_API ResourceRouter : public Object
	{
	public:
		 
		ResourceRouter(Context* context) :
			Object(context)
		{
		}

		/// 处理资源请求并且修改资源名字符串，名字为空的字符串意味着资源没找到或不能被访问
		virtual void Route(String& name, ResourceRequest requestType) = 0;
	};


	/// 资源缓存子系统。  在需要的时候加载资源并且保存它们以便以后访问
	class SAPPHIRE_API ResourceCache : public Object
	{
		SAPPHIRE_OBJECT(ResourceCache, Object);

	public:
		/// 构造器
		ResourceCache(Context* context);
		/// 析构，释放所有资源
		virtual ~ResourceCache();

		/// 添加一个资源加载目录。 可选优先级参数，它将控制搜索顺序
		bool AddResourceDir(const String& pathName, unsigned priority = PRIORITY_LAST);
		/// 为加载资源添加一个包文件。 可选优先级参数，它将控制搜索顺序
		bool AddPackageFile(PackageFile* package, unsigned priority = PRIORITY_LAST);
		///  用添加一个包文件加载资源。可选优先级参数，它将控制搜索顺序
		bool AddPackageFile(const String& fileName, unsigned priority = PRIORITY_LAST);
		/// 添加一个手动创建的资源， 必须有唯一的名字
		bool AddManualResource(Resource* resource);
		/// 移除一个资源加载目录
		void RemoveResourceDir(const String& pathName);
		/// 移除一个包文件。 可选释放它所加载的资源
		void RemovePackageFile(PackageFile* package, bool releaseResources = true, bool forceRelease = false);
		/// 移除一个包文件。 可选释放它所加载的资源
		void RemovePackageFile(const String& fileName, bool releaseResources = true, bool forceRelease = false);
		/// 通过名字释放一个资源
		void ReleaseResource(StringHash type, const String& name, bool force = false);
		/// 释放所有指定类型的资源
		void ReleaseResources(StringHash type, bool force = false);
		/// 释放一个特定类型和部分名字的资源
		void ReleaseResources(StringHash type, const String& partialName, bool force = false);
		/// 释放部分名字的所有资源
		void ReleaseResources(const String& partialName, bool force = false);
		/// 释放所有资源。 当force 标志 ==false时， 释放所有当前不用的资源
		void ReleaseAllResources(bool force = false);
		/// 重载如一个资源。 成功返回true。失败的请况下不会从缓存中移除资源
		bool ReloadResource(Resource* resource);
		/// 通过一个名字重载一个资源。  如果必要会重载所有依赖的资源
		void ReloadResourceWithDependencies(const String& fileName);
		/// 为指定的资源类型设置内存预估。 0是指无限制
		void SetMemoryBudget(StringHash type, unsigned long long budget);
		/// 打开或关闭当问及被修改时自动重载资源， 默认关闭
		void SetAutoReloadResources(bool enable);
		/// 打开或关闭加载失败时返回资源。 默认false 。 这对编辑资源不丢失引用属性有用
		void SetReturnFailedResources(bool enable) { returnFailedResources_ = enable; }

		/// 定义当获取资源时首先检查包文件还是目录。 true对于包，false对于目录
		void SetSearchPackagesFirst(bool value) { searchPackagesFirst_ = value; }

		/// 设置多少每帧最大多少毫秒花在完成后台加载资源上
		void SetFinishBackgroundResourcesMs(int ms) { finishBackgroundResourcesMs_ = Max(ms, 1); }

		/// 添加资源Router对象。 默认为空， 所以跳过routing处理
		void AddResourceRouter(ResourceRouter* router, bool addAsFirst = false);
		/// 移除一个资源router对象
		void RemoveResourceRouter(ResourceRouter* router);

		///从资源加载路径或包文件里打开并返回一个文件。如果没有找到， 用绝对路径回退搜索。 失败返回false
		SharedPtr<File> GetFile(const String& name, bool sendEventOnFailure = true);
		/// 通过类型和名字取得资源。 如果未曾加载则加载。如果没有找到或失败返回null. 除非调用过SetReturnFailedResources(true)。只能被主线程调用
		Resource* GetResource(StringHash type, const String& name, bool sendEventOnFailure = true);
		///  加载一个资源并不保存到资源缓存中，如果资源能安全的完全加载，那么可以被非主线程调用。(例如：GPU数据就不行)
		SharedPtr<Resource> GetTempResource(StringHash type, const String& name, bool sendEventOnFailure = true);
		/// Background load a resource. An event will be sent when complete. Return true if successfully stored to the load queue, false if eg. already exists. Can be called from outside the main thread.
		bool BackgroundLoadResource(StringHash type, const String& name, bool sendEventOnFailure = true, Resource* caller = 0);
		/// Return number of pending background-loaded resources.
		unsigned GetNumBackgroundLoadResources() const;
		/// Return all loaded resources of a specific type.
		void GetResources(PODVector<Resource*>& result, StringHash type) const;
		/// Return an already loaded resource of specific type & name, or null if not found. Will not load if does not exist.
		Resource* GetExistingResource(StringHash type, const String& name);

		/// Return all loaded resources.
		const HashMap<StringHash, ResourceGroup>& GetAllResources() const { return resourceGroups_; }

		/// Return added resource load directories.
		const Vector<String>& GetResourceDirs() const { return resourceDirs_; }

		/// Return added package files.
		const Vector<SharedPtr<PackageFile> >& GetPackageFiles() const { return packages_; }

		/// Template version of returning a resource by name.
		template <class T> T* GetResource(const String& name, bool sendEventOnFailure = true);
		/// Template version of returning an existing resource by name.
		template <class T> T* GetExistingResource(const String& name);
		/// Template version of loading a resource without storing it to the cache.
		template <class T> SharedPtr<T> GetTempResource(const String& name, bool sendEventOnFailure = true);
		/// Template version of queueing a resource background load.
		template <class T> bool BackgroundLoadResource(const String& name, bool sendEventOnFailure = true, Resource* caller = 0);
		/// Template version of returning loaded resources of a specific type.
		template <class T> void GetResources(PODVector<T*>& result) const;
		/// Return whether a file exists by name.
		bool Exists(const String& name) const;
		/// Return memory budget for a resource type.
		unsigned long long GetMemoryBudget(StringHash type) const;
		/// Return total memory use for a resource type.
		unsigned long long GetMemoryUse(StringHash type) const;
		/// Return total memory use for all resources.
		unsigned long long GetTotalMemoryUse() const;
		/// Return full absolute file name of resource if possible.
		String GetResourceFileName(const String& name) const;

		/// Return whether automatic resource reloading is enabled.
		bool GetAutoReloadResources() const { return autoReloadResources_; }

		/// Return whether resources that failed to load are returned.
		bool GetReturnFailedResources() const { return returnFailedResources_; }

		/// Return whether when getting resources should check package files or directories first.
		bool GetSearchPackagesFirst() const { return searchPackagesFirst_; }

		/// Return how many milliseconds maximum to spend on finishing background loaded resources.
		int GetFinishBackgroundResourcesMs() const { return finishBackgroundResourcesMs_; }

		/// Return a resource router by index.
		ResourceRouter* GetResourceRouter(unsigned index) const;

		/// Return either the path itself or its parent, based on which of them has recognized resource subdirectories.
		String GetPreferredResourceDir(const String& path) const;
		/// Remove unsupported constructs from the resource name to prevent ambiguity, and normalize absolute filename to resource path relative if possible.
		String SanitateResourceName(const String& name) const;
		/// Remove unnecessary constructs from a resource directory name and ensure it to be an absolute path.
		String SanitateResourceDirName(const String& name) const;
		/// Store a dependency for a resource. If a dependency file changes, the resource will be reloaded.
		void StoreResourceDependency(Resource* resource, const String& dependency);
		/// Reset dependencies for a resource.
		void ResetDependencies(Resource* resource);

		/// Returns a formatted string containing the memory actively used.
		String PrintMemoryUsage() const;

	private:
		/// Find a resource.
		const SharedPtr<Resource>& FindResource(StringHash type, StringHash nameHash);
		/// Find a resource by name only. Searches all type groups.
		const SharedPtr<Resource>& FindResource(StringHash nameHash);
		/// Release resources loaded from a package file.
		void ReleasePackageResources(PackageFile* package, bool force = false);
		/// Update a resource group. Recalculate memory use and release resources if over memory budget.
		void UpdateResourceGroup(StringHash type);
		/// Handle begin frame event. Automatic resource reloads and the finalization of background loaded resources are processed here.
		void HandleBeginFrame(StringHash eventType, VariantMap& eventData);
		/// Search FileSystem for file.
		File* SearchResourceDirs(const String& nameIn);
		/// Search resource packages for file.
		File* SearchPackages(const String& nameIn);

		/// Mutex for thread-safe access to the resource directories, resource packages and resource dependencies.
		mutable Mutex resourceMutex_;
		/// Resources by type.
		HashMap<StringHash, ResourceGroup> resourceGroups_;
		/// Resource load directories.
		Vector<String> resourceDirs_;
		/// File watchers for resource directories, if automatic reloading enabled.
		Vector<SharedPtr<FileWatcher> > fileWatchers_;
		/// Package files.
		Vector<SharedPtr<PackageFile> > packages_;
		/// Dependent resources. Only used with automatic reload to eg. trigger reload of a cube texture when any of its faces change.
		HashMap<StringHash, HashSet<StringHash> > dependentResources_;
		/// Resource background loader.
		SharedPtr<BackgroundLoader> backgroundLoader_;
		/// Resource routers.
		Vector<SharedPtr<ResourceRouter> > resourceRouters_;
		/// Automatic resource reloading flag.
		bool autoReloadResources_;
		/// Return failed resources flag.
		bool returnFailedResources_;
		/// Search priority flag.
		bool searchPackagesFirst_;
		/// Resource routing flag to prevent endless recursion.
		mutable bool isRouting_;
		/// How many milliseconds maximum per frame to spend on finishing background loaded resources.
		int finishBackgroundResourcesMs_;
	};

	template <class T> T* ResourceCache::GetExistingResource(const String& name)
	{
		StringHash type = T::GetTypeStatic();
		return static_cast<T*>(GetExistingResource(type, name));
	}

	template <class T> T* ResourceCache::GetResource(const String& name, bool sendEventOnFailure)
	{
		StringHash type = T::GetTypeStatic();
		return static_cast<T*>(GetResource(type, name, sendEventOnFailure));
	}

	template <class T> SharedPtr<T> ResourceCache::GetTempResource(const String& name, bool sendEventOnFailure)
	{
		StringHash type = T::GetTypeStatic();
		return StaticCast<T>(GetTempResource(type, name, sendEventOnFailure));
	}

	template <class T> bool ResourceCache::BackgroundLoadResource(const String& name, bool sendEventOnFailure, Resource* caller)
	{
		StringHash type = T::GetTypeStatic();
		return BackgroundLoadResource(type, name, sendEventOnFailure, caller);
	}

	template <class T> void ResourceCache::GetResources(PODVector<T*>& result) const
	{
		PODVector<Resource*>& resources = reinterpret_cast<PODVector<Resource*>&>(result);
		StringHash type = T::GetTypeStatic();
		GetResources(resources, type);

		for (unsigned i = 0; i < result.Size(); ++i)
		{
			Resource* resource = resources[i];
			result[i] = static_cast<T*>(resource);
		}
	}

	/// 注册资源库子系统和对象
	void SAPPHIRE_API RegisterResourceLibrary(Context* context);

}

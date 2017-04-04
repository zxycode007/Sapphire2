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
		/// 后台加载一个资源。 当完成时发送一个事件。 如果成功返回true并保存到加载队列。    如果已经存在返回false， 可以被非主线程调用
		bool BackgroundLoadResource(StringHash type, const String& name, bool sendEventOnFailure = true, Resource* caller = 0);
		/// 返回后台要加载的资源数
		unsigned GetNumBackgroundLoadResources() const;
		/// 返回所有加载的特定类型的资源
		void GetResources(PODVector<Resource*>& result, StringHash type) const;
		/// 返回一个特定类型和名字的已加载资源。如果没找到返回空
		Resource* GetExistingResource(StringHash type, const String& name);

		/// 返回所有已加载资源
		const HashMap<StringHash, ResourceGroup>& GetAllResources() const { return resourceGroups_; }

		/// 返回所有添加的资源加载目录
		const Vector<String>& GetResourceDirs() const { return resourceDirs_; }

		/// 返回所有添加的包文件
		const Vector<SharedPtr<PackageFile> >& GetPackageFiles() const { return packages_; }

		/// 获取模板类型的资源
		template <class T> T* GetResource(const String& name, bool sendEventOnFailure = true);
		/// 获取一个已存在的资源
		template <class T> T* GetExistingResource(const String& name);
		/// 获取没有保存在缓存中的资源
		template <class T> SharedPtr<T> GetTempResource(const String& name, bool sendEventOnFailure = true);
		/// 后台加载资源的模板化版本
		template <class T> bool BackgroundLoadResource(const String& name, bool sendEventOnFailure = true, Resource* caller = 0);
		/// 加载特定类型资源的模板化版本
		template <class T> void GetResources(PODVector<T*>& result) const;
		/// 返回一个文件是否存在
		bool Exists(const String& name) const;
		/// 返回一个资源类型的内存预估
		unsigned long long GetMemoryBudget(StringHash type) const;
		/// 返回一个资源类型的总内存占用
		unsigned long long GetMemoryUse(StringHash type) const;
		/// 返回所有资源占用的总内存
		unsigned long long GetTotalMemoryUse() const;
		/// 如果可以返回文件名的绝对路径
		String GetResourceFileName(const String& name) const;

		/// 返回自动加载资源是否打开
		bool GetAutoReloadResources() const { return autoReloadResources_; }

		/// 返回是否返回加载失败的资源
		bool GetReturnFailedResources() const { return returnFailedResources_; }

		/// 返回是否搜索包优先
		bool GetSearchPackagesFirst() const { return searchPackagesFirst_; }

		/// 返回花在后台加载资源的最大毫秒数
		int GetFinishBackgroundResourcesMs() const { return finishBackgroundResourcesMs_; }

		/// 取得一个资源router
		ResourceRouter* GetResourceRouter(unsigned index) const;

		/// 返回路径或它的父路径组织的资源子目录
		String GetPreferredResourceDir(const String& path) const;
		/// 从资源名移除不支持的构造函数，避免二义性。并且如果可以标准化绝对文件名到资源路径的相对路径
		String SanitateResourceName(const String& name) const;
		/// 从一个资源目录名移除不必要的构造器并且确保它作为一个绝对路径
		String SanitateResourceDirName(const String& name) const;
		/// 保存一个资源的依赖。 如果依赖文件改变， 这个资源将会重载
		void StoreResourceDependency(Resource* resource, const String& dependency);
		/// 重载一个资源的依赖的关系
		void ResetDependencies(Resource* resource);

		/// 返回一个包含内存使用状态的格式化的字符串
		String PrintMemoryUsage() const;

	private:
		/// 查找资源
		const SharedPtr<Resource>& FindResource(StringHash type, StringHash nameHash);
		/// 只通过名字查找资源。搜索所有的类型组
		const SharedPtr<Resource>& FindResource(StringHash nameHash);
		/// 从一个包文件释放加载的资源
		void ReleasePackageResources(PackageFile* package, bool force = false);
		/// 更新一个资源组。 如果超过内存预算，重新计算内存使用量并释放资源
		void UpdateResourceGroup(StringHash type);
		/// 处理begin frame事件. 资源自动重载并且结束后台加载资源的处理
		void HandleBeginFrame(StringHash eventType, VariantMap& eventData);
		/// 从目录搜索文件
		File* SearchResourceDirs(const String& nameIn);
		/// 从包搜索文件
		File* SearchPackages(const String& nameIn);

		/// 线程安全访问资源目录，资源包和资源依赖的互斥量
		mutable Mutex resourceMutex_;
		/// 资源类型
		HashMap<StringHash, ResourceGroup> resourceGroups_;
		/// 资源加载目录
		Vector<String> resourceDirs_;
		/// 资源目录的文件观察者，如果打开自动重载的话
		Vector<SharedPtr<FileWatcher> > fileWatchers_;
		/// 包文件
		Vector<SharedPtr<PackageFile> > packages_;
		/// 资源依赖。 只用于自动重载。 
		HashMap<StringHash, HashSet<StringHash> > dependentResources_;
		/// 后台资源加载器
		SharedPtr<BackgroundLoader> backgroundLoader_;
		/// 资源Router
		Vector<SharedPtr<ResourceRouter> > resourceRouters_;
		/// 资源自动重载标志
		bool autoReloadResources_;
		/// 返回失败的资源标志
		bool returnFailedResources_;
		/// 搜索优先级标志
		bool searchPackagesFirst_;
		/// 资源routing标志，避免无限递归
		mutable bool isRouting_;
		/// 每帧用在后台加载资源的最大毫秒数
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
		//转换资源类型
		PODVector<Resource*>& resources = reinterpret_cast<PODVector<Resource*>&>(result);
		//获取类型信息
		StringHash type = T::GetTypeStatic();
		//获取资源
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

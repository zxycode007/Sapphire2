#pragma once

#include "../Core/Object.h"
#include "../Core/Timer.h"


namespace Sapphire
{
	class Deserializer;
	class Serializer;

	/// 异步加载一个资源状态
	enum AsyncLoadState
	{
		/// 没有异步操作
		ASYNC_DONE = 0,
		/// 异步加载的查询
		ASYNC_QUEUED = 1,
		/// 在工作线程中调用BeginLoad()的进度
		ASYNC_LOADING = 2,
		/// BeginLoad() 成功。 EndLoad()将会被主线程调用
		ASYNC_SUCCESS = 3,
		/// BeginLoad() 失败.
		ASYNC_FAIL = 4
	};

	///资源类的基类
	class SAPPHIRE_API Resource : public Object
	{
		SAPPHIRE_OBJECT(Resource, Object);

	public:
		 
		Resource(Context* context);

		/// 同步加载资源。 如果两个都成功，调用BeginLoad() 和  EndLoad()都返回true
		bool Load(Deserializer& source);
		/// 从流中加载资源。 可以从工作线程被调用。 如果返回true完成
		virtual bool BeginLoad(Deserializer& source);
		/// 资源加载完成。通常在主线程里调用。 如果为true，则成功
		virtual bool EndLoad();
		/// 保存资源。如果成功返回true
		virtual bool Save(Serializer& dest) const;

		/// 设置名字
		void SetName(const String& name);
		/// 守则内存使用量，可以大概值
		void SetMemoryUse(unsigned size);
		/// 重置最后使用的timer
		void ResetUseTimer();
		/// 守则异步加载状态。 由资源缓冲调用。资源在异步加载当中不能正常返回给用户
		void SetAsyncLoadState(AsyncLoadState newState);

		/// 返回名字
		const String& GetName() const { return name_; }

		/// 返回名字的Hash
		StringHash GetNameHash() const { return nameHash_; }

		/// 返回内存使用量，可能是近似值
		unsigned GetMemoryUse() const { return memoryUse_; }

		/// 返回上次使用的时间毫秒数。 如果引用不在资源缓冲，总是返回0
		unsigned GetUseTimer();

		/// 返回异步加载的状态
		AsyncLoadState GetAsyncLoadState() const { return asyncLoadState_; }

	private:
		//名字
		String name_;
		/// 名字hash值
		StringHash nameHash_;
		/// 上次使用的时间
		Timer useTimer_;
		/// 内存使用字节
		unsigned memoryUse_;
		/// 异步加载状态
		AsyncLoadState asyncLoadState_;
	};

	inline const String& GetResourceName(Resource* resource)
	{
		return resource ? resource->GetName() : String::EMPTY;
	}

	inline StringHash GetResourceType(Resource* resource, StringHash defaultType)
	{
		return resource ? resource->GetType() : defaultType;
	}

	inline ResourceRef GetResourceRef(Resource* resource, StringHash defaultType)
	{
		return ResourceRef(GetResourceType(resource, defaultType), GetResourceName(resource));
	}

	template <class T> Vector<String> GetResourceNames(const Vector<SharedPtr<T> >& resources)
	{
		Vector<String> ret(resources.Size());
		for (unsigned i = 0; i < resources.Size(); ++i)
			ret[i] = GetResourceName(resources[i]);

		return ret;
	}

	template <class T> ResourceRefList GetResourceRefList(const Vector<SharedPtr<T> >& resources)
	{
		return ResourceRefList(T::GetTypeStatic(), GetResourceNames(resources));
	}
}




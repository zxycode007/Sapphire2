#include "../../Sapphire/Predefined.h"

#include "../Core/Profiler.h"
#include "../IO/Log.h"
#include "../Resource/Resource.h"


namespace Sapphire
{

	Resource::Resource(Context* context) :
		Object(context),
		memoryUse_(0),
		asyncLoadState_(ASYNC_DONE)
	{
	}

	bool Resource::Load(Deserializer& source)
	{
		// 由于BeginLoad() / EndLoad()是在工作线程中调用，profiling将会空操作
		// 创建基于类型名的profile块
#ifdef SAPPHIRE_PROFILING
		String profileBlockName("Load" + GetTypeName());

		Profiler* profiler = GetSubsystem<Profiler>();
		if (profiler)
			profiler->BeginBlock(profileBlockName.CString());
#endif
		//是否是主线程决定是否为同步或异步
		SetAsyncLoadState(Thread::IsMainThread() ? ASYNC_DONE : ASYNC_LOADING);
		bool success = BeginLoad(source);  //从流中加载资源
		if (success)
			success &= EndLoad(); 
		SetAsyncLoadState(ASYNC_DONE);

#ifdef SAPPHIRE_PROFILING
		if (profiler)
			profiler->EndBlock();
#endif

		return success;
	}

	bool Resource::BeginLoad(Deserializer& source)
	{
		// 这个需要从子类覆写
		return false;
	}

	bool Resource::EndLoad()
	{
		// 如果不用上传GPU，则可以不用覆写
		return true;
	}

	bool Resource::Save(Serializer& dest) const
	{
		SAPPHIRE_LOGERROR("Save not supported for " + GetTypeName());
		return false;
	}

	void Resource::SetName(const String& name)
	{
		name_ = name;
		nameHash_ = name;
	}

	void Resource::SetMemoryUse(unsigned size)
	{
		memoryUse_ = size;
	}

	void Resource::ResetUseTimer()
	{
		useTimer_.Reset();
	}

	void Resource::SetAsyncLoadState(AsyncLoadState newState)
	{
		asyncLoadState_ = newState;
	}

	unsigned Resource::GetUseTimer()
	{
		// 如果多于1个resource cache引用， 返回0重置Timer
		if (Refs() > 1)
		{
			useTimer_.Reset();
			return 0;
		}
		else
			return useTimer_.GetMSec(false);
	}
}
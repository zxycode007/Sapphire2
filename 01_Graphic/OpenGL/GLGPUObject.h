#pragma once
#include "Sapphire.h"
#include "Ptr.h"

namespace Sapphire
{
	class VideoDriver;
	class SAPPHIRE_API GPUObject
	{
	public:

		GPUObject(VideoDriver* drv);
		virtual ~GPUObject();

		void Release();

		virtual void OnDeviceLost();

		virtual void OnDeviceReset();

		VideoDriver*  GetVideoDiver();

		unsigned GetGPUHandle();

	private:
		WeakPtr<VideoDriver> mDriver;
		unsigned  mHandle;
	};
}

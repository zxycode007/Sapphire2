#include "../OpenGL/GLGPUObject.h"
#include "VideoDriver.h"

Sapphire::GPUObject::GPUObject(VideoDriver* drv) : mHandle(0),mDriver(drv)
{
}

void Sapphire::GPUObject::Release()
{
}

void Sapphire::GPUObject::OnDeviceLost()
{
	mHandle = 0;
}

void Sapphire::GPUObject::OnDeviceReset()
{
}

Sapphire::VideoDriver * Sapphire::GPUObject::GetVideoDiver()
{
	return mDriver;
}

unsigned Sapphire::GPUObject::GetGPUHandle()
{
	return mHandle;
}

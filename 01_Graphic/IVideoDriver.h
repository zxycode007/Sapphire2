#pragma once
#include "Object.h"

namespace Sapphire
{
	class WindowsHandle;
	class ShaderManager;


	class  SAPPHIRE_API  VideoDriver : public Object
	{
		SAPPHIRE_OBJECT(VideoDriver, Object);

	public:

		VideoDriver(Context* context);
		~VideoDriver();

		virtual void        SetWindow(WindowsHandle* window) = 0;
		virtual WindowsHandle*  GetWindow() = 0;
		virtual void        SetShaderManager(Sapphire::ShaderManager* manager) = 0;
		virtual ShaderManager*  GetShaderManager() = 0;
		virtual void release() = 0;

	};
}

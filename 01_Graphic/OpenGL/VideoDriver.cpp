#include "VideoDriver.h"
#include "Context.h"
#include "Color.h"
#include "../ITexture.h"

Sapphire::VideoDriver::VideoDriver(Context * context) :Object(context)
{
}

Sapphire::VideoDriver::~VideoDriver()
{
}

bool Sapphire::VideoDriver::m_gl3support = false;

void Sapphire::VideoDriver::Initialize()
{
	//glfwInit来初始化GLFW，然后我们可以使用glfwWindowHint来配置GLFW。
	glfwInit();
	//glfwWindowHint的第一个参数告诉我们，配置哪个选项，这些选项带有GLFW_前缀。第二个参数是一个整数，它代表我们为选项所设置的值
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, SAPPHIRE_OPENGL_VERSION_MAJOR); //告诉GLFW我们使用的OpenGL版本是3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, SAPPHIRE_OPENGL_VERSION_MIRROR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //我们同样告诉GLFW，我们希望明确地使用core-profile
	if (m_canResize)
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	}
	else
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}

	SetShaderManager(context_->GetSubsystem<ShaderManager>());

	


}

void Sapphire::VideoDriver::SetWindow(WindowsHandle * window)
{
	if (window)
	{
		glfwMakeContextCurrent(window);
		if (mWindowHandle)
		{
			glfwDestroyWindow(mWindowHandle);
		}
		mWindowHandle = window;
	}
}

Sapphire::WindowsHandle * Sapphire::VideoDriver::GetWindow()
{
	return mWindowHandle==NULL? mWindowHandle:NULL;
}

void Sapphire::VideoDriver::SetShaderManager(Sapphire::ShaderManager * manager)
{
	if (manager)
	{
		if (mShaderManager.NotNull())
		{
			mShaderManager.Reset();
		}
		mShaderManager = manager;

	}
}

Sapphire::ShaderManager * Sapphire::VideoDriver::GetShaderManager()
{
	return mShaderManager.NotNull()?mShaderManager.Get():NULL;
}

Sapphire::ETextureFilterMode Sapphire::VideoDriver::GetDefaultTextureFilterMode()
{
	return ETextureFilterMode::EFILTER_NEAREST;
}

void Sapphire::VideoDriver::SetTextureParametersDirty()
{
	MutexLock lock(m_gpuobjectsMutex);
	for (PODVector<GPUObject*>::Iterator it = m_gpuobjects.Begin(); it != m_gpuobjects.End(); ++it)
	{
		Texture* texture = dynamic_cast<Texture*>(*it);
		if (texture)
			texture->SetParametersDirty();   //设置纹理参数发生变更
	}
}

void Sapphire::VideoDriver::BeginFrame()
{
}

void Sapphire::VideoDriver::render()
{
}

void Sapphire::VideoDriver::createWindow()
{
}

void Sapphire::VideoDriver::EndFrame()
{
}

void Sapphire::VideoDriver::release()
{
	glfwTerminate();
}

int Sapphire::VideoDriver::getWidth()
{
	return m_windth;
}

void Sapphire::VideoDriver::setWidth(int width)
{
	m_windth = width;
}

void Sapphire::VideoDriver::setClearColor(Color c)
{
	m_clearColor = c;
}

Sapphire::Color Sapphire::VideoDriver::getClearColor()
{
	return m_clearColor;
}

void Sapphire::VideoDriver::setWindowName(String name)
{
	m_windowName = name;
}

Sapphire::String Sapphire::VideoDriver::getWindowName()
{
	return m_windowName;
}

void Sapphire::VideoDriver::setTexture(int index, Texture* tex)
{
	if (index > SAPPHIRE_MAX_TEXTURE_UNIT)
	{
		SAPPHIRE_LOGGING("texture index exceeded!");
		return;
	}
	
	//纹理已存在
	if (m_textures[index] != tex)
	{
		if (m_activeTexture != index)
		{
			//激活该纹理
			glActiveTexture(GL_TEXTURE0 + index);
			m_activeTexture = index;
		}

		if (tex)
		{
			//如果纹理有效
			unsigned glType = tex->GetTextureTarget();
			if (m_textrueTypes[index] && m_textrueTypes[index] != glType)
			{
				//取消原绑定的纹理
				glBindTexture(m_textrueTypes[index], 0);
			}
			//绑定现在的纹理
			glBindTexture(glType, tex->GetGPUHandle());
			m_textrueTypes[index] = glType;

		}
		else if (m_textrueTypes[index])
		{
			//解除原理的绑定
			glBindTexture(m_textrueTypes[index], 0);
			m_textrueTypes[index] = 0;
		}

	}
	else
	{
		if (tex && tex->GetTextureParametersDirty())
		{
			if (m_activeTexture != index)
			{    
				//激活新的纹理
				glActiveTexture(GL_TEXTURE0 + index);
				m_activeTexture = index;
			}
			glBindTexture(m_textrueTypes[index], tex->GetGPUHandle());
		}
	}

}
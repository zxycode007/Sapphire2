#include "VideoDriver.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Context.h"
#include "Color.h"

Sapphire::VideoDriver::VideoDriver(Context * context) :Object(context)
{
}

Sapphire::VideoDriver::~VideoDriver()
{
}

void Sapphire::VideoDriver::Initialize()
{
	//glfwInit����ʼ��GLFW��Ȼ�����ǿ���ʹ��glfwWindowHint������GLFW��
	glfwInit();
	//glfwWindowHint�ĵ�һ�������������ǣ������ĸ�ѡ���Щѡ�����GLFW_ǰ׺���ڶ���������һ������������������Ϊѡ�������õ�ֵ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, SAPPHIRE_OPENGL_VERSION_MAJOR); //����GLFW����ʹ�õ�OpenGL�汾��3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, SAPPHIRE_OPENGL_VERSION_MIRROR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //����ͬ������GLFW������ϣ����ȷ��ʹ��core-profile
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
		if (mWindowHandle.NotNull())
		{
			glfwDestroyWindow(mWindowHandle.Get());
			mWindowHandle.Reset();
		}
		mWindowHandle = window;
	}
}

Sapphire::WindowsHandle * Sapphire::VideoDriver::GetWindow()
{
	return mWindowHandle.NotNull()? mWindowHandle.Get():NULL;
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
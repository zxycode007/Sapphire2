#pragma once

#include "stdafx.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Line.h"
#include "Geometry.h"

typedef ShaderStruct;
typedef GLFWwindow WindowsHandle;

class IVideoDriver
{
public:

	virtual void        SetWindow(WindowsHandle* window) = 0;
	virtual WindowsHandle*  GetWindow() = 0;
	virtual void        drawLine(const Sapphire::Line3d& line, const char* shaderName) = 0;
	virtual void        drawGeometry(Sapphire::Geometry* geo, const char* shaderName) = 0;
	virtual void        SetShaderManager(ShaderManager* manager) = 0;
	virtual ShaderManager*  GetShaderManager() = 0;
	virtual void release() = 0;

};

class OpenGLVideoDriver : public IVideoDriver
{
public:
	

	OpenGLVideoDriver()
	{
		m_pShaderManager = 0;
		mWindow = 0;
	}

	OpenGLVideoDriver(WindowsHandle* handle, ShaderManager* pShaderManager)
	{
		mWindow = handle;
		m_pShaderManager = pShaderManager;
	}
   
	WindowsHandle*  GetWindow() { return mWindow; };
	void        SetWindow(WindowsHandle* window) { mWindow = window; };
	void        SetShaderManager(ShaderManager* manager) { m_pShaderManager = manager; };
	void        drawLine(const Sapphire::Line3d& line, const char* shaderName);
	void        drawGeometry(Sapphire::Geometry* geo, const char* shaderName);
	ShaderManager*  GetShaderManager();
	void        release();

private:


	//要绘图的窗口句柄
	WindowsHandle* mWindow;
	//shader管理器
	ShaderManager* m_pShaderManager;

};
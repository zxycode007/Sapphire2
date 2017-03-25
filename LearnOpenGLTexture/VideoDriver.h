#pragma once

#include "stdafx.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Line.h"
#include "Geometry.h"

typedef ShaderStruct;
typedef GLFWwindow * WindowHandle;

class OpenGLVideoDriver
{
public:
	typedef std::vector<Sapphire::Geometry> RenderList;

	OpenGLVideoDriver()
	{
		m_pShaderManager = 0;
		mWindow = 0;
		mCurShader = 0;
	}

	OpenGLVideoDriver(WindowHandle* handle, ShaderManager* pShaderManager)
	{
		mWindow = handle;
		m_pShaderManager = pShaderManager;
	}

	bool Load(ShaderStruct* shader);
	ShaderStruct*  GetShader() { return mCurShader; };
	WindowHandle*  GetWindow() { return mWindow; };
	void        SetWindow(WindowHandle* window) { mWindow = window; };
	void        drawLine(const Sapphire::Line3d& line);
	void        drawRenderList();

private:
	//要绘图的窗口句柄
	WindowHandle* mWindow;
	//当前临时shader
	ShaderStruct* mCurShader;
	//shader管理器
	ShaderManager* m_pShaderManager;
	//渲染列表
	RenderList       mRenderList;
};
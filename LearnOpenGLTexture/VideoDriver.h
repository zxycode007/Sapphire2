#pragma once

#include "stdafx.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Line.h"

typedef ShaderStruct;
typedef GLFWwindow * WindowHandle;

class OpenGLVideoDriver
{
public:

	bool Load(ShaderStruct* shader);
	ShaderStruct*  GetShader() { return mShader; };
	WindowHandle*  GetWindow() { return mWindow; };
	void        SetWindow(WindowHandle* window) { mWindow = window; };
	void        drawLine(const Sapphire::Line3d& line);

private:

	WindowHandle* mWindow;
	ShaderStruct* mShader;

};
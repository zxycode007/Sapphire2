#pragma once
#include <string>
#include <unordered_map>
#include <GL\glew.h>


namespace Sapphire
{
	struct Shader
	{
		std::string  ShaderName;
		std::string  VertexShaderSrc;
		std::string  FragmentShaderSrc;
		GLuint    ShaderProgram;
		GLuint    VertexShaderHandle;
		GLuint    FragmentShaderHandle;
		bool    isCompiled;

		Shader()
		{
			VertexShaderHandle = 0;
			FragmentShaderHandle = 0;
			ShaderProgram = 0;
			isCompiled = false;
		}

	};


}
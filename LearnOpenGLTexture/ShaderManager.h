#pragma once

#include "stdafx.h"
#include <string>
#include <unordered_map>
#include <GL\glew.h>


struct ShaderStruct
{
	string  ShaderName;
	string  VertexShaderSrc;
	string  FragmentShaderSrc;
	GLuint    ShaderProgram;
	GLuint    VertexShaderHandle;
	GLuint    FragmentShaderHandle;
	bool    isCompiled;

	ShaderStruct()
	{
		VertexShaderHandle = 0;
		FragmentShaderHandle = 0;
		ShaderProgram = 0;
		isCompiled = false;
	}

};


class ShaderManager
{
public:

	ShaderManager() {};
	~ShaderManager() {};

	void Release();

	ShaderStruct*  CreateShaderProgram(string shaderName, string vertexShaderSrc, string fragmentShaderSrc);
	bool  CompileAndLink(ShaderStruct* shader);
	ShaderStruct*  FindShader(string shaderName);
	void  ReleaseShader(string shaderName);
	void  PrintLogs();

private:

	hash_map<string, ShaderStruct*> shaderMap;
	vector<string>                 logs;

	void  DeleteShader(ShaderStruct*& shader);




};

namespace Sapphire
{
	typedef ShaderStruct Shader;
	
}
#pragma once

#pragma once

#include "stdafx.h"
#include <string>
#include <unordered_map>
#include <GL\glew.h>
#include "Shader.h"



namespace Sapphire
{
	


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

	typedef ShaderStruct Shader;
	
}
#pragma once

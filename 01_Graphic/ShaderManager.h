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

		Shader*  CreateShaderProgram(string shaderName, string vertexShaderSrc, string fragmentShaderSrc);
		bool     CompileAndLink(Shader* shader);
		Shader*  FindShader(string shaderName);
		void  ReleaseShader(string shaderName);
		void  PrintLogs();

	private:

		hash_map<string, Shader*> shaderMap;
		vector<string>                 logs;

		void  DeleteShader(Shader*& shader);




	};
}
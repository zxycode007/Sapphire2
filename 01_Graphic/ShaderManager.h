#pragma once

#include "stdafx.h"
#include <string>
#include <unordered_map>
#include <GL\glew.h>
#include "Shader.h"

namespace Sapphire
{
	class SAPPHIRE_API ShaderManager : public Object
	{
		SAPPHIRE_OBJECT(ShaderManager, Object);

	public:

		ShaderManager(Context* ctx);
		virtual ~ShaderManager() {};

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
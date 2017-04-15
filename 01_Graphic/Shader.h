#pragma once
#include <string>
#include <unordered_map>
#include <GL\glew.h>
#include "Resource\Resource.h"


namespace Sapphire
{
	class SAPPHIRE_API Shader :public Resource
	{
		SAPPHIRE_OBJECT(Shader, Resource);

	public:

		Shader(Context* ctx);
		virtual ~Shader();
		static void RegisterObject(Context* context);

		String  ShaderName;
		String  VertexShaderSrc;
		String  FragmentShaderSrc;
		GLuint    ShaderProgram;
		GLuint    VertexShaderHandle;
		GLuint    FragmentShaderHandle;
		bool    isCompiled;
		

	};


}
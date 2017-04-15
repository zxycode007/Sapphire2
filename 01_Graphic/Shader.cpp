#include "Shader.h"
#include <Context.h>
Sapphire::Shader::Shader(Context * ctx): Resource(ctx)
{
	VertexShaderHandle = 0;
	FragmentShaderHandle = 0;
	ShaderProgram = 0;
	isCompiled = false;
}

Sapphire::Shader::~Shader()
{
}

void Sapphire::Shader::RegisterObject(Context * context)
{
	context->RegisterFactory<Shader>();
}

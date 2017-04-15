#include "ShaderManager.h"
#include <sstream>
#include <iostream>
#include  <GL\glew.h>

Sapphire::ShaderManager::ShaderManager(Context* ctx) : Object(ctx)
{
}

void Sapphire::ShaderManager::Release()
{
	hash_map<string, Shader*>::iterator it = shaderMap.begin();

	while (it != shaderMap.begin())
	{
		Shader* shader = it->second;
		DeleteShader(shader);
	}

	delete this;
}

Sapphire::Shader * Sapphire::ShaderManager::CreateShaderProgram(string shaderName, string vertexShaderSrc, string fragmentShaderSrc)
{
	SharedPtr<Shader> shader = DynamicCast<Shader>(context_->CreateObject(Shader::GetTypeStatic()));
	shader->ShaderName = shaderName;
	shader->VertexShaderSrc = vertexShaderSrc;
	shader->FragmentShaderSrc = fragmentShaderSrc;

	shader->ShaderProgram = glCreateProgram();
	shader->VertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	shader->FragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

	shaderMap[shaderName] = shader;

	return shader;
}

bool Sapphire::ShaderManager::CompileAndLink(Shader* shader)
{
	const GLchar* vertexShaderSource = shader->VertexShaderSrc.c_str();
	glShaderSource(shader->VertexShaderHandle, 1, &vertexShaderSource, NULL);
	glCompileShader(shader->VertexShaderHandle);
	GLint success;
	GLchar infoLog[512];
	stringstream ss;
	//»ñÈ¡±àÒë×´Ì¬
	glGetShaderiv(shader->VertexShaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader->VertexShaderHandle, 512, NULL, infoLog);
		ss << " Shader Name:" << shader->ShaderName.c_str();
		ss << "    ERROR::SHADER::VERTEX::COMPILATION_FAILED   \n" << infoLog << endl;
		logs.push_back(ss.str());
		ss.str("");
		return false;
	}

	const GLchar* fragmentShaderSource = shader->FragmentShaderSrc.c_str();
	glShaderSource(shader->FragmentShaderHandle, 1, &fragmentShaderSource, NULL);
	glCompileShader(shader->FragmentShaderHandle);
	//»ñÈ¡±àÒë×´Ì¬
	glGetShaderiv(shader->FragmentShaderHandle, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader->FragmentShaderHandle, 512, NULL, infoLog);
		ss << " Shader Name:" << shader->ShaderName.c_str();
		ss << "    ERROR::SHADER::FRAGMENT::COMPILATION_FAILED    \n" << infoLog << endl;
		logs.push_back(ss.str());
		ss.str("");
		return false;
	}

	glAttachShader(shader->ShaderProgram, shader->VertexShaderHandle);
	glAttachShader(shader->ShaderProgram, shader->FragmentShaderHandle);
	glLinkProgram(shader->ShaderProgram);
	glGetProgramiv(shader->ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader->ShaderProgram, 512, NULL, infoLog);
		GLenum errorcode = glGetError();
		ss << " Shader Name:" << shader->ShaderName.c_str();
		ss << "    SHADER::PROGRAME::LINK_FAILED   \n" << infoLog << "  ERROR CODE: " << errorcode << infoLog << endl;
		logs.push_back(ss.str());
		ss.str("");
		return false;
	}
	shader->isCompiled = true;
	return true;
}

Sapphire::Shader * Sapphire::ShaderManager::FindShader(string shaderName)
{
	Shader* shader = shaderMap[shaderName];
	return shader;
}


void Sapphire::ShaderManager::ReleaseShader(string shaderName)
{
	Shader* shader = FindShader(shaderName);
	DeleteShader(shader);

}

void Sapphire::ShaderManager::DeleteShader(Shader*& shader)
{
	if (shader)
		return;
	if (shader->VertexShaderHandle)
	{
		glDeleteShader(shader->VertexShaderHandle);
	}
	if (shader->FragmentShaderHandle)
	{
		glDeleteShader(shader->FragmentShaderHandle);
	}
	if (shader->ShaderProgram)
	{
		glDeleteProgram(shader->ShaderProgram);
	}
	hash_map<string, Shader*>::iterator it = shaderMap.find(shader->ShaderName);
	if (it != shaderMap.end())
	{
		shaderMap.erase(it);
	}
	delete shader;
	shader = NULL;
}

void Sapphire::ShaderManager::PrintLogs()
{
	for (int i = 0; i < logs.size(); i++)
	{
		cout << logs[i] << endl;
	}
}



#include "stdafx.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "ShaderManager.h"
#include "Mesh.h"
#include <iostream>



bool Sapphire::SMeshNode::Load(MeshStruct* mesh, MeshBuffer::BufferMode mode)
{
	if (!mesh)
		return false;
	mMeshBuffer = new MeshBuffer(mesh, mWindow, mode);
	return true;
}

bool Sapphire::SMeshNode::Load(ShaderStruct * shader)
{
	if (!shader->isCompiled)
	{
		return false;
	}
	mShader = shader;
	return true;
}

void Sapphire::SMeshNode::Release()
{
	if (mMeshBuffer)
	{
		mMeshBuffer->Release();
		mMeshBuffer = NULL;
	}
	mWindow = NULL;
	mShader = NULL;

	delete this;
}

void Sapphire::SMeshNode::DrawMesh()
{
	mMeshBuffer->DrawMeshBuffer(mShader);
}

bool Sapphire::MeshBuffer::Initialize()
{
	//创建VBO缓冲区
	glGenBuffers(1, &mVbo);
	//创建VAO
	glGenVertexArrays(1, &mVao);
	//创建EBO对象 (元素缓冲对象  画顶点还是元素)
	glGenBuffers(1, &mEbo);
	mIsInit = true;
	return true;
}

void Sapphire::MeshBuffer::Release()
{
	delete mMesh;
	delete this;
}

void Sapphire::MeshBuffer::DrawMeshBuffer(ShaderStruct * shader)
{
	if (shader == NULL || shader->isCompiled == false)
	{
		return;
	}
	Initialize();
	//设置VBO
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	switch (mMode)
	{
	case BufferMode::Static_Draw:
	{
		glBufferData(GL_ARRAY_BUFFER, mMesh->vertexBufferSize, mMesh->vertices, GL_STATIC_DRAW);
		break;
	}
	case BufferMode::Dynamic_Draw:
	{
		glBufferData(GL_ARRAY_BUFFER, mMesh->vertexBufferSize, mMesh->vertices, GL_DYNAMIC_DRAW);
		break;
	}
	case BufferMode::Stream_Draw:
	{
		glBufferData(GL_ARRAY_BUFFER, mMesh->vertexBufferSize, mMesh->vertices, GL_STREAM_DRAW);
		break;
	}
	default:
		break;
	}

	//设置VAO
	{
		glBindVertexArray(mVao);
		//设置顶点属性结构
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
		//启用顶点属性数组（index < MAX_VERTEX_ATTRBS-1）
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		//设置EBO
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
			switch (mMode)
			{
			case BufferMode::Static_Draw:
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->indexBufferSize, mMesh->indices, GL_STATIC_DRAW);
				break;
			}
			case BufferMode::Dynamic_Draw:
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->indexBufferSize, mMesh->indices, GL_DYNAMIC_DRAW);
				break;
			}
			case BufferMode::Stream_Draw:
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->indexBufferSize, mMesh->indices, GL_STREAM_DRAW);
				break;
			}
			default:
				break;
			}
		}
		glBindVertexArray(0);

	}




	glUseProgram(shader->ShaderProgram);


	glBindVertexArray(mVao);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	GLenum errorcode = glGetError();
	glBindVertexArray(0);

	Finish();
	return;

}

void Sapphire::MeshBuffer::Finish()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mVbo);
	glDeleteBuffers(1, &mEbo);
}

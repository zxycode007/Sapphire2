#pragma once
#include "stdafx.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>

typedef ShaderStruct;
typedef GLFWwindow * WindowHandle;

struct MeshStruct
{
	float*  vertices;
	uint*   indices;
	ulong   vertexBufferSize;
	ulong   indexBufferSize;
	ulong   offset;
	uint    index;
	uint    attribCount;

	MeshStruct() {};

};

class MeshBuffer
{
public:

	enum BufferMode
	{
		Static_Draw,
		Dynamic_Draw,
		Stream_Draw
	};
	
	MeshBuffer(MeshStruct* mesh, WindowHandle* Window, BufferMode mode = BufferMode::Static_Draw)
	{
		mVbo = 0;
		mEbo = 0;
		mVao = 0;
		mMesh = mesh;
		mWindow = Window;
		mVaoIndex = 0;
	};
	bool Initialize();
	void Release();
	void DrawMeshBuffer(ShaderStruct* shader);

	BufferMode getMode() { return mMode; };
	void       setMode(BufferMode mode) { mMode = mode; };
	GLuint     getVAOIndex() { return mVaoIndex; };
	GLuint     setVAOIndex(GLuint index) { mVaoIndex = index; };
	bool       isInit() { return mIsInit; }

private:

	void       Finish();

	GLuint  mVbo;
	GLuint  mVao;
	GLuint  mEbo;
	GLuint  mVaoIndex;
	MeshStruct* mMesh;
	BufferMode mMode;
	bool       mIsInit;
	WindowHandle* mWindow;

};

class SMeshNode
{
public:
	SMeshNode() 
	{
	};
	~SMeshNode() {};

	bool Load(MeshStruct* mesh, MeshBuffer::BufferMode mode = MeshBuffer::BufferMode::Static_Draw);
	bool Load(ShaderStruct* shader);
	ShaderStruct*  GetShader() { return mShader; };
	void Release();
	void DrawMesh();

	MeshBuffer* GetMeshBuffer() { return mMeshBuffer; };
	void        SetMeshBuffer(MeshBuffer* buffer) { mMeshBuffer = buffer; };
	WindowHandle*  GetWindow() { return mWindow; };
	void        SetWindow(WindowHandle* window) { mWindow = window; };

private:

	MeshBuffer* mMeshBuffer;
	WindowHandle* mWindow;
	ShaderStruct* mShader;


};

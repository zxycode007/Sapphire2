//#define GLEW_STATIC  //使用GLEW的静态库

#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <fstream>
#include <sstream>
#include "StringUtil.h"
#include "IOUtil.h"
#include "ShaderManager.h"
#include "Mesh.h"
#include "Line.h"
#include "VideoDriver.h"
#include "Vertex.h"

#pragma comment(lib,"opengl32.lib")

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void render();

GLFWwindow*  init();


int main(char* argc[], int argv)
{

	GLFWwindow* window = init();

	// 定义窗口大小
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);



	while (!glfwWindowShouldClose(window))
	{
		render();
		glfwSwapBuffers(window);
		//检查，如果事件被激活(按键，鼠标移动)调用合适的响应函数
		glfwPollEvents();
		// 交换缓冲区
	}
	//结束GLFW，清空GLFW的资源
	glfwTerminate();
	return 0;
}

GLFWwindow* init()
{
	//glfwInit来初始化GLFW，然后我们可以使用glfwWindowHint来配置GLFW。
	glfwInit();
	//glfwWindowHint的第一个参数告诉我们，配置哪个选项，这些选项带有GLFW_前缀。第二个参数是一个整数，它代表我们为选项所设置的值
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //告诉GLFW我们使用的OpenGL版本是3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //我们同样告诉GLFW，我们希望明确地使用core-profile
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//GLFW创建窗口函数
	GLFWwindow * window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	//创建我们窗口的环境（glfwMakeContextCurrent），这个环境是当前线程的主环境
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	// 设置需要的回调函数
	glfwSetKeyCallback(window, key_callback);

	//初始化GLEW
	//在初始化GLEW前我们把glewExperimental变量设置为GL_TRUE。(glew 1.13或更早的版本)
	//设置glewExperimental为true可以保证GLEW使用更多的现代技术来管理OpenGL机能。
	//如果不这么设置，它就会使用默认的GL_FALSE，这样当使用core profile的时有可能发生问题。
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) //glewInit()放在Opengl环境建立之后
	{
		//初始化失败
		std::cout << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
	return window;
}

void render()
{
	string vs_source = readTextFile("vs.glsl");
	string ps_source = readTextFile("ps.glsl");

	ShaderManager* shaderMgr = new ShaderManager();
	shaderMgr->CreateShaderProgram("shader1", vs_source.c_str(), ps_source.c_str());
	ShaderStruct* shader = shaderMgr->FindShader("shader1");
	shaderMgr->CompileAndLink(shader);
	shaderMgr->PrintLogs();

	//顶点集合
	/*
	GLfloat vertices[] = {

	-0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
	0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,
	0.5f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	*/
	GLfloat vertices[] = {

		0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 右上角
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // 左下角
		-0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 1.0f,// 左上角
	};
	GLuint indices[] = { // 起始于0!

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	MeshStruct* mesh = new MeshStruct();
	mesh->vertices = vertices;
	mesh->vertexBufferSize = sizeof(vertices);
	mesh->indices = indices;
	mesh->indexBufferSize = sizeof(indices);
	mesh->offset = 0;
	mesh->attribCount = 3;
	mesh->index = 0;
	SMeshNode*  smesh = new SMeshNode();
	smesh->Load(shader);
	smesh->Load(mesh);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	smesh->DrawMesh();

	Sapphire::Line3d lines(Sapphire::Vector3(0.5, -0.8, 1.0), Sapphire::Vector3(-0.5, -0.8, 1.0));
	Sapphire::Line3d lines2(Sapphire::Vector3(-0.2, -0.8, 1.0), Sapphire::Vector3(0.7, 0.8, 1.0));
	lines.setColor(Sapphire::Color(1.0, 0.0, 0.0, 1.0));
	lines2.setColor(Sapphire::Color(1.0, 1.0, 0.0, 1.0));
	OpenGLVideoDriver* vd = new OpenGLVideoDriver();
	vd->Load(shader);
	vd->drawLine(lines);
	vd->drawLine(lines2);
	delete vd;

}


//当按键时GLFW的回调响应
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
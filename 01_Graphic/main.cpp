//#define GLEW_STATIC  //使用GLEW的静态库

#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <fstream>
#include <sstream>

#pragma comment(lib,"opengl32.lib")

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main(char* argc[], int argv)
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
		return -1;
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
		return -1;
	}


	// 定义时刻大小
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	std::ifstream in("vertex.glsl");
	std::ostringstream tmp;
	tmp << in.rdbuf();
	std::string str = tmp.str();
	in.close();


	const GLchar* vs_source = str.c_str();
	in.open("fragment.glsl");
	tmp.str("");  //清空sstream用clear()没有用
	tmp << in.rdbuf();
	str = tmp.str();
	in.close();
	const GLchar* fs_source = str.c_str();

	//定义顶点着色器句柄
	GLuint vertexShader;
	//创建shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//添加shader源代码
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	//获取编译状态
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fs_source, NULL);
	glCompileShader(fragmentShader);

	//获取编译状态
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	//着色器程序对象（shader program object）是多个着色器最后链接的版本。
	//如果要使用刚才编译的着色器我们必须把它们链接为一个着色器程序对象，然后当渲染物体的时候激活这个着色器程序。激活了的着色器程序的着色器，在调用渲染函数时才可用。
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		GLenum errorcode = glGetError();
		std::cout << "SHADER::PROGRAME::LINK_FAILED\n" << infoLog << "  ERROR CODE: " << errorcode << std::endl;
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//顶点集合
	GLfloat vertices[] = {

		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};
	GLuint indices[] = { // 起始于0!

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	//vbo对象句柄
	GLuint VBO;
	//创建缓冲区
	glGenBuffers(1, &VBO);
	//创建顶点数组对象
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//绑定VAO
	glBindVertexArray(VAO);

	//绑定缓冲区
	//OpenGL有很多缓冲对象类型，GL_ARRAY_BUFFER是其中一个顶点缓冲对象的缓冲类型。
	//OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//为缓存区设置数据
	//任何缓冲函数（在GL_ARRAY_BUFFER目标上）都会用来配置当前绑定的缓冲（VBO）。
	//然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中
	//glBufferData是一个用来把用户定义数据复制到当前绑定缓冲的函数。
	//它的第一个参数是我们希望把数据复制到上面的缓冲类型：
	//顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
	//第二个参数指定我们希望传递给缓冲的数据的大小（字节）；用一个简单的sizeof计算出顶点数据就行。
	//第三个参数是我们希望发送的真实数据。
	/*
	第四个参数指定了我们希望显卡如何管理给定的数据。有三种形式：

	GL_STATIC_DRAW：数据不会或几乎不会改变。
	GL_DYNAMIC_DRAW：数据会被改变很多。
	GL_STREAM_DRAW：数据每次绘制时都会改变。
	三角形的位置数据不会改变，每次渲染调用时都保持原样，所以它使用的类型最好是GL_STATIC_DRAW。如果，比如，一个缓冲中的数据将频繁被改变，那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW。这样就能确保图形卡把数据放在高速写入的内存部分。
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//顶点属性location作为它的参数
	glEnableVertexAttribArray(0);

	// 1. 设置顶点属性指针,顶点数据流
	//第一个参数指定我们要配置哪一个顶点属性,在顶点着色器中使用layout(location = 0)定义了顶点属性——位置（position）的location
	//第二个参数指定顶点属性的大小。顶点属性是vec3类型，它由3个数值组成。
	//第三个参数指定数据的类型，这里是GL_FLOAT
	//第四个参数定义我们是否希望数据被标准化。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是 - 1）到1之间。
	//第五个参数叫做步长（stride），它告诉我们在连续的顶点属性之间间隔有多少 (要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）)
	//最后一个参数有古怪的GLvoid*的强制类型转换。它我们的位置数据在缓冲中起始位置的偏移量。由于位置数据是数组的开始，所以这里是0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);


	/*
	每个顶点属性从VBO管理的内存中获得它的数据，它所获取数据的那个VBO，
	就是当调用glVetexAttribPointer的时候，最近绑定到GL_ARRAY_BUFFER的那个VBO。
	由于在调用glVertexAttribPointer之前绑定了VBO，顶点属性0现在链接到了它的顶点数据。
	*/

	/*

	OpenGL core-profile版要求我们使用VAO，这样它就能知道对我们的顶点输入做些什么。如果我们绑定VAO失败，OpenGL会拒绝绘制任何东西。
	一个顶点数组对象储存下面的内容：

	调用glEnableVertexAttribArray和glDisableVertexAttribArray的。
	使用glVertexAttribPointer的顶点属性配置。
	使用glVertexAttribPointer进行的顶点缓冲对象与顶点属性链接

	*/
	// 渲染
	// 清空颜色缓冲区
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//用glUseProgram函数，用新创建的程序对象作为它的参数，这样就能激活这个程序对象：
	//现在在glUseProgram函数调用之后的每个着色器和渲染函数都会用到这个程序对象（当然还有这些链接的着色器）了。
	glUseProgram(shaderProgram);
	//绘制三角形
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//绑定索引缓冲区
	GLuint EBO;
	glGenBuffers(1, &EBO);
	//与VBO相似，我们绑定EBO然后用glBufferData把索引复制到缓冲里。同样，和VBO相似，我们会把这些函数调用放在绑定和解绑函数调用之间，这次我们把缓冲的类型定义为GL_ELEMENT_ARRAY_BUFFER。
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	////第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样
	//第二个参数是我们打算绘制顶点的次数。我们填6，说明我们总共想绘制6个顶点
	//第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
	//最后一个参数里我们可以指定EBO中的偏移量(或者传递一个索引数组，但是这只是当你不是在使用索引缓冲对象的时候)
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glfwSwapBuffers(window);
	//解除绑定
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{

		//检查，如果事件被激活(按键，鼠标移动)调用合适的响应函数
		glfwPollEvents();
		// 交换缓冲区

	}
	glDeleteBuffers(1, &VBO);

	//结束GLFW，清空GLFW的资源
	glfwTerminate();
	return 0;
}

//当按键时GLFW的回调响应
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
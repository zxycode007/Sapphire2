#version 330 core  //定义版本 openGL 3.3 core profile
 #pragma optimize (off)
 //输入的流变量要和vertex shader里输出的一致
in vec4 vertexColor;
//输出属性定义
out vec4 color;
 
void main()
{
    //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	color = vec4(vertexColor.r,vertexColor.g,vertexColor.b,vertexColor.a);
}
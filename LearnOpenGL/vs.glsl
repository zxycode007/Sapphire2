#version 330 core

layout (location = 0) in vec3 position;   //输入流位置 （位置0）
in vec4 color;                            //输入流颜色

out vec4 vertexColor;                     //输出流

void main()
{
   gl_Position = vec4(position.xyz,1.0);
   vertexColor = color;
}
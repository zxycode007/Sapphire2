#version 330 core   //定义版本 openGL 3.3 core profile

//输入顶点属性: 定义布局(顶点属性index)  类型  输入变量
layout (location=0) in vec3 position;

void main()
{
    //gl_Position预定义的类型
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
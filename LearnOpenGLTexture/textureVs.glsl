#version 420 core   //定义版本 openGL 3.3 core profile
 #pragma optimize (off)
//输入顶点属性: 定义布局(顶点属性index)  类型  输入变量
layout (location=0) in vec3 position;
layout (location=1) in vec4 vColor;
layout (location=2) in vec3 vNormal;
layout (location=3) in vec2 texTcoord1;

out vec4 vertexColor;
out vec2 tcoord;

void main()
{
    //gl_Position预定义的类型
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
	vertexColor = vColor;
	tcoord = vec2(texTcoord1.x, texTcoord1.y);
}
#version 420 core  //定义版本 openGL 3.3 core profile
 #pragma optimize (off)
 //输入的流变量要和vertex shader里输出的一致
in vec4 vertexColor;
in vec2 tcoord;
//输出属性定义
out vec4 color;

uniform sampler2D texture1;
 
void main()
{
    //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	color = texture(texture1, tcoord);
}
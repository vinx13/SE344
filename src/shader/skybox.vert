#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 view;
out vec3 texCoords;

void main()
{
	texCoords = aPos;
	gl_Position = (view*vec4(aPos.xyz, 1.0)).xyww;
}

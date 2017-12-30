#version 330 core
 layout (location = 0) in vec3 aPos;
 layout (location = 1) in vec3 offset;
 uniform mat4 mvp;
 void main() {
     gl_Position = mvp * vec4(aPos.xyz + offset.xyz, 1.0);
     gl_PointSize = 5.0;
 }

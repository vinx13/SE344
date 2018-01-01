#version 330 core
 layout (location = 0) in vec3 aPos;
 uniform mat4 mvp;
 void main() {
     gl_Position = mvp * vec4(aPos.xyz, 1.0);
     gl_PointSize = 5.0;
 }

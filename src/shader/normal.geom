#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 8) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 1.0;
out vec4 vertex_color;

void GenerateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    vertex_color = vec4(0, 0, 1, 1);
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    vertex_color = vec4(0, 0, 1, 1);
    EmitVertex();
    EndPrimitive();

}

void main()
{
    GenerateLine(0); // 第一个顶点法线
    GenerateLine(1); // 第二个顶点法线
    GenerateLine(2); // 第三个顶点法线


    vec3 P0 = gl_in[0].gl_Position.xyz;
    vec3 P1 = gl_in[1].gl_Position.xyz;
    vec3 P2 = gl_in[2].gl_Position.xyz;

    vec3 V0 = P0 - P1;
    vec3 V1 = P2 - P1;

    vec3 N = normalize(cross(V1, V0));

    // Center of the triangle
    vec3 P = (P0+P1+P2) / 3.0;

    gl_Position = vec4(P, 1.0);
    vertex_color = vec4(1, 0, 0, 1);
    EmitVertex();

    gl_Position = vec4(P + N * MAGNITUDE, 1.0);
    vertex_color = vec4(1, 0, 0, 1);
    EmitVertex();
    EndPrimitive();
}
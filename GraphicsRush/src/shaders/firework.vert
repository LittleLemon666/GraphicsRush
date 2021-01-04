#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 u_model;

layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_projection;
    mat4 u_view;
};

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
}
#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 u_model;

layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_projection;
    mat4 u_view;
};

uniform int rain_time;
uniform int rain_step;
uniform int height;

void main()
{
    vec3 bias = vec3(0.0, -(rain_step + rain_time) % height, 0.0);
    gl_Position = u_projection * u_view * u_model * vec4(position + bias, 1.0f);
}
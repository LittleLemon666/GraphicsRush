#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 u_model;

layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_projection;
    mat4 u_view;
};

uniform int star_time;
uniform float star_angle;
uniform float height;
uniform float r;

void main()
{
    float rotate_angle = star_time / 6.0 + star_angle;
    float r_time = r + r * ((star_time * 15 % 360) / 360.0 - 0.5);
    vec3 bias = vec3(r_time * cos(radians(rotate_angle)), height * 2 * (star_time * 15 % 360) / 360.0, -r_time * sin(radians(rotate_angle)));
    gl_Position = u_projection * u_view * u_model * vec4(position + bias, 1.0f);
}
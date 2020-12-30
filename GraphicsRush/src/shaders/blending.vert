#version 430 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texture_coordinate;

uniform mat4 u_model;

layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_projection;
    mat4 u_view;
};

out V_OUT
{
   vec2 texture_coordinate;
} v_out;

void main()
{
    v_out.texture_coordinate = vec2(texture_coordinate.x, 1.0f - texture_coordinate.y);
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);
}
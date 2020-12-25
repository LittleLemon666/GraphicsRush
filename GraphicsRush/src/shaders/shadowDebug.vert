#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinate;

out V_OUT
{
   vec2 texture_coordinate;
} v_out;

void main()
{
    v_out.texture_coordinate = texture_coordinate;
    gl_Position = vec4(position, 1.0);
}
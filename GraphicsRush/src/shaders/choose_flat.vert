#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 u_model;

out V_OUT
{
   vec3 position;
} v_out;

void main()
{
    gl_Position = u_model * vec4(position, 1.0f);

    v_out.position = vec3(u_model * vec4(position, 1.0f));
}
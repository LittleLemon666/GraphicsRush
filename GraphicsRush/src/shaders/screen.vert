#version 430 core
layout (location = 0) in vec3 position;

out V_OUT
{
   vec2 uv;
} v_out;

void main()
{
    v_out.uv = position.xy * 0.5 + 0.5;

    gl_Position = vec4(position, 1.0f);
}
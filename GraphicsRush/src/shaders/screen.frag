#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 uv;
} f_in;

uniform sampler2D screen;

void main()
{
    f_color = texture(screen, f_in.uv);
}
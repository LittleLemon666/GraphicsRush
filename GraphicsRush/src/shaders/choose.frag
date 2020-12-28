#version 430 core
out vec4 f_color;

in V_OUT
{
   vec3 position;
} f_in;

uniform int chooser;

void main()
{   
    f_color = vec4(chooser / 100 / 10.0f, chooser / 10 % 10 / 10.0f, chooser % 10 / 10.0f, 1.0f);
}
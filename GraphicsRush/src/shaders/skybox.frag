#version 430 core

#extension GL_NV_shadow_samplers_cube : enable

out vec4 f_color;

in V_OUT
{
   vec3 texture_coordinate;
} f_in;

uniform samplerCube skybox;

void main()
{      
    f_color = texture(skybox, f_in.texture_coordinate);
}
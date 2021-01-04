#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 texture_coordinate;
} f_in;

uniform sampler2D u_texture;

void main()
{    
    vec4 color = vec4(texture(u_texture, f_in.texture_coordinate));
    float gray = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
    f_color = vec4(gray, gray, gray, color.a);
}
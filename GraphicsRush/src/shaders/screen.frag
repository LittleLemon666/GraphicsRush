#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 uv;
} f_in;

uniform sampler2D screen;
uniform float brightness;
uniform int filter_id;
uniform vec2 size;

float cartoonRG(float x)
{
    float neer_color[] = { 0, 36 / 256.0, 73 / 256.0, 109 / 256.0, 146 / 256.0, 182 / 256.0, 219 / 256.0, 1.0};
    int neer_color_num = 8;
    for (int neer_color_index = 0; neer_color_index < neer_color_num - 1; neer_color_index++)
    {
        if (neer_color[neer_color_index] <= x && x < neer_color[neer_color_index + 1])
        {
            if (x - neer_color[neer_color_index] < neer_color[neer_color_index + 1] - x)
            {
                return neer_color[neer_color_index];
            }
            else
            {
                return neer_color[neer_color_index + 1];
            }
        }
    }
    return neer_color[neer_color_num - 1];
}

float cartoonB(float x)
{
    float neer_color[] = { 0, 85 / 256.0, 170 / 256.0, 1.0 };
    int neer_color_num = 4;
    for (int neer_color_index = 0; neer_color_index < neer_color_num - 1; neer_color_index++)
    {
        if (neer_color[neer_color_index] <= x && x < neer_color[neer_color_index + 1])
        {
            if (x - neer_color[neer_color_index] < neer_color[neer_color_index + 1] - x)
            {
                return neer_color[neer_color_index];
            }
            else
            {
                return neer_color[neer_color_index + 1];
            }
        }
    }
    return neer_color[neer_color_num - 1];
}

vec3 gray(vec3 color)
{
    return vec3(color.r * 0.3 + color.g * 0.59 + color.b * 0.11);
}

vec4 edge()
{
    float dx = 1.0 / size.x;
    float dy = 1.0 / size.y;
    vec3 Ix = (gray(texture(screen, vec2(f_in.uv.x - dx, f_in.uv.y - dy)).rgb) +
        gray(texture(screen, vec2(f_in.uv.x, f_in.uv.y - dy)).rgb) +
        gray(texture(screen, vec2(f_in.uv.x + dx, f_in.uv.y - dy)).rgb) -
        gray(texture(screen, vec2(f_in.uv.x - dx, f_in.uv.y + dy)).rgb) -
        gray(texture(screen, vec2(f_in.uv.x, f_in.uv.y + dy)).rgb) -
        gray(texture(screen, vec2(f_in.uv.x + dx, f_in.uv.y + dy)).rgb)) / 6.0;
    vec3 Iy = (gray(texture(screen, vec2(f_in.uv.x - dx, f_in.uv.y - dy)).rgb) +
        gray(texture(screen, vec2(f_in.uv.x - dx, f_in.uv.y)).rgb) +
        gray(texture(screen, vec2(f_in.uv.x - dx, f_in.uv.y + dy)).rgb) -
        gray(texture(screen, vec2(f_in.uv.x + dx, f_in.uv.y - dy)).rgb) -
        gray(texture(screen, vec2(f_in.uv.x + dx, f_in.uv.y)).rgb) -
        gray(texture(screen, vec2(f_in.uv.x + dx, f_in.uv.y + dy)).rgb)) / 6.0;
    vec3 color = (Ix + Iy) * 16.0;
    if (color.b < 0.5)  color = vec3(0.3,0.4,1.0);
    return vec4(color, 1.0);
}

void main()
{
    vec4 color = texture(screen, f_in.uv);
    if (filter_id == 1)
        color = vec4(cartoonRG(color.r), cartoonRG(color.g), cartoonB(color.b), 1.0);
    else if (filter_id == 2)
    {
        if (f_in.uv.x < 0.4 || f_in.uv.x > 0.6)
            color = color * vec4(0.3,0.3,0.3,1.0);
    }
    else if (filter_id == 3)
        color = edge();

    f_color = color * brightness;
}
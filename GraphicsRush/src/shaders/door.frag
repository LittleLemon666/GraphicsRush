#version 430 core
out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} f_in;

uniform sampler2D door_scene_texture;
uniform sampler2D door_texture;
uniform float offset;

bool isGreen(vec3 color)
{
    return color.g > 0.35 && color.r < 0.3 && color.b < 0.3;
}

void main()
{   
    vec3 door_scene_color = vec3(texture(door_scene_texture, f_in.texture_coordinate));
    vec3 door_scene_out_color = vec3(texture(door_scene_texture, vec2(f_in.texture_coordinate.x - offset, f_in.texture_coordinate.y)));
    vec3 door_color = vec3(texture(door_texture, vec2(f_in.texture_coordinate.x - offset, f_in.texture_coordinate.y)));
    vec3 color = door_scene_color;
    if (isGreen(door_scene_color))
    {
        if (!isGreen(door_scene_out_color))
            discard;
        color = door_color;
    }
    f_color = vec4(color, 1.0f);
}
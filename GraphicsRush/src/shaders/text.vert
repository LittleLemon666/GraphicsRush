#version 430 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out V_OUT
{
   vec2 texture_coordinate;
} v_out;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    v_out.texture_coordinate = vertex.zw;
}
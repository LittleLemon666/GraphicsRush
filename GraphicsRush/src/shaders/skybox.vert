#version 430 core
layout (location = 0) in vec3 position;

layout (std140, binding = 1) uniform skybox_matrices
{
    mat4 u_projection;
    mat4 u_view;
};

out V_OUT
{
   vec3 texture_coordinate;
} v_out;

void main()
{
    v_out.texture_coordinate = position;
    vec4 pos = u_projection * u_view * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
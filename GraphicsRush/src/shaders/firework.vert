#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 u_model;

layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_projection;
    mat4 u_view;
};

out V_OUT
{
   vec3 position;
   vec3 center;
} v_out;

uniform float time;
uniform vec3 center;

void main()
{
    float time_d = min(time, 10.0); // range [0, 50.0) for up, range [50.0, 100.0) for bomb
    vec3 bias = vec3(0.0, time_d, 0.0);
    gl_Position = u_projection * u_view * u_model * vec4(position + bias, 1.0f);

    v_out.position = vec3(u_model * vec4(position + bias, 1.0f));
    v_out.center = vec3(u_model * vec4(center + bias, 1.0f));
}
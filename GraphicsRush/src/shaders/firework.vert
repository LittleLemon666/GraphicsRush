#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;

uniform mat4 u_model;

layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_projection;
    mat4 u_view;
};

uniform mat4 lightSpaceMatrix;

out V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
   vec4 frag_pos_light_space;
} v_out;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);

    v_out.position = vec3(u_model * vec4(position, 1.0f));
    v_out.normal = mat3(transpose(inverse(u_model))) * normal;
    v_out.texture_coordinate = vec2(texture_coordinate.x, 1.0 - texture_coordinate.y);
    v_out.frag_pos_light_space = lightSpaceMatrix * vec4(v_out.position, 1.0);
}
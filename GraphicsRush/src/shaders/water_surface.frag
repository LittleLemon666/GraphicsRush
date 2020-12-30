#version 430 core

#extension GL_NV_shadow_samplers_cube : enable

out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
} f_in;

uniform samplerCube skybox;

layout (std140, binding = 2) uniform Viewer
{
    vec3 view_pos;     //16
}viewer;

uniform float Eta = 1 / 1.33;

void main()
{
    vec3 InVec = normalize(f_in.position - viewer.view_pos); 
    //湖面（各頂點）到眼睛的入射向量
 
    vec3 ReflectVec = reflect(InVec, f_in.normal);
    vec3 RefractVec;
    float cosTheta = dot(InVec, f_in.normal) / length(InVec) / length(f_in.normal);
    if (cosTheta > 0)
        RefractVec = refract(InVec, -f_in.normal, 1.33); //Eta，折射率，1.33
    else
        RefractVec = refract(InVec, f_in.normal, Eta); //Eta，折射率，1 / 1.33

    vec3 ReflectColor = vec3(textureCube(skybox, ReflectVec));
    vec3 RefractColor = vec3(textureCube(skybox, RefractVec));

    float ratio = 0.4 + 0.6 * pow(min(0.0, 1.0 - dot(-InVec, f_in.normal)), 4.0); //float fresnel 

    vec3 color = mix(RefractColor, ReflectColor, ratio);
    
    f_color =  vec4(color, 1.0);
}
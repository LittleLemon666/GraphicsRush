#version 430 core
out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} f_in;

uniform vec3 u_color;

uniform sampler2D u_texture;

layout (std140, binding = 2) uniform Dir_light
{
    float shininess;    //16
    vec3 direction;     //16
    vec3 ambient;       //16
    vec3 diffuse;       //16
    vec3 specular;      //16

    vec3 viewPos;       //16
}dir_light;

vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dir_light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), dir_light.shininess);
    // combine results
    vec3 ambient  = dir_light.ambient  * vec3(texture(u_texture, f_in.texture_coordinate));
    vec3 diffuse  = dir_light.diffuse  * diff * vec3(texture(u_texture, f_in.texture_coordinate));
    vec3 specular = dir_light.specular * spec * vec3(texture(u_texture, f_in.texture_coordinate));
    return (ambient + diffuse + specular);
}

void main()
{   
    vec3 viewDir = normalize(dir_light.viewPos - f_in.position);
    vec3 color = CalcDirLight(f_in.normal, viewDir);
    //vec3 color = vec3(texture(u_texture, f_in.texture_coordinate));
    f_color = vec4(color, 1.0f);
}
#version 430 core
out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
   vec4 frag_pos_light_space;
} f_in;

uniform vec3 u_color;

uniform sampler2D u_texture;
uniform sampler2D shadowMap;

layout (std140, binding = 2) uniform Viewer
{
    vec3 view_pos;     //16
}viewer;

layout (std140, binding = 3) uniform Dir_light
{
    float shininess;    //16
    vec3 direction;     //16
    vec3 ambient;       //16
    vec3 diffuse;       //16
    vec3 specular;      //16
}dir_light;

//vec3 CalcDirLight(vec3 normal, vec3 viewDir)
//{
//    vec3 color = vec3(texture(u_texture, f_in.texture_coordinate));
//
//    vec3 lightDir = normalize(-dir_light.direction);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), dir_light.shininess);
//    // combine results
//    vec3 ambient  = dir_light.ambient  * vec3(texture(u_texture, f_in.texture_coordinate));
//    vec3 diffuse  = dir_light.diffuse  * diff;
//    vec3 specular = dir_light.specular * spec;
//    return (ambient + diffuse + specular) * color;
//}

vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
    vec3 color = texture(u_texture, f_in.texture_coordinate).rgb;
    normal = normalize(normal);
    vec3 lightColor = vec3(0.7);
    // ambient
    vec3 ambient = 0.7 * color;
    // diffuse
    vec3 lightDir = normalize(dir_light.direction);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow                    
    return (ambient + diffuse + specular) * color;
}

//float ShadowCalculation(vec4 fragPosLightSpace)
//{
//    // perform perspective divide
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(shadowMap, projCoords.xy).r; 
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//    // calculate bias (based on depth map resolution and slope)
//    vec3 normal = normalize(f_in.normal);
//    float bias = max(0.05 * (1.0 - dot(normal, dir_light.direction)), 0.005);
//    // check whether current frag pos is in shadow
//    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
//    // PCF
//    float shadow = 0.0;
//    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
//    for(int x = -1; x <= 1; ++x)
//    {
//        for(int y = -1; y <= 1; ++y)
//        {
//            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
//            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
//        }    
//    }
//    shadow /= 9.0;
//    
//    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
//    if(projCoords.z > 1.0)
//        shadow = 0.0;
//        
//    return shadow;
//}

void main()
{   
    vec3 viewDir = normalize(viewer.view_pos - f_in.position);
//    float shadow = ShadowCalculation(f_in.frag_pos_light_space);
//
//    vec3 color = texture(u_texture, f_in.texture_coordinate).rgb;
//    vec3 normal = normalize(f_in.normal);
//    vec3 lightColor = vec3(0.7);
//    // ambient
//    vec3 ambient = 0.7 * color;
//    // diffuse
//    vec3 lightDir = normalize(dir_light.direction);
//    float diff = max(dot(lightDir, normal), 0.0);
//    vec3 diffuse = diff * lightColor;
//    // specular
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = 0.0;
//    vec3 halfwayDir = normalize(lightDir + viewDir);  
//    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
//    vec3 specular = spec * lightColor;    
//    // calculate shadow                    
//    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
//    f_color = vec4(lighting, 1.0f);

    vec3 color = CalcDirLight(f_in.normal, viewDir);
    f_color = vec4(color, 1.0f);
}
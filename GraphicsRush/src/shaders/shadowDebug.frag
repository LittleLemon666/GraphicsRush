#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 texture_coordinate;
} f_in;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;
uniform float offset;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{
    if (f_in.texture_coordinate.x < offset)
    {
        float depthValue = texture(depthMap, f_in.texture_coordinate).r;
        // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
        f_color = vec4(vec3(depthValue), 1.0); // orthographic        
    }
    else discard;
}
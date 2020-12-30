#version 430 core

#extension GL_NV_shadow_samplers_cube : enable

out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
} f_in;

uniform samplerCube environment_box;

layout (std140, binding = 2) uniform Viewer
{
    vec3 view_pos;     //16
}viewer;

//uniform vec3 environmentCenter;
//uniform vec3 bboxMax;
//uniform vec3 bboxMin;
//vec3 LocalCorrect(vec3 origVec, vec3 vertexPos)
//{
//    //*********************************************************************
//	// Author: Roberto Lopez Mendez
//	// URL: https://community.arm.com/developer/tools-software/graphics/b/blog/posts/reflections-based-on-local-cubemaps-in-unity
//	//*********************************************************************
//
//    vec3 invOrigVec = vec3(1.0,1.0,1.0)/origVec;
//
//    vec3 intersecAtMaxPlane = (bboxMax - vertexPos) * invOrigVec;
//
//    vec3 intersecAtMinPlane = (bboxMin - vertexPos) * invOrigVec;
//
//    // Get the largest intersection values (we are not intersted in negative values)
//
//    vec3 largestIntersec = max(intersecAtMaxPlane, intersecAtMinPlane);
//
//    // Get the closest of all solutions
//
//   float Distance = min(min(largestIntersec.x, largestIntersec.y), largestIntersec.z);
//
//    // Get the intersection position
//
//    vec3 IntersectPositionWS = vertexPos + origVec * Distance;
//
//    // Get corrected vector
//
//    vec3 localCorrectedVec = IntersectPositionWS - environmentCenter;
//
//    return localCorrectedVec;
//}

void main()
{   
//    vec3 InVec = normalize(f_in.position - viewer.view_pos); 
//    //湖面（各頂點）到眼睛的入射向量
// 
//    vec3 ReflectVec = reflect(InVec, f_in.normal);
//    vec3 RefractVec;
//    float cosTheta = dot(InVec, f_in.normal) / length(InVec) / length(f_in.normal);
//    if (cosTheta > 0)
//        RefractVec = refract(InVec, -f_in.normal, 1.33); //Eta，折射率，1.33
//    else
//        RefractVec = refract(InVec, f_in.normal, 1 / 1.33); //Eta，折射率，1 / 1.33
//    //vec3 newReflectVec = LocalCorrect(ReflectVec, f_in.position);
//    //vec3 newRefractVec = LocalCorrect(RefractVec, f_in.position);
//    vec3 ReflectColor = vec3(textureCube(environment_box, ReflectVec));
//    vec3 RefractColor = vec3(textureCube(environment_box, RefractVec));
//
//    float ratio = 0.4 + 0.6 * pow(min(0.0, 1.0 - dot(-InVec, f_in.normal)), 4.0); //float fresnel 

    //vec3 color = mix(RefractColor, ReflectColor, ratio);
    //vec3 color = ReflectColor;
    vec3 color = vec3(textureCube(environment_box, f_in.normal));

    f_color = vec4(color, 1.0f);
}
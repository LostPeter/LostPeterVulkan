/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#version 450

//TransformConstants
struct TransformConstants
{
    mat4 mat4View;
    mat4 mat4View_Inv;
    mat4 mat4Proj;
    mat4 mat4Proj_Inv;
    mat4 mat4ViewProj;
    mat4 mat4ViewProj_Inv;
};
//CameraConstants
struct CameraConstants
{
    vec3 posEyeWorld;    
    float fNearZ;
    float fFarZ;
    float fReserved1;
    float fReserved2;
    float fReserved3;
};
//LightConstants
#define MAX_LIGHT_COUNT 16
struct LightConstants
{
    vec4 com;      // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
    vec3 position;    // directional/point/spot
    float falloffStart; // point/spot light only
    vec3 direction;   // directional/spot light only
    float falloffEnd;   // point/spot light only
    vec4 ambient;     // ambient
    vec4 diffuse;     // diffuse
    vec4 specular;    // specular

    float4x4 depthMVP;  // depthMVP
};
//PassConstants
layout(binding = 0) uniform PassConstants 
{
    //TransformConstants
    TransformConstants g_Transforms[2]; //0: Eye Left(Main); 1: Eye Right
    //CameraConstants
    CameraConstants g_Cameras[2]; //0: Eye Left(Main); 1: Eye Right
    
    //TimeConstants
    float g_TotalTime;
    float g_DeltaTime;
    float g_Pad1;
    float g_Pad2;

    //RenderTarget
    vec2 g_RenderTargetSize;
    vec2 g_RenderTargetSize_Inv;

    //Material
    vec4 g_AmbientLight;
    
    //Light
    LightConstants g_MainLight;
    LightConstants g_AdditionalLights[MAX_LIGHT_COUNT];
} passConsts;

layout(binding = 1) uniform ObjectConstants
{
    mat4 g_MatWorld;
} objectConsts;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main()
{
    gl_Position = passConsts.g_Transforms[0].mat4Proj * passConsts.g_Transforms[0].mat4View * objectConsts.g_MatWorld * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}
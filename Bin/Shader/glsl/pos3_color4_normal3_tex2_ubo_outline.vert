//author: LostPeter
//time:   2022-12-17

#version 450

#define MAX_LIGHT_COUNT 16
struct Light
{
    vec3 position;      // point light only
    float falloffStart; // point/spot light only
    vec3 direction;     // directional/spot light only
    float falloffEnd;   // point/spot light only
    vec3 strength;      // directional/point/spot
    float spotPower;    // spot light only
};

layout(binding = 0) uniform PassConstants 
{
    mat4 g_MatView;
    mat4 g_MatView_Inv;
    mat4 g_MatProj;
    mat4 g_MatProj_Inv;
    mat4 g_MatViewProj;
    mat4 g_MatViewProj_Inv;
    vec3 g_EyePosW;
    float g_cbPerObjectPad1;
    vec2 g_RenderTargetSize;
    vec2 g_RenderTargetSize_Inv;
    float g_NearZ;
    float g_FarZ;
    float g_TotalTime;
    float g_DeltaTime;
    vec4 g_AmbientLight;
    Light g_Lights[MAX_LIGHT_COUNT];
} passConsts;

layout(binding = 1) uniform ObjectConstants
{
    mat4 g_MatWorld;
    vec4 outlineColor;
    float outlineWidth;
} objectConsts;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec4 fragColor;

void main()
{
    vec4 pos = vec4(inPosition.xyz + inNormal * objectConsts.outlineWidth, 1.0);
    gl_Position = passConsts.g_MatProj * passConsts.g_MatView * objectConsts.g_MatWorld * pos;
    fragColor = objectConsts.outlineColor;
}
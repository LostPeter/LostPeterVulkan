/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-02-14
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../hlsl_input.hlsl"
#include "../hlsl_common.hlsl"
#include "../hlsl_lighting_lambert.hlsl"


[[vk::binding(0)]]cbuffer passConsts                : register(b0) 
{
    PassConstants passConsts;
}


[[vk::binding(1)]]cbuffer objectConsts              : register(b1) 
{
    ObjectConstants objectConsts[MAX_OBJECT_COUNT];
}


[[vk::binding(2)]]cbuffer materialConsts            : register(b2) 
{
    MaterialConstants materialConsts[MAX_MATERIAL_COUNT];
}


[[vk::binding(3)]]cbuffer instanceConsts            : register(b3) 
{
    InstanceConstants instanceConsts[MAX_INSTANCE_COUNT];
}


[[vk::binding(4)]] Texture2D texDiffuse             : register(t1);
[[vk::binding(4)]] SamplerState texDiffuseSampler   : register(s1);

[[vk::binding(5)]] Texture2D texParallaxMap           : register(t2);
[[vk::binding(5)]] SamplerState texParallaxMapSampler : register(s2);



float3 unpackNormalXYZ(float2 uv)
{
    float3 tangentNormal = texParallaxMap.SampleLevel(texParallaxMapSampler, uv, 0.0).rgb;
    return normalize(tangentNormal * 2.0 - 1.0);
}
float3 unpackNormalXY(float2 uv)
{
    float2 packedNormal = texParallaxMap.SampleLevel(texParallaxMapSampler, uv, 0.0).rg;
    float3 normal;
    normal.xy = packedNormal.xy * 2 - 1;
    normal.z = sqrt(1 - saturate(dot(normal.xy, normal.xy)));
    return normal;
}

float3 calculateNormal(VSInput_Pos4Color4Normal3Tangent3TexCood2TSPos3TSEyePos3 input, float2 uv)
{
    float3 tangentNormal = unpackNormalXYZ(uv);

    float3 N = normalize(input.inWorldNormal);
    float3 T = normalize(input.inWorldTangent);
    float3 B = normalize(cross(N, T));
    float3x3 TBN = transpose(float3x3(T, B, N));

    return normalize(mul(TBN, tangentNormal));
}

float2 commonParallaxMapping(VSInput_Pos4Color4Normal3Tangent3TexCood2TSPos3TSEyePos3 input, 
                             float2 uv,
                             float3 viewDirTS,
                             float heightScale,
                             float parallaxBias)
{
	float height = 1.0 - texParallaxMap.SampleLevel(texParallaxMapSampler, uv, 0.0).a;
	float2 p = viewDirTS.xy * (height * (heightScale * 0.5) + parallaxBias) / viewDirTS.z;
	return uv - p;
}

float2 steepParallaxMapping(VSInput_Pos4Color4Normal3Tangent3TexCood2TSPos3TSEyePos3 input, 
                            float2 uv, 
                            float3 viewDirTS,
                            float heightScale,
                            float numLayers)
{
	float layerDepth = 1.0 / numLayers;
	float currLayerDepth = 0.0;
	float2 deltaUV = viewDirTS.xy * heightScale / (viewDirTS.z * numLayers);
	float2 currUV = uv;
	float height = 1.0 - texParallaxMap.SampleLevel(texParallaxMapSampler, currUV, 0.0).a;
	for (int i = 0; i < numLayers; i++) 
    {
		currLayerDepth += layerDepth;
		currUV -= deltaUV;
		height = 1.0 - texParallaxMap.SampleLevel(texParallaxMapSampler, currUV, 0.0).a;
		if (height < currLayerDepth) 
        {
			break;
		}
	}
	return currUV;
}

float2 occlusionParallaxMapping(VSInput_Pos4Color4Normal3Tangent3TexCood2TSPos3TSEyePos3 input, 
                                float2 uv, 
                                float3 viewDirTS,
                                float heightScale,
                                float numLayers)
{
	float layerDepth = 1.0 / numLayers;
	float currLayerDepth = 0.0;
	float2 deltaUV = viewDirTS.xy * heightScale / (viewDirTS.z * numLayers);
	float2 currUV = uv;
	float height = 1.0 - texParallaxMap.SampleLevel(texParallaxMapSampler, currUV, 0.0).a;
	for (int i = 0; i < numLayers; i++) 
    {
		currLayerDepth += layerDepth;
		currUV -= deltaUV;
		height = 1.0 - texParallaxMap.SampleLevel(texParallaxMapSampler, currUV, 0.0).a;
		if (height < currLayerDepth) 
        {
			break;
		}
	}
	float2 prevUV = currUV + deltaUV;
	float nextDepth = height - currLayerDepth;
	float prevDepth = 1.0 - texParallaxMap.SampleLevel(texParallaxMapSampler, prevUV, 0.0).a - currLayerDepth + layerDepth;
	return lerp(currUV, prevUV, nextDepth / (nextDepth - prevDepth));
}


float4 main(VSInput_Pos4Color4Normal3Tangent3TexCood2TSPos3TSEyePos3 input, 
            uint viewIndex : SV_ViewID) : SV_TARGET
{
    float3 outColor;

    CameraConstants cam = passConsts.g_Cameras[viewIndex];
    MaterialConstants mat = materialConsts[floor(input.inWorldPos.w + 0.5)];
    float3 V = normalize(cam.posEyeWorld - input.inWorldPos.xyz);
    float3 VT = normalize(input.inTSEyePos - input.inTSPos);
    float3 N = float3(0,0,1);
    float parallaxMapFlag = mat.aTexLayers[1].indexTextureArray;  
    float2 uv = input.inTexCoord;
    if (parallaxMapFlag == 1 ||
        parallaxMapFlag == 2)
    {
        N = calculateNormal(input, uv);
    }
    else if (parallaxMapFlag == 3)
    {
        float heightScale = mat.aTexLayers[1].texSpeedU;
        float parallaxBias = mat.aTexLayers[1].texSpeedV;
        uv = commonParallaxMapping(input, 
                                   uv, 
                                   VT, 
                                   heightScale, 
                                   parallaxBias);
        N = calculateNormal(input, uv);
    }
    else if (parallaxMapFlag == 4)
    {
        float heightScale = mat.aTexLayers[1].texSpeedU;
        float numLayers = mat.aTexLayers[1].texSpeedW;
        uv = steepParallaxMapping(input, 
                                  uv, 
                                  VT, 
                                  heightScale, 
                                  numLayers);
        N = calculateNormal(input, uv);
    }
    else if (parallaxMapFlag == 5)
    {
        float heightScale = mat.aTexLayers[1].texSpeedU;
        float numLayers = mat.aTexLayers[1].texSpeedW;
        uv = occlusionParallaxMapping(input, 
                                      uv, 
                                      VT, 
                                      heightScale, 
                                      numLayers);
        N = calculateNormal(input, uv);
    }
    else
    {
        N = normalize(input.inWorldNormal);
    }

    // Discard fragments at texture border
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) 
    {
        clip(-1);
    }
    
    float3 colorLight;
    //Main Light
    float3 colorMainLight = calculate_Light(passConsts.g_AmbientLight.rgb,
                                            passConsts.g_MainLight,
                                            mat,
                                            input.inWorldPos.xyz,
                                            cam.posEyeWorld, 
                                            N);
    colorLight = colorMainLight;

    //Additional Light



    //Texture
    float3 colorTexture = texDiffuse.Sample(texDiffuseSampler, uv).rgb;
    //VertexColor
    float3 colorVertex = input.inColor.rgb;

    //Final Color
    if (parallaxMapFlag == 0)
    {
        outColor = colorTexture;
    }
    else if (parallaxMapFlag == 1)
    {
        outColor = N;
    }
    else
    {
        outColor = colorLight * colorTexture * colorVertex;
    }

    return float4(outColor, 1.0);
}
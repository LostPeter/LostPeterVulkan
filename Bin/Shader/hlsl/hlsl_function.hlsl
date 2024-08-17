/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-08-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _HLSL_FUNCTION_
#define _HLSL_FUNCTION_


///////////////////////////////// NormalMap //////////////////////////////////////////////
float3 Func_UnpackNormalRGB(float4 packedNormal, float scale = 1.0)
{
    float3 normal;
    normal.xyz = packedNormal.rgb * 2.0 - 1.0;
    normal.xy *= scale;
    return normalize(normal);
}

float3 Func_UnpackNormalXYZ(Texture2D texNormalMap,
                            SamplerState texNormalMapSampler,
                            float2 uv)
{
    float3 packedNormal = texNormalMap.Sample(texNormalMapSampler, uv).rgb;
    return normalize(packedNormal * 2.0 - 1.0);
}
float3 Func_UnpackNormalXY(Texture2D texNormalMap,
                           SamplerState texNormalMapSampler,
                           float2 uv)
{
    float2 packedNormal = texNormalMap.Sample(texNormalMapSampler, uv).rg;
    float3 normal;
    normal.xy = packedNormal.xy * 2 - 1;
    normal.z = sqrt(1 - saturate(dot(normal.xy, normal.xy)));
    return normal;
}


float3 Func_CalculateNormal(Texture2D texNormalMap,
                            SamplerState texNormalMapSampler,
                            float2 inTexCoord,
                            float3 inWorldNormal,
                            float3 inWorldTangent)
{
    float3 normal = Func_UnpackNormalXYZ(texNormalMap,
                                         texNormalMapSampler,
                                         inTexCoord);

    float3 N = normalize(inWorldNormal);
    float3 T = normalize(inWorldTangent);
    float3 B = normalize(cross(N, T));
    float3x3 TBN = transpose(float3x3(T, B, N));

    return normalize(mul(TBN, normal));
}


///////////////////////////////// ParallaxMap ////////////////////////////////////////////




///////////////////////////////// Linear - SRGB //////////////////////////////////////////
float4 Func_ProcessLinear(float4 pixel)
{
	return float4(pow(abs(pixel.xyz), 1.0 / 2.2), pixel.w);
}

float4 Func_LinearToSRGB(float4 pixel)
{
	float3 sRGBLo = pixel.xyz * 12.92;
	float3 sRGBHi = (pow(abs(pixel.xyz), float3(1.0 / 2.4, 1.0 / 2.4, 1.0 / 2.4)) * 1.055) - 0.055;
	float3 sRGB = select((pixel.xyz <= 0.0031308), sRGBLo, sRGBHi);
	return float4(sRGB, pixel.w);
}



#endif
#ifndef _HLSL_FUNCTION_
#define _HLSL_FUNCTION_


///////////////////////////////// NormalMap //////////////////////////////////////////////
float3 Func_UnpackNormalXYZ(Texture2D texNormalMap,
                            SamplerState texNormalMapSampler,
                            float2 uv)
{
    float3 tangentNormal = texNormalMap.Sample(texNormalMapSampler, uv).rgb;
    return normalize(tangentNormal * 2.0 - 1.0);
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
    float3 tangentNormal = Func_UnpackNormalXYZ(texNormalMap,
                                                texNormalMapSampler,
                                                inTexCoord);

    float3 N = normalize(inWorldNormal);
    float3 T = normalize(inWorldTangent);
    float3 B = normalize(cross(N, T));
    float3x3 TBN = transpose(float3x3(T, B, N));

    return normalize(mul(TBN, tangentNormal));
}


///////////////////////////////// ParallaxMap ////////////////////////////////////////////




#endif
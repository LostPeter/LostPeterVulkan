//author: LostPeter
//time:   2022-11-13

struct VSOutput
{
    [[vk::location(0)]] float3 inColor          : COLOR0;
};

float4 main(VSOutput input) : SV_TARGET
{
    float3 outColor = input.inColor;
    
    return float4(outColor.xyz, 1.0);
}
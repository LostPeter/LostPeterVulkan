//author: LostPeter
//time:   2022-11-13

struct VSInput
{
    [[vk::location(0)]]float2 inPosition    : POSITION0;
    [[vk::location(1)]]float4 inColor       : COLOR0;
    [[vk::location(2)]]float2 inTexCoord    : TEXCOORD0;
};

struct VSOutput
{
	float4 outPosition                      : SV_POSITION;
    [[vk::location(0)]] float4 outColor     : COLOR0;
    [[vk::location(1)]] float2 outTexCoord  : TEXCOORD0;
};


VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput)0;
    output.outPosition = float4(input.inPosition, 0.0, 1.0);
    output.outColor = input.inColor;
    output.outTexCoord = input.inTexCoord;

    return output;
}
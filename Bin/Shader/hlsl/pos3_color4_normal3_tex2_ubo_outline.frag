//author: LostPeter
//time:   2022-12-11

struct VSOutput
{
    [[vk::location(0)]] float4 inColor          : COLOR0;
};

float4 main(VSOutput input) : SV_TARGET
{   
    return input.inColor;
}
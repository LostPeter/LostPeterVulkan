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

#ifndef _HLSL_SHADOW_
#define _HLSL_SHADOW_


float calculate_Depth(Texture2D textureDepth,
                      SamplerState textureDepthSampler,
                      float4 shadowCoord, 
                      float2 off,
                      float ambient)
{
	float shadow = 1.0;
	if (shadowCoord.z > -1.0 && shadowCoord.z < 1.0)
	{
		float dist = textureDepth.Sample(textureDepthSampler, shadowCoord.xy + off).r;
		if (shadowCoord.w > 0.0 && dist < shadowCoord.z)
		{
			shadow = ambient;
		}
	}
	return shadow;
}

float filterPCF(Texture2D textureDepth,
                SamplerState textureDepthSampler,
                float4 sc,
                float ambient)
{
	int2 texDim;
	textureDepth.GetDimensions(texDim.x, texDim.y);
	float scale = 1.5;
	float dx = scale * 1.0 / float(texDim.x);
	float dy = scale * 1.0 / float(texDim.y);

	float shadowFactor = 0.0;
	int count = 0;
	int range = 1;

	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			shadowFactor += calculate_Depth(textureDepth,
                                            textureDepthSampler,
                                            sc, 
                                            float2(dx*x, dy*y),
                                            ambient);
			count++;
		}

	}
	return shadowFactor / count;
}


#endif
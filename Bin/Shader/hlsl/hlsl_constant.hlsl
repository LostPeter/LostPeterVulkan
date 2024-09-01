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

#ifndef _HLSL_CONSTANT_
#define _HLSL_CONSTANT_


///////////////////////////////// Constants /////////////////////////////////////
//Common
#define PI 3.141592
#define PI_HALF 1.570796




//Max XX Count
#define MAX_LIGHT_COUNT                 16
#define MAX_OBJECT_COUNT                1024
#define MAX_TEXTURE_COUNT               16
#define MAX_MATERIAL_COUNT              64
#define MAX_INSTANCE_COUNT              1024
#define MAX_TERRAIN_SPLAT_COUNT         16
#define MAX_PLANE_COUNT		            6


//Static
static const float c_fOscillateDelta = 0.05;

static const float3 c_aAggressiveExtentArray[8] =
{
    float3( 1,       1,      1),
    float3( 1,       1,     -1),
    float3( 1,      -1,      1),
    float3( 1,      -1,     -1),
    float3(-1,       1,      1),
    float3(-1,       1,     -1),
    float3(-1,      -1,      1),
    float3(-1,      -1,     -1)
};



#endif
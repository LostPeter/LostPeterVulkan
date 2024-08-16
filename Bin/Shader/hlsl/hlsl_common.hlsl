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

#ifndef _HLSL_COMMON_
#define _HLSL_COMMON_

///////////////////////////////// Constants /////////////////////////////////////
#define MAX_LIGHT_COUNT 16
#define MAX_OBJECT_COUNT 1024
#define MAX_TEXTURE_COUNT 16
#define MAX_MATERIAL_COUNT 64
#define MAX_INSTANCE_COUNT 1024
#define MAX_TERRAIN_SPLAT_COUNT 16


///////////////////////////////// PassConstants /////////////////////////////////
//TransformConstants
struct TransformConstants
{
    float4x4 mat4View;
    float4x4 mat4View_Inv;
    float4x4 mat4Proj;
    float4x4 mat4Proj_Inv;
    float4x4 mat4ViewProj;
    float4x4 mat4ViewProj_Inv;
};

//CameraConstants
struct CameraConstants
{
    float3 posEyeWorld;    
    float fNearZ;
    float fFarZ;
    float fReserved1;
    float fReserved2;
    float fReserved3;
};

//LightConstants
struct LightConstants
{
    float4 common;      // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
    float3 position;    // directional/point/spot
    float falloffStart; // point/spot light only
    float3 direction;   // directional/spot light only
    float falloffEnd;   // point/spot light only
    float4 ambient;     // ambient
    float4 diffuse;     // diffuse
    float4 specular;    // specular

    float4x4 depthMVP;  // depthMVP
};

//PassConstants
struct PassConstants
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
    float2 g_RenderTargetSize;
    float2 g_RenderTargetSize_Inv;

    //Material
    float4 g_AmbientLight;
    
    //Light
    LightConstants g_MainLight;
    LightConstants g_AdditionalLights[MAX_LIGHT_COUNT];
};


///////////////////////////////// ObjectConstants ///////////////////////////////
//ObjectConstants
struct ObjectConstants
{
    float4x4 g_MatWorld;
};

//OutlineObjectConstants
struct OutlineObjectConstants
{
    float4x4 g_MatWorld;

    float4 outlineColor;
    float outlineWidth;
};

//CameraAxisObjectConstants
struct CameraAxisObjectConstants
{
    float4x4 g_MatWorld;
    float4 color;
};

//CoordinateAxisObjectConstants
struct CoordinateAxisObjectConstants
{
    float4x4 g_MatWorld;
    float4 color;
};

//GridObjectConstants
struct GridObjectConstants
{
    float4x4 g_MatWorld;
    float4 color;
};

//LineFlat2DObjectConstants
struct LineFlat2DObjectConstants
{
    float4 color;
};

//LineFlat3DObjectConstants
struct LineFlat3DObjectConstants
{
    float4x4 g_MatWorld;
    float4 color;
};

//CopyBlitObjectConstants
struct CopyBlitObjectConstants
{
    float offsetX;
    float offsetY;
    float scaleX;
    float scaleY;
};



///////////////////////////////// MaterialConstants /////////////////////////////
//TextureConstants
struct TextureConstants
{
    float texWidth;
    float texHeight;
    float texDepth;
    float indexTextureArray;

    float texSpeedU;
    float texSpeedV;
    float texSpeedW;
    float reserve0;

    float texChunkMaxX;
    float texChunkMaxY;
    float texChunkIndexX;
    float texChunkIndexY;
};

//MaterialConstants
struct MaterialConstants
{
    float4 factorAmbient;
    float4 factorDiffuse;
    float4 factorSpecular;

    float shininess;
    float alpha;
    float lighting;
    float reserve0;

    TextureConstants aTexLayers[MAX_TEXTURE_COUNT];
};


///////////////////////////////// InstanceConstants /////////////////////////////
//InstanceConstants
struct InstanceConstants
{
    int offsetObject;
    int indexObject;
    int offsetMaterial;
    int indexMaterial;
};


///////////////////////////////// ComputeConstants //////////////////////////////
struct TextureCopyConstants
{
    //[x,y,z,w] = [w,h,mip,processLinear]
    float4 texInfo;
	float4 texOffset;
	float4 texIndexArray;
	float4 texClearColor;
};


///////////////////////////////// GeometryConstants /////////////////////////////
//GeometryConstants
struct GeometryConstants
{
    float width;
    float height;
    float length;
	float reserve;
};


///////////////////////////////// TessellationConstants /////////////////////////
struct TessellationConstants
{
    float tessLevelOuter;
    float tessLevelInner;
    float tessAlpha;
    float tessStrength;
};


///////////////////////////////// GeometryConstants /////////////////////////////



///////////////////////////////// TerrainConstants //////////////////////////////
struct TerrainSplatConstants
{
    float splatSizeX; //size x
    float splatSizeY; //size y
    float splatOffsetX; //offset x
    float splatOffsetY; //offset y
};
struct TerrainConstants
{
    float4x4 matWorld; //Matrix world 
    float textureX; //HeightMap/NarmalMap Texture Size X
    float textureZ; //HeightMap/NarmalMap Texture Size Z
    float textureX_Inverse; //1/textureX
    float textureZ_Inverse; //1/textureZ
    float heightStart; //Height Low Start 
    float heightMax; //Height Max (from heightStart, heightEnd = heightStart + heightMax)
    float reserve1;
    float reserve2;

    TerrainSplatConstants aSplats[MAX_TERRAIN_SPLAT_COUNT];
};
struct TerrainObjectConstants
{
    float offsetX; //Instance offset X
    float offsetZ; //Instance offset Z
};


#endif
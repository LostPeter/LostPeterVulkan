/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _ENGINE_DEFINE_H_
#define _ENGINE_DEFINE_H_

#include "PreDefine.h"
#include "FMath.h"
#include "FMeshVertex.h"

namespace LostPeterEngine
{
/////Struct

    //////////////////////////////// TransformConstants /////////////////////////////
    struct engineExport TransformConstants
    {
        FMatrix4 mat4View;
        FMatrix4 mat4View_Inv;
        FMatrix4 mat4Proj;
        FMatrix4 mat4Proj_Inv;
        FMatrix4 mat4ViewProj;
        FMatrix4 mat4ViewProj_Inv;

        TransformConstants()
            : mat4View(FMath::Identity4x4())
            , mat4View_Inv(FMath::Identity4x4())
            , mat4Proj(FMath::Identity4x4())
            , mat4Proj_Inv(FMath::Identity4x4())
            , mat4ViewProj(FMath::Identity4x4())
            , mat4ViewProj_Inv(FMath::Identity4x4())
        {

        }
    };

    //////////////////////////////// CameraConstants ////////////////////////////////
    struct engineExport CameraConstants
    {
        FVector3 posEyeWorld;    
        float fNearZ;
        float fFarZ;
        float fReserved1;
        float fReserved2;
        float fReserved3;

        CameraConstants()
            : posEyeWorld(0.0f, 0.0f, 0.0f)
            , fNearZ(0.0f)
            , fFarZ(0.0f)
            , fReserved1(0.0f)
            , fReserved2(0.0f)
            , fReserved3(0.0f)
        {

        }
    }; 

    //////////////////////////////// LightConstants /////////////////////////////////
    struct engineExport LightConstants
    {
        FVector4 common;    // x: type; y: enable(1 or 0); z: 0-11; w: spotPower
        FVector3 position;  // directional/point/spot
        float falloffStart;  // point/spot light only
        FVector3 direction; // directional/spot light only
        float falloffEnd;    // point/spot light only
        FVector4 ambient;   // ambient
        FVector4 diffuse;   // diffuse
        FVector4 specular;  // specular

        LightConstants()
            : common(0, 0, 0, 64)
            , position(0.0f, 100.0f, 0.0f)
            , falloffStart(1.0f)
            , direction(0.0f, -1.0f, 0.0f)
            , falloffEnd(10.0f)
            , ambient(1.0f, 1.0f, 1.0f, 1.0f)
            , diffuse(1.0f, 1.0f, 1.0f, 1.0f)
            , specular(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
    };

    //////////////////////////////// PassConstants //////////////////////////////////
    struct engineExport PassConstants
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
        FVector2 g_RenderTargetSize;   
        FVector2 g_RenderTargetSize_Inv;

        //Material
        FVector4 g_AmbientLight;

        //Light
        LightConstants g_MainLight;
        LightConstants g_AdditionalLights[C_CONFIG_MAX_LIGHT_COUNT];

        PassConstants()
            : g_TotalTime(0.0f)
            , g_DeltaTime(0.0f)
            , g_Pad1(1.0f)
            , g_Pad2(1.0f)
            , g_RenderTargetSize(0.0f, 0.0f)
            , g_RenderTargetSize_Inv(0.0f, 0.0f)
            , g_AmbientLight(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
    };

    //////////////////////////////// ObjectConstants ////////////////////////////////
    struct engineExport ObjectConstants
    {
        FMatrix4 g_MatWorld;
        
        ObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
        {

        }
    };
    struct engineExport TerrainObjectConstants
    {
        FMatrix4 g_MatWorld;
    

        TerrainObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
        {

        }
    };
    struct engineExport GridObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;

        GridObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(0.5f, 0.5f, 0.5f, 1.0f)
        {
            
        }
    };
    struct engineExport CameraAxisObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;

        CameraAxisObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(0.5f, 0.5f, 0.5f, 1.0f)
        {
            
        }
    };
    struct engineExport CoordinateAxisObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;

        CoordinateAxisObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(0.5f, 0.5f, 0.5f, 1.0f)
        {
            
        }
    };
    struct engineExport LineFlat2DObjectConstants
    {
        FColor color;
        
        LineFlat2DObjectConstants()
            : color(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
    };
    struct engineExport LineFlat3DObjectConstants
    {
        FMatrix4 g_MatWorld;
        FColor color;
        
        LineFlat3DObjectConstants()
            : g_MatWorld(FMath::Identity4x4())
            , color(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
    };
    struct engineExport CopyBlitObjectConstants
    {
        float offsetX;
        float offsetY;
        float scaleX;
        float scaleY;

        CopyBlitObjectConstants()
            : offsetX(0.0f)
            , offsetY(0.0f)
            , scaleX(1.0f)
            , scaleY(1.0f)
        {
            
        }
    };

    //////////////////////////////// TextureConstants ///////////////////////////////
    struct engineExport TextureConstants
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

        TextureConstants()
            : texWidth(512.0f)
            , texHeight(512.0f)
            , texDepth(512.0f)
            , indexTextureArray(0.0f)

            , texSpeedU(0.0f)
            , texSpeedV(0.0f)
            , texSpeedW(0.0f)
            , reserve0(0.0f)
            
            , texChunkMaxX(0.0f)
            , texChunkMaxY(0.0f)
            , texChunkIndexX(0.0f)
            , texChunkIndexY(0.0f)
        {

        }
    };
    
    //////////////////////////////// MaterialConstants //////////////////////////////
    struct engineExport MaterialConstants
    {
        FVector4 factorAmbient;
        FVector4 factorDiffuse;
        FVector4 factorSpecular;

        float shininess;
        float alpha;
        float lighting;
        float reserve0;

        TextureConstants aTexLayers[C_CONFIG_MAX_TEXTURE_UNIT_COUNT];

        MaterialConstants()
            : factorAmbient(1.0f, 1.0f, 1.0f, 1.0f)
            , factorDiffuse(1.0f, 1.0f, 1.0f, 1.0f)
            , factorSpecular(1.0f, 1.0f, 1.0f, 1.0f)
            , shininess(20.0f)
            , alpha(1.0f)
            , lighting(1.0f)
            , reserve0(0.0f)
        {
            
        }
    };

    //////////////////////////////// InstanceConstants //////////////////////////////
    struct engineExport InstanceConstants
    {
        int offsetObject;
        int indexObject;
        int offsetMaterial;
        int indexMaterial;

        InstanceConstants()
            : offsetObject(0)
            , indexObject(0)
            , offsetMaterial(0)
            , indexMaterial(0)
        {

        }  
    };

    //////////////////////////////// TextureCopyConstants ///////////////////////////
    struct engineExport TextureCopyConstants
    {
        FVector4 texInfo;
        FVector4 texOffset;
        FVector4 texIndexArray;
        FVector4 texClearColor;

        TextureCopyConstants()
            : texInfo(0, 0, 0, 0)
            , texOffset(0, 0, 0, 0)
            , texIndexArray(0, 0, 0, 0)
            , texClearColor(0, 0, 0, 1)
        {

        }
    };

    //////////////////////////////// TessellationConstants //////////////////////////
    struct engineExport TessellationConstants
    {
    ////tesc
        float tessLevelOuter;
        float tessLevelInner;

    ////tese
        float tessAlpha;
        float tessStrength;

        TessellationConstants()
            : tessLevelOuter(3.0f)
            , tessLevelInner(3.0f)
            , tessAlpha(1.0f)
            , tessStrength(0.1f)
        {

        }
    };

    //////////////////////////////// GeometryConstants //////////////////////////////
    struct engineExport GeometryConstants
    {
        float width;
        float height;
        float length;
        float reserve;

        GeometryConstants()
            : width(0.05f)
            , height(3.0f)
            , length(0.02f)
            , reserve(0.0f)
        {

        }
    };

}; //LostPeterEngine

#endif
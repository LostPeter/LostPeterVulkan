/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_MESH_VERTEX_H_
#define _F_MESH_VERTEX_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    //////////////////////////////// FVertex_Pos2Color4 //////////////////////////////
    //0: F_MeshVertex_Pos2Color4
    struct LPF_Export FVertex_Pos2Color4
    {
        FVector2 pos;
        FVector4 color;

        FVertex_Pos2Color4()
        {
            Reset();
        }

        FVertex_Pos2Color4(const FVector2& _pos, 
                           const FVector4& _color)
            : pos(_pos)
            , color(_color)
        {

        }

        bool operator==(const FVertex_Pos2Color4& other) const 
        {
            return this->pos == other.pos && 
                   this->color == other.color;
        }

        void Reset()
        {
            this->pos = FVector2(0, 0);
            this->color = FVector4(0, 0, 0, 1);
        }
    };


    //////////////////////////////// FVertex_Pos3Color4 //////////////////////////////
    //1: F_MeshVertex_Pos3Color4
    struct LPF_Export FVertex_Pos3Color4 //FMeshVertexPC
    {
        FVector3 pos;
        FVector4 color;

        FVertex_Pos3Color4()
        {
            Reset();
        }

        FVertex_Pos3Color4(const FVector3& _pos, 
                           const FVector4& _color)
            : pos(_pos)
            , color(_color)
        {

        }

        FVertex_Pos3Color4(float px, float py, float pz,
                           float r, float g, float b, float a)
            : pos(px, py, pz)
            , color(r, g, b, a)
        {

        }

        bool operator==(const FVertex_Pos3Color4& other) const 
        {
            return this->pos == other.pos && 
                   this->color == other.color;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->color = FVector4(0, 0, 0, 1);
        }
    };
    typedef FVertex_Pos3Color4 FMeshVertexPC;
    typedef std::vector<FMeshVertexPC> FMeshVertexPCVector;


    //////////////////////////////// FVertex_Pos3Normal3 /////////////////////////////
    //2: F_MeshVertex_Pos3Normal3
    struct LPF_Export FVertex_Pos3Normal3
    {
        FVector3 pos;
        FVector3 normal;


        FVertex_Pos3Normal3()
        {
            Reset();
        }

        FVertex_Pos3Normal3(const FVector3& _pos, 
                            const FVector3& _normal)
            : pos(_pos)
            , normal(_normal)
        {

        }

        bool operator==(const FVertex_Pos3Normal3& other) const 
        {
            return this->pos == other.pos && 
                   this->normal == other.normal;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->normal = FVector3(0, 1, 0);
        }
    };


    //////////////////////////////// FVertex_Pos3Normal3Tex2 /////////////////////////
    //3: F_MeshVertex_Pos3Normal3Tex2
    struct LPF_Export FVertex_Pos3Normal3Tex2   
    {
        FVector3 pos;
        FVector3 normal;
        FVector2 texCoord;


        FVertex_Pos3Normal3Tex2()
        {
            Reset();
        }

        FVertex_Pos3Normal3Tex2(const FVector3& _pos, 
                                const FVector3& _normal,
                                const FVector2& _texCoord)
            : pos(_pos)
            , normal(_normal)
            , texCoord(_texCoord)
        {

        }

        bool operator==(const FVertex_Pos3Normal3Tex2& other) const 
        {
            return this->pos == other.pos && 
                   this->normal == other.normal &&
                   this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->normal = FVector3(0, 1, 0);
            this->texCoord = FVector2(0, 0);
        }
    };


    //////////////////////////////// FVertex_Pos2Color4Tex2 //////////////////////////
    //4: F_MeshVertex_Pos2Color4Tex2
    struct LPF_Export FVertex_Pos2Color4Tex2    
    {
        FVector2 pos;
        FVector4 color;
        FVector2 texCoord;


        FVertex_Pos2Color4Tex2()
        {
            Reset();
        }

        FVertex_Pos2Color4Tex2(const FVector2& _pos, 
                               const FVector4& _color, 
                               const FVector2& _texCoord)
            : pos(_pos)
            , color(_color)
            , texCoord(_texCoord)
        {

        }
        
        bool operator==(const FVertex_Pos2Color4Tex2& other) const 
        {
            return this->pos == other.pos && 
                   this->color == other.color && 
                   this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = FVector2(0, 0);
            this->color = FVector4(0, 0, 0, 1);
            this->texCoord = FVector2(0, 0);
        }
    };


    //////////////////////////////// FVertex_Pos3Color4Tex2 //////////////////////////
    //5: F_MeshVertex_Pos3Color4Tex2
    struct LPF_Export FVertex_Pos3Color4Tex2 //FMeshVertexPCT
    {
        FVector3 pos;
        FVector4 color;
        FVector2 texCoord;

        FVertex_Pos3Color4Tex2()
        {
            Reset();
        }

        FVertex_Pos3Color4Tex2(const FVector3& _pos, 
                               const FVector4& _color, 
                               const FVector2& _texCoord)
            : pos(_pos)
            , color(_color)
            , texCoord(_texCoord)
        {

        }

        bool operator==(const FVertex_Pos3Color4Tex2& other) const 
        {
            return this->pos == other.pos && 
                   this->color == other.color && 
                   this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->color = FVector4(0, 0, 0, 1);
            this->texCoord = FVector2(0, 0);
        }
    };
    typedef FVertex_Pos3Color4Tex2 FMeshVertexPCT;
    typedef std::vector<FMeshVertexPCT> FMeshVertexPCTVector;


    //////////////////////////////// FVertex_Pos3Color4Normal3Tex2 ///////////////////
    //6: F_MeshVertex_Pos3Color4Normal3Tex2
    struct LPF_Export FVertex_Pos3Color4Normal3Tex2
    {
        FVector3 pos;
        FVector4 color;
        FVector3 normal;
        FVector2 texCoord;

        FVertex_Pos3Color4Normal3Tex2()
        {
            Reset();
        }

        FVertex_Pos3Color4Normal3Tex2(const FVector3& _pos, 
                                      const FVector4& _color, 
                                      const FVector3& _normal, 
                                      const FVector2& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , texCoord(_texCoord)
        {
            
        }

        bool operator==(const FVertex_Pos3Color4Normal3Tex2& other) const 
        {
            return this->pos == other.pos && 
                   this->color == other.color && 
                   this->normal == other.normal && 
                   this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->color = FVector4(0, 0, 0, 1);
            this->normal = FVector3(0, 1, 0);
            this->texCoord = FVector2(0, 0);
        }
    };


    //////////////////////////////// FVertex_Pos3Color4Normal3Tex4 ///////////////////
    //7: F_MeshVertex_Pos3Color4Normal3Tex4
    struct LPF_Export FVertex_Pos3Color4Normal3Tex4
    {
        FVector3 pos;
        FVector4 color;
        FVector3 normal;
        FVector4 texCoord;

        FVertex_Pos3Color4Normal3Tex4()
        {
            Reset();
        }

        FVertex_Pos3Color4Normal3Tex4(const FVector3& _pos, 
                                      const FVector4& _color, 
                                      const FVector3& _normal, 
                                      const FVector4& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , texCoord(_texCoord)
        {
            
        }

        bool operator==(const FVertex_Pos3Color4Normal3Tex4& other) const 
        {
            return this->pos == other.pos && 
                   this->color == other.color && 
                   this->normal == other.normal && 
                   this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->color = FVector4(0, 0, 0, 1);
            this->normal = FVector3(0, 1, 0);
            this->texCoord = FVector4(0, 0, 0, 0);
        }
    };


    //////////////////////////////// FVertex_Pos3Color4Normal3Tangent3Tex2 ///////////////
    //8: F_MeshVertex_Pos3Color4Normal3Tangent3Tex2
    struct LPF_Export FVertex_Pos3Color4Normal3Tangent3Tex2 //FMeshVertex
    {
        FVector3 pos;
        FVector4 color;
        FVector3 normal;
        FVector3 tangent;
        FVector2 texCoord;


        FVertex_Pos3Color4Normal3Tangent3Tex2()
        {
            Reset();
        }

        FVertex_Pos3Color4Normal3Tangent3Tex2(const FVector3& _pos, 
                                              const FVector4& _color, 
                                              const FVector3& _normal, 
                                              const FVector3& _tangent, 
                                              const FVector2& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , tangent(_tangent)
            , texCoord(_texCoord)
        {
            
        }

        FVertex_Pos3Color4Normal3Tangent3Tex2(float px, float py, float pz,
                                              float nx, float ny, float nz,
                                              float tx, float ty, float tz,
                                              float u, float v)
            : pos(px, py, pz)
            , color(1.0f, 1.0f, 1.0f, 1.0f)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v)
        {

        }

        FVertex_Pos3Color4Normal3Tangent3Tex2(float px, float py, float pz,
                                              float cx, float cy, float cz, float cw,
                                              float nx, float ny, float nz,
                                              float tx, float ty, float tz,
                                              float u, float v)
            : pos(px, py, pz)
            , color(cx, cy, cz, cw)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v)
        {

        }

        bool operator==(const FVertex_Pos3Color4Normal3Tangent3Tex2& other) const 
        {
            return this->pos == other.pos && 
                   this->color == other.color && 
                   this->normal == other.normal && 
                   this->tangent == other.tangent && 
                   this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->color = FVector4(0, 0, 0, 1);
            this->normal = FVector3(0, 1, 0);
            this->tangent = FVector3(0, 0, 1);
            this->texCoord = FVector2(0, 0);
        }
    };
    typedef FVertex_Pos3Color4Normal3Tangent3Tex2 FMeshVertex;
    typedef std::vector<FMeshVertex> FMeshVertexVector;


    //////////////////////////////// FVertex_Pos3Color4Normal3Tangent3Tex4 ///////////////
    //9: F_MeshVertex_Pos3Color4Normal3Tangent3Tex4
    struct LPF_Export FVertex_Pos3Color4Normal3Tangent3Tex4 //FMeshVertexUV2
    {
        FVector3 pos;
        FVector4 color;
        FVector3 normal;
        FVector3 tangent;
        FVector4 texCoord;


        FVertex_Pos3Color4Normal3Tangent3Tex4()
        {
            Reset();
        }

        FVertex_Pos3Color4Normal3Tangent3Tex4(const FVector3& _pos, 
                                              const FVector4& _color, 
                                              const FVector3& _normal, 
                                              const FVector3& _tangent, 
                                              const FVector4& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , tangent(_tangent)
            , texCoord(_texCoord)
        {
            
        }

        FVertex_Pos3Color4Normal3Tangent3Tex4(float px, float py, float pz,
                                              float nx, float ny, float nz,
                                              float tx, float ty, float tz,
                                              float u, float v, float w, float x)
            : pos(px, py, pz)
            , color(1.0f, 1.0f, 1.0f, 1.0f)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v, w, x)
        {

        }

        FVertex_Pos3Color4Normal3Tangent3Tex4(float px, float py, float pz,
                                              float cx, float cy, float cz, float cw,
                                              float nx, float ny, float nz,
                                              float tx, float ty, float tz,
                                              float u, float v, float w, float x)
            : pos(px, py, pz)
            , color(cx, cy, cz, cw)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , texCoord(u, v, w, x)
        {

        }

        bool operator==(const FVertex_Pos3Color4Normal3Tangent3Tex4& other) const 
        {
            return this->pos == other.pos && 
                   this->color == other.color && 
                   this->normal == other.normal && 
                   this->tangent == other.tangent && 
                   this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->color = FVector4(0, 0, 0, 1);
            this->normal = FVector3(0, 1, 0);
            this->tangent = FVector3(0, 0, 1);
            this->texCoord = FVector4(0, 0, 0, 0);
        }
    };
    typedef FVertex_Pos3Color4Normal3Tangent3Tex4 FMeshVertexUV2;
    typedef std::vector<FMeshVertexUV2> FMeshVertexUV2Vector;


    //////////////////////////////// FVertex_Pos3Normal3Tangent3BlendWI8Tex2 //////////
    //10: F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
    struct FVertex_Pos3Normal3Tangent3BlendWI8Tex2
    {
        FVector3 pos;
        FVector3 normal;
        FVector3 tangent;
        FVector4 blendweight;
        FVector4 blendindex;
        FVector2 texCoord;


        FVertex_Pos3Normal3Tangent3BlendWI8Tex2()
        {
            Reset();
        }

        FVertex_Pos3Normal3Tangent3BlendWI8Tex2(const FVector3& _pos, 
                                                const FVector3& _normal, 
                                                const FVector3& _tangent, 
                                                const FVector4& _blendweight,
                                                const FVector4& _blendindex,
                                                const FVector2& _texCoord)
            : pos(_pos)
            , normal(_normal)
            , tangent(_tangent)
            , blendweight(_blendweight)
            , blendindex(_blendindex)
            , texCoord(_texCoord)
        {
            
        }

        FVertex_Pos3Normal3Tangent3BlendWI8Tex2(float px, float py, float pz,
                                                float nx, float ny, float nz,
                                                float tx, float ty, float tz,
                                                float bwx, float bwy, float bwz, float bww,
                                                float bix, float biy, float biz, float biw,  
                                                float u, float v)
            : pos(px, py, pz)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , blendweight(bwx, bwy, bwz, bww)
            , blendindex(bix, biy, biz, biw)
            , texCoord(u, v)
        {

        }

        bool operator==(const FVertex_Pos3Normal3Tangent3BlendWI8Tex2& other) const 
        {
            return this->pos == other.pos && 
                   this->normal == other.normal && 
                   this->tangent == other.tangent && 
                   this->blendweight == other.blendweight &&
                   this->blendindex == other.blendindex &&
                   this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->normal = FVector3(0, 1, 0);
            this->tangent = FVector3(0, 0, 1);
            this->blendweight = FVector4(0, 0, 0, 0);
            this->blendindex = FVector4(0, 0, 0, 0);
            this->texCoord = FVector2(0, 0);
        }
    };


    //////////////////////////////// FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 ////
    //11: F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2
    struct FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 //FMeshVertexSkin
    {
        FVector3 pos;
        FVector4 color;
        FVector3 normal;
        FVector3 tangent;
        FVector4 blendweight;
        FVector4 blendindex;
        FVector2 texCoord;


        FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2()
        {
            Reset();
        }

        FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2(const FVector3& _pos, 
                                                      const FVector4& _color, 
                                                      const FVector3& _normal, 
                                                      const FVector3& _tangent, 
                                                      const FVector4& _blendweight,
                                                      const FVector4& _blendindex,
                                                      const FVector2& _texCoord)
            : pos(_pos)
            , color(_color)
            , normal(_normal)
            , tangent(_tangent)
            , blendweight(_blendweight)
            , blendindex(_blendindex)
            , texCoord(_texCoord)
        {
            
        }

        FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2(float px, float py, float pz,
                                                      float cx, float cy, float cz, float cw,
                                                      float nx, float ny, float nz,
                                                      float tx, float ty, float tz,
                                                      float bwx, float bwy, float bwz, float bww,
                                                      float bix, float biy, float biz, float biw,  
                                                      float u, float v)
            : pos(px, py, pz)
            , color(cx, cy, cz, cw)
            , normal(nx, ny, nz)
            , tangent(tx, ty, tz)
            , blendweight(bwx, bwy, bwz, bww)
            , blendindex(bix, biy, biz, biw)
            , texCoord(u, v)
        {

        }

        bool operator==(const FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2& other) const 
        {
            return this->pos == other.pos && 
                   this->color == other.color && 
                   this->normal == other.normal && 
                   this->tangent == other.tangent && 
                   this->blendweight == other.blendweight &&
                   this->blendindex == other.blendindex &&
                   this->texCoord == other.texCoord;
        }

        void Reset()
        {
            this->pos = FVector3(0, 0, 0);
            this->color = FVector4(0, 0, 0, 1);
            this->normal = FVector3(0, 1, 0);
            this->tangent = FVector3(0, 0, 1);
            this->blendweight = FVector4(0, 0, 0, 0);
            this->blendindex = FVector4(0, 0, 0, 0);
            this->texCoord = FVector2(0, 0);
        }
    };
    typedef FVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 FMeshVertexSkin;
    typedef std::vector<FMeshVertexSkin> FMeshVertexSkinVector;
    

}; //LostPeterFoundation

#endif
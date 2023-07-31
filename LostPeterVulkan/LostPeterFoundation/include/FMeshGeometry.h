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

#ifndef _F_MESH_GEOMETRY_H_
#define _F_MESH_GEOMETRY_H_

#include "FPreDefine.h"
#include "FMeshData.h"
#include "FUtilString.h"

namespace LostPeterFoundation
{
    ///////////////////////////////////////// FMeshCreateParam ///////////////////////////////////////////
    class LPF_Export FMeshCreateParam
    {
    public:
        FMeshCreateParam(bool _flipV, bool _rightHand)
            : flipV(_flipV)
            , rightHand(_rightHand)
        {

        }
        virtual ~FMeshCreateParam()
        {

        }

    public:
        bool flipV;
        bool rightHand;
        
    public:
        virtual String ToName() = 0;
    };


    ////////////////////////////////// Line //////////////////////////////////
    //Line
    class LPF_Export FMeshCreateParam_Line : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_Line()
            : FMeshCreateParam(false, false)
            , vStart(0, 0, 0)
            , vEnd(1, 0, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_Line(const FVector3& _vStart,
                              const FVector3& _vEnd)
            : FMeshCreateParam(false, false)
            , vStart(_vStart)
            , vEnd(_vEnd)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_Line(const FVector3& _vStart,
                              const FVector3& _vEnd,
                              const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vStart(_vStart)
            , vEnd(_vEnd)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_Line()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vStart;
        FVector3 vEnd;
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]", 
                                            ms_nameType.c_str(), 
                                            this->vStart.x,
                                            this->vStart.y,
                                            this->vStart.z,
                                            this->vEnd.x,
                                            this->vEnd.y,
                                            this->vEnd.z);
        }
    };

    //LineTriangle
    class LPF_Export FMeshCreateParam_LineTriangle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineTriangle()
            : FMeshCreateParam(false, false)
            , vTop( 0.0f,  0.5f, 0.0f)
            , vLeft(-0.5f, -0.5f, 0.0f)
            , vRight( 0.5f, -0.5f, 0.0f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineTriangle(const FVector3& _vTop,
                                      const FVector3& _vLeft,
                                      const FVector3& _vRight)
            : FMeshCreateParam(false, false)
            , vTop(_vTop)
            , vLeft(_vLeft)
            , vRight(_vRight)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineTriangle(const FVector3& _vTop,
                                      const FVector3& _vLeft,
                                      const FVector3& _vRight,
                                      const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vTop(_vTop)
            , vLeft(_vLeft)
            , vRight(_vRight)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_LineTriangle()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vTop;
        FVector3 vLeft;
        FVector3 vRight;
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             this->vTop.x,
                                             this->vTop.y,
                                             this->vTop.z,
                                             this->vLeft.x,
                                             this->vLeft.y,
                                             this->vLeft.z,
                                             this->vRight.x,
                                             this->vRight.y,
                                             this->vRight.z);
        }
    };

    //LineQuad  
    class LPF_Export FMeshCreateParam_LineQuad : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineQuad()
            : FMeshCreateParam(false, false)
            , vLeftTop(-0.5f, 0.5f, 0.0f)
            , vLeftBottom(-0.5f, -0.5f, 0.0f)
            , vRightBottom(0.5f, -0.5f, 0.0f)
            , vRightTop(0.5f, 0.5f, 0.0f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineQuad(const FVector3& _vLeftTop,
                                  const FVector3& _vLeftBottom,
                                  const FVector3& _vRightBottom,
                                  const FVector3& _vRightTop)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineQuad(const FVector3& _vLeftTop,
                                  const FVector3& _vLeftBottom,
                                  const FVector3& _vRightBottom,
                                  const FVector3& _vRightTop,
                                  const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_LineQuad()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vLeftTop; 
        FVector3 vLeftBottom; 
        FVector3 vRightBottom; 
        FVector3 vRightTop; 
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             this->vLeftTop.x,
                                             this->vLeftTop.y,
                                             this->vLeftTop.z,
                                             this->vLeftBottom.x,
                                             this->vLeftBottom.y,
                                             this->vLeftBottom.z,
                                             this->vRightBottom.x,
                                             this->vRightBottom.y,
                                             this->vRightBottom.z,
                                             this->vRightTop.x,
                                             this->vRightTop.y,
                                             this->vRightTop.z);
        }
    };

    //LineGrid  
    class LPF_Export FMeshCreateParam_LineGrid : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineGrid()
            : FMeshCreateParam(false, false)
            , centerX(0)
            , centerY(0)
            , width(1)
            , height(1)
        {

        }
        FMeshCreateParam_LineGrid(float _centerX,
                                  float _centerY,
                                  float _width,
                                  float _height)
            : FMeshCreateParam(false, false)
            , centerX(_centerX)
            , centerY(_centerY)
            , width(_width)
            , height(_height)
        {

        }
        virtual ~FMeshCreateParam_LineGrid()
        {

        }

    public:
        static String ms_nameType;

    public:
        float centerX;
        float centerY;
        float width;
        float height;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%f", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             centerX,
                                             centerY,
                                             width,
                                             height);
        }
    };

    //LineCircle
    class LPF_Export FMeshCreateParam_LineCircle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineCircle()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_LineCircle()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //LineAABB
    class LPF_Export FMeshCreateParam_LineAABB : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineAABB()
            : FMeshCreateParam(false, false)
        {

        }
        FMeshCreateParam_LineAABB(float _width,
                                  float _height,
                                  float _depth)
            : FMeshCreateParam(false, false)
            , width(_width)
            , height(_height)
            , depth(_depth)
        {

        }
        virtual ~FMeshCreateParam_LineAABB()
        {

        }

    public:
        static String ms_nameType;

    public:
        float width;
        float height;
        float depth;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             width,
                                             height,
                                             depth);
        }
    };

    //LineSphere
    class LPF_Export FMeshCreateParam_LineSphere : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineSphere()
            : FMeshCreateParam(false, false)
        {

        }
        FMeshCreateParam_LineSphere(float _radius,
                                    uint32 _sliceCount,
                                    uint32 _stackCount)
            : FMeshCreateParam(false, false)
            , radius(_radius)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
        {

        }
        virtual ~FMeshCreateParam_LineSphere()
        {

        }

    public:
        static String ms_nameType;

    public:
        float radius;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%u-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             sliceCount,
                                             stackCount);
        }
    };

    //LineCylinder
    class LPF_Export FMeshCreateParam_LineCylinder : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineCylinder()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_LineCylinder()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };


    //LineCapsule
    class LPF_Export FMeshCreateParam_LineCapsule : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineCapsule()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_LineCapsule()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //LineCone
    class LPF_Export FMeshCreateParam_LineCone : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineCone()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_LineCone()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //LineTorus
    class LPF_Export FMeshCreateParam_LineTorus : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineTorus()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_LineTorus()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };


    ////////////////////////////////// Flat //////////////////////////////////
    //FlatTriangle
    class LPF_Export FMeshCreateParam_FlatTriangle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatTriangle()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_FlatTriangle()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //FlatQuad
    class LPF_Export FMeshCreateParam_FlatQuad : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatQuad()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_FlatQuad()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //FlatCircle
    class LPF_Export FMeshCreateParam_FlatCircle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatCircle()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_FlatCircle()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //FlatAABB
    class LPF_Export FMeshCreateParam_FlatAABB : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatAABB()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_FlatAABB()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //FlatSphere
    class LPF_Export FMeshCreateParam_FlatSphere : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatSphere()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_FlatSphere()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //FlatCylinder
    class LPF_Export FMeshCreateParam_FlatCylinder : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatCylinder()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_FlatCylinder()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //FlatCapsule
    class LPF_Export FMeshCreateParam_FlatCapsule : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatCapsule()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_FlatCapsule()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //FlatCone
    class LPF_Export FMeshCreateParam_FlatCone : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatCone()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_FlatCone()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //FlatTorus
    class LPF_Export FMeshCreateParam_FlatTorus : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatTorus()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_FlatTorus()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };


    ////////////////////////////////// Entity ////////////////////////////////
    //EntityTriangle
    class LPF_Export FMeshCreateParam_EntityTriangle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityTriangle()
            : FMeshCreateParam(true, false)
        {

        }
        FMeshCreateParam_EntityTriangle(bool _flipV, bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
        {

        }
        virtual ~FMeshCreateParam_EntityTriangle()
        {

        }

    public:
        static String ms_nameType;

    public:

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //EntityQuad
    class LPF_Export FMeshCreateParam_EntityQuad : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityQuad()
            : FMeshCreateParam(false, false)
            , centerX(0)
            , centerY(0)
            , width(1)
            , height(1)
            , depth(0)
        {

        }
        FMeshCreateParam_EntityQuad(float _centerX,
                                    float _centerY,
                                    float _width,
                                    float _height,
                                    float _depth,
                                    bool _flipV,
                                    bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , centerX(_centerX)
            , centerY(_centerY)
            , width(_width)
            , height(_height)
            , depth(_depth)
        {

        }
        virtual ~FMeshCreateParam_EntityQuad()
        {

        }

    public:
        static String ms_nameType;

    public:
        float centerX;
        float centerY;
        float width;
        float height;
        float depth;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%f", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             centerX,
                                             centerY,
                                             width,
                                             height,
                                             depth);
        }
    };

    //EntityGrid
    class LPF_Export FMeshCreateParam_EntityGrid : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityGrid()
            : FMeshCreateParam(false, false)
            , width(1)
            , height(1)
            , m(10)
            , n(10)
        {

        }
        FMeshCreateParam_EntityGrid(float _width,
                                    float _height,
                                    uint32 _m,
                                    uint32 _n,
                                    bool _flipV,
                                    bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , width(_width)
            , height(_height)
            , m(_m)
            , n(_n)
        {

        }
        virtual ~FMeshCreateParam_EntityGrid()
        {

        }

    public:
        static String ms_nameType;

    public:
        float width;
        float height;
        uint32 m;
        uint32 n;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%u-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             width,
                                             height,
                                             m,
                                             n);
        }
    };

    //EntityCircle
    class LPF_Export FMeshCreateParam_EntityCircle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityCircle()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , segment(100)
        {

        }
        FMeshCreateParam_EntityCircle(float _radius,
                                      uint32 _segment,
                                      bool _flipV,
                                      bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , radius(_radius)
            , segment(_segment)
        {

        }
        virtual ~FMeshCreateParam_EntityCircle()
        {

        }

    public:
        static String ms_nameType;

    public:
        float radius;
        uint32 segment;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             segment);
        }
    };

    //EntityAABB
    class LPF_Export FMeshCreateParam_EntityAABB : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityAABB()
            : FMeshCreateParam(false, false)
            , width(1)
            , height(1)
            , depth(1)
            , numSubdivisions(0)
        {

        }
        FMeshCreateParam_EntityAABB(float _width,
                                    float _height,
                                    float _depth,
                                    uint32 _numSubdivisions,
                                    bool _flipV,
                                    bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , width(_width)
            , height(_height)
            , depth(_depth)
            , numSubdivisions(_numSubdivisions)
        {

        }
        virtual ~FMeshCreateParam_EntityAABB()
        {
            
        }

    public:
        static String ms_nameType;

    public:
        float width;
        float height;
        float depth;
        uint32 numSubdivisions;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             width,
                                             height,
                                             depth,
                                             numSubdivisions);
        }
    };

    //EntitySphere
    class LPF_Export FMeshCreateParam_EntitySphere : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntitySphere()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , sliceCount(30)
            , stackCount(30)
        {

        }
        FMeshCreateParam_EntitySphere(float _radius,
                                      uint32 _sliceCount,
                                      uint32 _stackCount,
                                      bool _flipV,
                                      bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , radius(_radius)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
        {

        }
        virtual ~FMeshCreateParam_EntitySphere()
        {

        }

    public:
        static String ms_nameType;

    public:
        float radius;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%u-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             sliceCount,
                                             stackCount);
        }
    };

    //EntityGeoSphere
    class LPF_Export FMeshCreateParam_EntityGeoSphere : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityGeoSphere()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , numSubdivisions(5)
        {

        }
        FMeshCreateParam_EntityGeoSphere(float _radius,
                                         uint32 _numSubdivisions,
                                         bool _flipV,
                                         bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , radius(_radius)
            , numSubdivisions(_numSubdivisions)
        {

        }
        virtual ~FMeshCreateParam_EntityGeoSphere()
        {

        }

    public:
        static String ms_nameType;

    public:
        float radius;
        uint32 numSubdivisions;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             numSubdivisions);
        }
    };

    //EntityCylinder
    class LPF_Export FMeshCreateParam_EntityCylinder : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityCylinder()
            : FMeshCreateParam(false, false)
            , bottomRadius(0.5f)
            , topRadius(0.5f)
            , height(1.0f)
            , heightOffset(0.0f)
            , sliceCount(50)
            , stackCount(50)
        {

        }
        FMeshCreateParam_EntityCylinder(float _bottomRadius,
                                        float _topRadius,
                                        float _height,
                                        float _heightOffset,
                                        uint32 _sliceCount,
                                        uint32 _stackCount,
                                        bool _flipV,
                                        bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , bottomRadius(_bottomRadius)
            , topRadius(_topRadius)
            , height(_height)
            , heightOffset(_heightOffset)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
        {
        
        }
        virtual ~FMeshCreateParam_EntityCylinder()
        {

        }

    public:
        static String ms_nameType;

    public:
        float bottomRadius;
        float topRadius;
        float height;
        float heightOffset;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%u-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             bottomRadius,
                                             topRadius,
                                             height,
                                             heightOffset,
                                             sliceCount,
                                             stackCount);
        }
    };

    //EntityCapsule
    class LPF_Export FMeshCreateParam_EntityCapsule : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityCapsule()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , height(1.0f)
            , heightOffset(0.0f)
            , numRings(5)
            , numSegments(50)
            , numSegHeight(30)
        {

        }
        FMeshCreateParam_EntityCapsule(float _radius,
                                       float _height,
                                       float _heightOffset,
                                       uint32 _numRings,
                                       uint32 _numSegments,
                                       uint32 _numSegHeight,
                                       bool _flipV,
                                       bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , radius(_radius)
            , height(_height)
            , heightOffset(_heightOffset)
            , numRings(_numRings)
            , numSegments(_numSegments)
            , numSegHeight(_numSegHeight)
        {

        }
        virtual ~FMeshCreateParam_EntityCapsule()
        {

        }

    public:
        static String ms_nameType;

    public:
        float radius;
        float height;
        float heightOffset;

        uint32 numRings;
        uint32 numSegments;
        uint32 numSegHeight;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%u-%u-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             height,
                                             heightOffset,
                                             numRings,
                                             numSegments,
                                             numSegHeight);
        }
    };

    //EntityCone
    class LPF_Export FMeshCreateParam_EntityCone : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityCone()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , height(1.0f)
            , heightOffset(0.0f)
            , numSegBase(50)
            , numSegHeight(30)
        {

        }
        FMeshCreateParam_EntityCone(float _radius,
                                    float _height,
                                    float _heightOffset,
                                    uint32 _numSegBase,
                                    uint32 _numSegHeight,
                                    bool _flipV,
                                    bool _rightHand)
            : FMeshCreateParam(false, false)
            , radius(_radius)
            , height(_height)
            , heightOffset(_heightOffset)
            , numSegBase(_numSegBase)
            , numSegHeight(_numSegHeight)
        {

        }
        virtual ~FMeshCreateParam_EntityCone()
        {

        }

    public:
        static String ms_nameType;

    public:
        float radius;
        float height;
        float heightOffset;

        uint32 numSegBase;
	    uint32 numSegHeight;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%u-%u",
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             height,
                                             heightOffset,
                                             numSegBase,
                                             numSegHeight);
        }
    };

    //EntityTorus
    class LPF_Export FMeshCreateParam_EntityTorus : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityTorus()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , sectionRadius(0.2f)
            , numSegSection(50)
            , numSegCircle(20)
        {

        }
        FMeshCreateParam_EntityTorus(float _radius,
                                     float _sectionRadius,
                                     uint32 _numSegSection,
                                     uint32 _numSegCircle,
                                     bool _flipV,
                                     bool _rightHand)
            : FMeshCreateParam(false, false)
            , radius(_radius)
            , sectionRadius(_sectionRadius)
            , numSegSection(_numSegSection)
            , numSegCircle(_numSegCircle)
        {

        }
        virtual ~FMeshCreateParam_EntityTorus()
        {

        }

    public:
        static String ms_nameType;

    public:
        float radius;
        float sectionRadius;

        uint32 numSegSection;
        uint32 numSegCircle;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%u-%u",
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             sectionRadius,
                                             numSegSection,
                                             numSegCircle);
        }
    };

    //EntitySkyBox
    class LPF_Export FMeshCreateParam_EntitySkyBox : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntitySkyBox()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_EntitySkyBox()
        {

        }

    public:
        static String ms_nameType;

    public:
        

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //EntitySkyDome
    class LPF_Export FMeshCreateParam_EntitySkyDome : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntitySkyDome()
            : FMeshCreateParam(false, false)
        {

        }
        virtual ~FMeshCreateParam_EntitySkyDome()
        {

        }

    public:
        static String ms_nameType;

    public:
        

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0);
        }
    };

    //EntityTerrain
    class LPF_Export FMeshCreateParam_EntityTerrain : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityTerrain()
            : FMeshCreateParam(false, false)
            , offsetX(0)
            , offsetZ(0)
            , width(1024)
            , height(1024)
            , vertexX(1025)
            , vertexZ(1025)
            , pHeight(nullptr)
            , heightDataGap(1)
        {

        }
        FMeshCreateParam_EntityTerrain(float _offsetX,
                                       float _offsetZ,
                                       float _width,
                                       float _height,
                                       uint32 _vertexX,
                                       uint32 _vertexZ,
                                       float* _pHeight,
                                       uint32 _heightDataGap,
                                       bool _flipV,
                                       bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , offsetX(_offsetX)
            , offsetZ(_offsetZ)
            , width(_width)
            , height(_height)
            , vertexX(_vertexX)
            , vertexZ(_vertexZ)
            , pHeight(_pHeight)
            , heightDataGap(_heightDataGap)
        {

        }
        virtual ~FMeshCreateParam_EntityTerrain()
        {

        }

    public:
        static String ms_nameType;

    public:
        float offsetX;
        float offsetZ;
        float width;
        float height;
        uint32 vertexX;
        uint32 vertexZ;
        float* pHeight;
        uint32 heightDataGap;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%u-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             offsetX,
                                             offsetZ,
                                             width,
                                             height,
                                             vertexX,
                                             vertexZ);
        }
    };

    ///////////////////////////////////////// FMeshGeometry //////////////////////////////////////////////
    class LPF_Export FMeshGeometry
	{
    public:
        static bool CreateGeometry(FMeshDataPC* pMeshDataPC, FMeshData* pMeshData, FMeshGeometryType typeMeshGeometry);
		static bool CreateGeometryWithParam(FMeshDataPC* pMeshDataPC, FMeshData* pMeshData, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);


	public:
    ////////////////////////////////// Line //////////////////////////////////
        static bool CreateLineGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry);
		static bool CreateLineGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);

        //Line
        static void CreateLine(FMeshDataPC& meshDataPC, FMeshCreateParam_Line* pParam)
        {
            CreateLine(meshDataPC, 
                       pParam->vStart,
                       pParam->vEnd,
                       pParam->vColor);
        }
        static void CreateLine(FMeshDataPC& meshDataPC,
                               const FVector3& vStart,
                               const FVector3& vEnd,
                               const FVector4& vColor);

        //LineTriangle
        static void CreateLineTriangle(FMeshDataPC& meshDataPC, FMeshCreateParam_LineTriangle* pParam)
        {
            CreateLineTriangle(meshDataPC,
                               pParam->vTop,
                               pParam->vLeft,
                               pParam->vRight,
                               pParam->vColor);
        }
        static void CreateLineTriangle(FMeshDataPC& meshDataPC,
                                       const FVector3& vTop,
                                       const FVector3& vLeft,
                                       const FVector3& vRight,
                                       const FVector4& vColor);

        //LineQuad
        static void CreateLineQuad(FMeshDataPC& meshDataPC, FMeshCreateParam_LineQuad* pParam)
        {
            CreateLineQuad(meshDataPC,
                           pParam->vLeftTop,
                           pParam->vLeftBottom,
                           pParam->vRightBottom,
                           pParam->vRightTop,
                           pParam->vColor);
        }
        static void CreateLineQuad(FMeshDataPC& meshDataPC,
                                   const FVector3& vLeftTop,
                                   const FVector3& vLeftBottom,
                                   const FVector3& vRightBottom,
                                   const FVector3& vRightTop,
                                   const FVector4& vColor);

        //LineGrid
        static void CreateLineGrid(FMeshDataPC& meshDataPC, FMeshCreateParam_LineGrid* pParam)
        {
            CreateLineGrid(meshDataPC);
        }
        static void CreateLineGrid(FMeshDataPC& meshDataPC);

        //LineCircle
        static void CreateLineCircle(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCircle* pParam)
        {
            CreateLineCircle(meshDataPC);
        }
        static void CreateLineCircle(FMeshDataPC& meshDataPC);

        //LineAABB
        static void CreateLineAABB(FMeshDataPC& meshDataPC, FMeshCreateParam_LineAABB* pParam)
        {
            CreateLineAABB(meshDataPC, 
                           pParam->width, 
                           pParam->height, 
                           pParam->depth);
        }
        static void CreateLineAABB(FMeshDataPC& meshDataPC,
                                   float width,
                                   float height,
                                   float depth);

        //LineSphere
        static void CreateLineSphere(FMeshDataPC& meshDataPC, FMeshCreateParam_LineSphere* pParam)
        {
            CreateLineSphere(meshDataPC, 
                             pParam->radius, 
                             pParam->sliceCount, 
                             pParam->stackCount);
        }
        static void CreateLineSphere(FMeshDataPC& meshDataPC,
                                     float radius,
                                     uint32 sliceCount,
                                     uint32 stackCount);

        //LineCylinder
        static void CreateLineCylinder(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCylinder* pParam)
        {
            CreateLineCylinder(meshDataPC);
        }
        static void CreateLineCylinder(FMeshDataPC& meshDataPC);

        //LineCapsule
        static void CreateLineCapsule(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCapsule* pParam)
        {
            CreateLineCapsule(meshDataPC);
        }
        static void CreateLineCapsule(FMeshDataPC& meshDataPC);

        //LineCone
        static void CreateLineCone(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCone* pParam)
        {
            CreateLineCone(meshDataPC);
        }
        static void CreateLineCone(FMeshDataPC& meshDataPC);

        //LineTorus
        static void CreateLineTorus(FMeshDataPC& meshDataPC, FMeshCreateParam_LineTorus* pParam)
        {
            CreateLineTorus(meshDataPC);
        }
        static void CreateLineTorus(FMeshDataPC& meshDataPC);


    ////////////////////////////////// Flat //////////////////////////////////
        static bool CreateFlatGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry);
		static bool CreateFlatGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);

        //FlatTriangle
        static void CreateFlatTriangle(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatTriangle* pParam)
        {
            CreateFlatTriangle(meshDataPC);
        }
        static void CreateFlatTriangle(FMeshDataPC& meshDataPC);

        //FlatQuad
        static void CreateFlatQuad(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatQuad* pParam)
        {
            CreateFlatQuad(meshDataPC);
        }
        static void CreateFlatQuad(FMeshDataPC& meshDataPC);

        //FlatCircle
        static void CreateFlatCircle(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCircle* pParam)
        {
            CreateFlatCircle(meshDataPC);
        }
        static void CreateFlatCircle(FMeshDataPC& meshDataPC);

        //FlatAABB
        static void CreateFlatAABB(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatAABB* pParam)
        {
            CreateFlatAABB(meshDataPC);
        }
        static void CreateFlatAABB(FMeshDataPC& meshDataPC);

        //FlatSphere
        static void CreateFlatSphere(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatSphere* pParam)
        {
            CreateFlatSphere(meshDataPC);
        }
        static void CreateFlatSphere(FMeshDataPC& meshDataPC);

        //FlatCylinder
        static void CreateFlatCylinder(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCylinder* pParam)
        {
            CreateFlatCylinder(meshDataPC);
        }
        static void CreateFlatCylinder(FMeshDataPC& meshDataPC);

        //FlatCapsule
        static void CreateFlatCapsule(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCapsule* pParam)
        {
            CreateFlatCapsule(meshDataPC);
        }
        static void CreateFlatCapsule(FMeshDataPC& meshDataPC);

        //FlatCone
        static void CreateFlatCone(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCone* pParam)
        {
            CreateFlatCone(meshDataPC);
        }
        static void CreateFlatCone(FMeshDataPC& meshDataPC);

        //FlatTorus
        static void CreateFlatTorus(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatTorus* pParam)
        {
            CreateFlatTorus(meshDataPC);
        }
        static void CreateFlatTorus(FMeshDataPC& meshDataPC);


    ////////////////////////////////// Entity ////////////////////////////////
        static bool CreateEntityGeometry(FMeshData& meshData, FMeshGeometryType typeMeshGeometry);
		static bool CreateEntityGeometryWithParam(FMeshData& meshData, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);

	    //EntityTriangle
        static void CreateEntityTriangle(FMeshData& meshData, FMeshCreateParam_EntityTriangle* pParam)
        {
            CreateEntityTriangle(meshData, 
                                 pParam->flipV, 
                                 pParam->rightHand);
        }
        static void CreateEntityTriangle(FMeshData& meshData, 
                                         bool flipV,
                                         bool rightHand);

        //EntityQuad
        static void CreateEntityQuad(FMeshData& meshData, FMeshCreateParam_EntityQuad* pParam)
        {
            CreateEntityQuad(meshData, 
                             pParam->centerX, 
                             pParam->centerY, 
                             pParam->width, 
                             pParam->height, 
                             pParam->depth,  
                             pParam->flipV, 
                             pParam->rightHand);
        }
        static void CreateEntityQuad(FMeshData& meshData,
                                     float centerX,
                                     float centerY,
                                     float width,
                                     float height,
                                     float depth,
                                     bool flipV,
                                     bool rightHand);

        //EntityGrid
        static void CreateEntityGrid(FMeshData& meshData, FMeshCreateParam_EntityGrid* pParam)
        {
            CreateEntityGrid(meshData, 
                             pParam->width, 
                             pParam->height, 
                             pParam->m, 
                             pParam->n, 
                             pParam->flipV, 
                             pParam->rightHand);
        }
        static void CreateEntityGrid(FMeshData& meshData,
                                     float width,
                                     float height,
                                     uint32 m,
                                     uint32 n,
                                     bool flipV,
                                     bool rightHand);

        //EntityCircle
        static void CreateEntityCircle(FMeshData& meshData, FMeshCreateParam_EntityCircle* pParam)
        {
            CreateEntityCircle(meshData, 
                               pParam->radius, 
                               pParam->segment, 
                               pParam->flipV, 
                               pParam->rightHand);
        }
        static void CreateEntityCircle(FMeshData& meshData,
                                       float radius,
                                       uint32 segment,
                                       bool flipV,
                                       bool rightHand);

        //EntityAABB
        static void CreateEntityAABB(FMeshData& meshData, FMeshCreateParam_EntityAABB* pParam)
        {
            CreateEntityAABB(meshData, 
                             pParam->width, 
                             pParam->height, 
                             pParam->depth, 
                             pParam->numSubdivisions, 
                             pParam->flipV, 
                             pParam->rightHand);
        }
        static void CreateEntityAABB(FMeshData& meshData,
                                     float width,
                                     float height,
                                     float depth,
                                     uint32 numSubdivisions,
                                     bool flipV,
                                     bool rightHand);

        //EntitySphere
        static void CreateEntitySphere(FMeshData& meshData, FMeshCreateParam_EntitySphere* pParam)
        {
            CreateEntitySphere(meshData, 
                               pParam->radius, 
                               pParam->sliceCount, 
                               pParam->stackCount, 
                               pParam->flipV, 
                               pParam->rightHand);
        }
        static void CreateEntitySphere(FMeshData& meshData,
                                       float radius,
                                       uint32 sliceCount,
                                       uint32 stackCount,
                                       bool flipV,
                                       bool rightHand);

        //EntityGeoSphere
        static void CreateEntityGeoSphere(FMeshData& meshData, FMeshCreateParam_EntityGeoSphere* pParam)
        {
            CreateEntityGeoSphere(meshData, 
                                  pParam->radius,
                                  pParam->numSubdivisions, 
                                  pParam->flipV, 
                                  pParam->rightHand);
        }
        static void CreateEntityGeoSphere(FMeshData& meshData,
                                          float radius,
                                          uint32 numSubdivisions,
                                          bool flipV,
                                          bool rightHand);

        //EntityCylinder
        static void CreateEntityCylinder(FMeshData& meshData, FMeshCreateParam_EntityCylinder* pParam)
        {
            CreateEntityCylinder(meshData, 
                                 pParam->bottomRadius, 
                                 pParam->topRadius, 
                                 pParam->height, 
                                 pParam->heightOffset,
                                 pParam->sliceCount, 
                                 pParam->stackCount, 
                                 pParam->flipV, 
                                 pParam->rightHand);
        }
        static void CreateEntityCylinder(FMeshData& meshData,
                                         float bottomRadius,
                                         float topRadius,
                                         float height,
                                         float heightOffset,
                                         uint32 sliceCount,
                                         uint32 stackCount,
                                         bool flipV,
                                         bool rightHand);

        //EntityCapsule
        static void CreateEntityCapsule(FMeshData& meshData, FMeshCreateParam_EntityCapsule* pParam)
        {
            CreateEntityCapsule(meshData,
                                pParam->radius, 
                                pParam->height, 
                                pParam->heightOffset,
                                pParam->numRings, 
                                pParam->numSegments, 
                                pParam->numSegHeight, 
                                pParam->flipV, 
                                pParam->rightHand);
        }
        static void CreateEntityCapsule(FMeshData& meshData,
                                        float radius,
                                        float height,
                                        float heightOffset,
                                        uint32 numRings,
                                        uint32 numSegments,
                                        uint32 numSegHeight,
                                        bool flipV,
                                        bool rightHand);

        //EntityCone
        static void CreateEntityCone(FMeshData& meshData, FMeshCreateParam_EntityCone* pParam)
        {
            CreateEntityCone(meshData, 
                             pParam->radius, 
                             pParam->height, 
                             pParam->heightOffset,
                             pParam->numSegBase, 
                             pParam->numSegHeight, 
                             pParam->flipV, 
                             pParam->rightHand);
        }
        static void CreateEntityCone(FMeshData& meshData,
                                     float radius,
                                     float height,
                                     float heightOffset,
                                     uint32 numSegBase,
                                     uint32 numSegHeight,
                                     bool flipV,
                                     bool rightHand);

        //EntityTorus
        static void CreateEntityTorus(FMeshData& meshData, FMeshCreateParam_EntityTorus* pParam)
        {
            CreateEntityTorus(meshData, 
                              pParam->radius, 
                              pParam->sectionRadius, 
                              pParam->numSegSection, 
                              pParam->numSegCircle, 
                              pParam->flipV, 
                              pParam->rightHand);
        }
        static void CreateEntityTorus(FMeshData& meshData,
                                      float radius,
                                      float sectionRadius,
                                      uint32 numSegSection,
                                      uint32 numSegCircle,
                                      bool flipV,
                                      bool rightHand);

        //EntitySkyBox
        static void CreateEntitySkyBox(FMeshData& meshData, FMeshCreateParam_EntitySkyBox* pParam)
        {
            CreateEntitySkyBox(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateEntitySkyBox(FMeshData& meshData,
                                       bool flipV,
                                       bool rightHand);

        //EntitySkyDome
        static void CreateEntitySkyDome(FMeshData& meshData, FMeshCreateParam_EntitySkyDome* pParam)
        {
            CreateEntitySkyDome(meshData, pParam->flipV, pParam->rightHand);
        }
        static void CreateEntitySkyDome(FMeshData& meshData,
                                        bool flipV,
                                        bool rightHand);

        //EntityTerrain
        static void CreateEntityTerrain(FMeshData& meshData, FMeshCreateParam_EntityTerrain* pParam)
        {
            CreateEntityTerrain(meshData, 
                                pParam->offsetX, 
                                pParam->offsetZ, 
                                pParam->width, 
                                pParam->height, 
                                pParam->vertexX, 
                                pParam->vertexZ, 
                                pParam->pHeight, 
                                pParam->heightDataGap, 
                                pParam->flipV, 
                                pParam->rightHand);
        }
        static void CreateEntityTerrain(FMeshData& meshData,
                                        float offsetX,
                                        float offsetZ,
                                        float width,
                                        float height,
                                        uint32 vertexX,
                                        uint32 vertexZ,
                                        float* pHeight,
                                        uint32 heightDataGap,
                                        bool flipV,
                                        bool rightHand);

    public:
        static void MidPoint(const FMeshVertex& v0, const FMeshVertex& v1, FMeshVertex& m);
        static void SubDivide(FMeshData& meshData, bool rightHand);

    public:
        //FMeshDataPC
        static uint32 GetVertexCount(FMeshDataPC& meshDataPC);
        static void ReserveVertexCount(FMeshDataPC& meshDataPC, uint32 count);
        static void ResizeVertexCount(FMeshDataPC& meshDataPC, uint32 count);
        static FMeshVertexPC& GetVertex(FMeshDataPC& meshDataPC, uint32 index);
        static uint32 AddVertex(FMeshDataPC& meshDataPC, const FMeshVertexPC& vertex);
        static uint32 AddVertex(FMeshDataPC& meshDataPC, 
                                const FVector3& vPos,
                                const FVector4& color);
        static void SetVertex(FMeshDataPC& meshDataPC, int index, const FMeshVertexPC& vertex);

        static uint32 GetIndexCount(FMeshDataPC& meshDataPC);
        static void ReserveIndexCount(FMeshDataPC& meshDataPC, uint32 count);
        static void ResizeIndexCount(FMeshDataPC& meshDataPC, uint32 count);
        static uint32 GetIndex(FMeshDataPC& meshDataPC, uint32 index);
        static uint32 AddIndex(FMeshDataPC& meshDataPC, uint32 value);
        static void AddIndices(FMeshDataPC& meshDataPC, uint32 count, uint32* pIndex);
        static void SetIndex(FMeshDataPC& meshDataPC, uint32 index, uint32 value);
        static void AddIndexLine(FMeshDataPC& meshDataPC, uint32 index1, uint32 index2);
        static void AddIndexTriangle(FMeshDataPC& meshDataPC, uint32 index1, uint32 index2, uint32 index3);
        static void SetIndexTriangle(FMeshDataPC& meshDataPC, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3);

    public:
        //FMeshData
        static uint32 GetVertexCount(FMeshData& meshData);
        static void ReserveVertexCount(FMeshData& meshData, uint32 count);
        static void ResizeVertexCount(FMeshData& meshData, uint32 count);
        static FMeshVertex& GetVertex(FMeshData& meshData, uint32 index);
        static uint32 AddVertex(FMeshData& meshData, const FMeshVertex& vertex);
        static uint32 AddVertex(FMeshData& meshData, 
                                const FVector3& vPos,
                                const FVector4& color, 
                                const FVector3& normal, 
                                const FVector3& tangent, 
                                const FVector2& texCoord);
        static void SetVertex(FMeshData& meshData, int index, const FMeshVertex& vertex);

        static uint32 GetIndexCount(FMeshData& meshData);
        static void ReserveIndexCount(FMeshData& meshData, uint32 count);
        static void ResizeIndexCount(FMeshData& meshData, uint32 count);
        static uint32 GetIndex(FMeshData& meshData, uint32 index);
        static uint32 AddIndex(FMeshData& meshData, uint32 value);
        static void AddIndices(FMeshData& meshData, uint32 count, uint32* pIndex);
        static void SetIndex(FMeshData& meshData, uint32 index, uint32 value);
        static void AddIndexLine(FMeshData& meshData, uint32 index1, uint32 index2);
        static void AddIndexTriangle(FMeshData& meshData, uint32 index1, uint32 index2, uint32 index3);
        static void SetIndexTriangle(FMeshData& meshData, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3);
	};

}; //LostPeterFoundation

#endif
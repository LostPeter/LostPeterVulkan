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
    class foundationExport FMeshCreateParam
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
        virtual bool ParseParam(const String& nameParam) = 0;
    };


    ////////////////////////////////// Line2D ////////////////////////////////
    //LineLine2D
    class foundationExport FMeshCreateParam_LineLine2D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineLine2D()
            : FMeshCreateParam(false, false)
            , vStart(0, 0)
            , vEnd(1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineLine2D(const FVector2& _vStart,
                                    const FVector2& _vEnd)
            : FMeshCreateParam(false, false)
            , vStart(_vStart)
            , vEnd(_vEnd)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineLine2D(const FVector2& _vStart,
                                    const FVector2& _vEnd,
                                    const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vStart(_vStart)
            , vEnd(_vEnd)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_LineLine2D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector2 vStart;
        FVector2 vEnd;
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f]_[%f_%f]", 
                                             ms_nameType.c_str(), 
                                             this->vStart.x,
                                             this->vStart.y,
                                             this->vEnd.x,
                                             this->vEnd.y);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineTriangle2D
    class foundationExport FMeshCreateParam_LineTriangle2D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineTriangle2D()
            : FMeshCreateParam(false, false)
            , vTop( 0.0f,  0.5f)
            , vLeft(-0.5f, -0.5f)
            , vRight( 0.5f, -0.5f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineTriangle2D(const FVector2& _vTop,
                                        const FVector2& _vLeft,
                                        const FVector2& _vRight)
            : FMeshCreateParam(false, false)
            , vTop(_vTop)
            , vLeft(_vLeft)
            , vRight(_vRight)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineTriangle2D(const FVector2& _vTop,
                                        const FVector2& _vLeft,
                                        const FVector2& _vRight,
                                        const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vTop(_vTop)
            , vLeft(_vLeft)
            , vRight(_vRight)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_LineTriangle2D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector2 vTop;
        FVector2 vLeft;
        FVector2 vRight;
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f]_[%f_%f]_[%f_%f]", 
                                             ms_nameType.c_str(), 
                                             this->vTop.x,
                                             this->vTop.y,
                                             this->vLeft.x,
                                             this->vLeft.y,
                                             this->vRight.x,
                                             this->vRight.y);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineQuad2D
    class foundationExport FMeshCreateParam_LineQuad2D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineQuad2D()
            : FMeshCreateParam(false, false)
            , vLeftTop(-0.5f, 0.5f)
            , vLeftBottom(-0.5f, -0.5f)
            , vRightBottom(0.5f, -0.5f)
            , vRightTop(0.5f, 0.5f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineQuad2D(const FVector2& _vLeftTop,
                                    const FVector2& _vLeftBottom,
                                    const FVector2& _vRightBottom,
                                    const FVector2& _vRightTop)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineQuad2D(const FVector2& _vLeftTop,
                                    const FVector2& _vLeftBottom,
                                    const FVector2& _vRightBottom,
                                    const FVector2& _vRightTop,
                                    const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_LineQuad2D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector2 vLeftTop; 
        FVector2 vLeftBottom; 
        FVector2 vRightBottom; 
        FVector2 vRightTop; 
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f]_[%f_%f]_[%f_%f]_[%f_%f]", 
                                             ms_nameType.c_str(), 
                                             this->vLeftTop.x,
                                             this->vLeftTop.y,
                                             this->vLeftBottom.x,
                                             this->vLeftBottom.y,
                                             this->vRightBottom.x,
                                             this->vRightBottom.y,
                                             this->vRightTop.x,
                                             this->vRightTop.y);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineGrid2D
    class foundationExport FMeshCreateParam_LineGrid2D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineGrid2D()
            : FMeshCreateParam(false, false)
            , vLeftTop(-0.5f, 0.5f)
            , vLeftBottom(-0.5f, -0.5f)
            , vRightBottom(0.5f, -0.5f)
            , vRightTop(0.5f, 0.5f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , m(10)
            , n(10)
        {

        }
        FMeshCreateParam_LineGrid2D(const FVector2& _vLeftTop,
                                    const FVector2& _vLeftBottom,
                                    const FVector2& _vRightBottom,
                                    const FVector2& _vRightTop,
                                    uint32 _m,
                                    uint32 _n)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , m(_m)
            , n(_n)
        {

        }
        FMeshCreateParam_LineGrid2D(const FVector2& _vLeftTop,
                                    const FVector2& _vLeftBottom,
                                    const FVector2& _vRightBottom,
                                    const FVector2& _vRightTop,
                                    const FVector4& _vColor,
                                    uint32 _m,
                                    uint32 _n)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(_vColor)
            , m(_m)
            , n(_n)
        {

        }
        virtual ~FMeshCreateParam_LineGrid2D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector2 vLeftTop; 
        FVector2 vLeftBottom; 
        FVector2 vRightBottom; 
        FVector2 vRightTop; 
        FVector4 vColor;
        uint32 m;
        uint32 n;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f]_[%f_%f]_[%f_%f]_[%f_%f]_[%u_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vLeftTop.x,
                                             this->vLeftTop.y,
                                             this->vLeftBottom.x,
                                             this->vLeftBottom.y,
                                             this->vRightBottom.x,
                                             this->vRightBottom.y,
                                             this->vRightTop.x,
                                             this->vRightTop.y,
                                             this->m,
                                             this->n);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineCircle2D
    class foundationExport FMeshCreateParam_LineCircle2D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineCircle2D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0)
            , vDir(1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , viewWidth(1280)
            , viewHeight(720)
            , segment(100)
            , isDrawCenter(true)
        {

        }
        FMeshCreateParam_LineCircle2D(const FVector2& _vCenter,
                                      const FVector2& _vDir,
                                      float _radius,
                                      int32 _viewWidth,
                                      int32 _viewHeight,
                                      uint32 _segment,
                                      bool _isDrawCenter)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vDir(_vDir)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , viewWidth(_viewWidth)
            , viewHeight(_viewHeight)
            , segment(_segment)
            , isDrawCenter(_isDrawCenter)
        {

        }
        FMeshCreateParam_LineCircle2D(const FVector2& _vCenter,
                                      const FVector2& _vDir,
                                      const FVector4& _vColor,
                                      float _radius,
                                      int32 _viewWidth,
                                      int32 _viewHeight,
                                      uint32 _segment,
                                      bool _isDrawCenter)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vDir(_vDir)
            , vColor(_vColor)
            , radius(_radius)
            , viewWidth(_viewWidth)
            , viewHeight(_viewHeight)
            , segment(_segment)
            , isDrawCenter(_isDrawCenter)
        {

        }
        virtual ~FMeshCreateParam_LineCircle2D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector2 vCenter;
        FVector2 vDir;
        FVector4 vColor;
        float radius;
        int32 viewWidth;
        int32 viewHeight;
        uint32 segment;
        bool isDrawCenter;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f]_[%f_%f]_[%f_%d_%d_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vDir.x,
                                             this->vDir.y,
                                             this->radius,
                                             this->viewWidth,
                                             this->viewHeight,
                                             this->segment);
        }

        virtual bool ParseParam(const String& nameParam);

        virtual int32 GetRadiusI(const FVector2& scaleWindow);
    };


    ////////////////////////////////// Flat2D ////////////////////////////////
    //FlatTriangle2D
    class foundationExport FMeshCreateParam_FlatTriangle2D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatTriangle2D()
            : FMeshCreateParam(false, false)
            , vTop( 0.0f,  0.5f)
            , vLeft(-0.5f, -0.5f)
            , vRight( 0.5f, -0.5f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_FlatTriangle2D(const FVector2& _vTop,
                                        const FVector2& _vLeft,
                                        const FVector2& _vRight)
            : FMeshCreateParam(false, false)
            , vTop(_vTop)
            , vLeft(_vLeft)
            , vRight(_vRight)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_FlatTriangle2D(const FVector2& _vTop,
                                        const FVector2& _vLeft,
                                        const FVector2& _vRight,
                                        const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vTop(_vTop)
            , vLeft(_vLeft)
            , vRight(_vRight)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_FlatTriangle2D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector2 vTop;
        FVector2 vLeft;
        FVector2 vRight;
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f]_[%f_%f]_[%f_%f]", 
                                             ms_nameType.c_str(), 
                                             this->vTop.x,
                                             this->vTop.y,
                                             this->vLeft.x,
                                             this->vLeft.y,
                                             this->vRight.x,
                                             this->vRight.y);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatQuad2D
    class foundationExport FMeshCreateParam_FlatQuad2D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatQuad2D()
            : FMeshCreateParam(false, false)
            , vLeftTop(-0.5f, 0.5f)
            , vLeftBottom(-0.5f, -0.5f)
            , vRightBottom(0.5f, -0.5f)
            , vRightTop(0.5f, 0.5f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_FlatQuad2D(const FVector2& _vLeftTop,
                                    const FVector2& _vLeftBottom,
                                    const FVector2& _vRightBottom,
                                    const FVector2& _vRightTop)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_FlatQuad2D(const FVector2& _vLeftTop,
                                    const FVector2& _vLeftBottom,
                                    const FVector2& _vRightBottom,
                                    const FVector2& _vRightTop,
                                    const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_FlatQuad2D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector2 vLeftTop; 
        FVector2 vLeftBottom; 
        FVector2 vRightBottom; 
        FVector2 vRightTop; 
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f]_[%f_%f]_[%f_%f]_[%f_%f]", 
                                             ms_nameType.c_str(), 
                                             this->vLeftTop.x,
                                             this->vLeftTop.y,
                                             this->vLeftBottom.x,
                                             this->vLeftBottom.y,
                                             this->vRightBottom.x,
                                             this->vRightBottom.y,
                                             this->vRightTop.x,
                                             this->vRightTop.y);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatCircle2D
    class foundationExport FMeshCreateParam_FlatCircle2D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatCircle2D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0)
            , vDir(1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , viewWidth(1280)
            , viewHeight(720)
            , segment(100)
        {

        }
        FMeshCreateParam_FlatCircle2D(const FVector2& _vCenter,
                                      const FVector2& _vDir,
                                      float _radius,
                                      int32 _viewWidth,
                                      int32 _viewHeight,
                                      uint32 _segment)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vDir(_vDir)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , viewWidth(_viewWidth)
            , viewHeight(_viewHeight)
            , segment(_segment)
        {

        }
        FMeshCreateParam_FlatCircle2D(const FVector2& _vCenter,
                                      const FVector2& _vDir,
                                      const FVector4& _vColor,
                                      float _radius,
                                      int32 _viewWidth,
                                      int32 _viewHeight,
                                      uint32 _segment)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vDir(_vDir)
            , vColor(_vColor)
            , radius(_radius)
            , viewWidth(_viewWidth)
            , viewHeight(_viewHeight)
            , segment(_segment)
        {

        }
        virtual ~FMeshCreateParam_FlatCircle2D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector2 vCenter;
        FVector2 vDir;
        FVector4 vColor;
        float radius;
        int32 viewWidth;
        int32 viewHeight;
        uint32 segment;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f]_[%f_%f]_[%f_%d_%d_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vDir.x,
                                             this->vDir.y,
                                             this->radius,
                                             this->viewWidth,
                                             this->viewHeight,
                                             this->segment);
        }

        virtual bool ParseParam(const String& nameParam);

        virtual int32 GetRadiusI(const FVector2& scaleWindow);
    };


    ////////////////////////////////// Line3D ////////////////////////////////
    //LineLine3D
    class foundationExport FMeshCreateParam_LineLine3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineLine3D()
            : FMeshCreateParam(false, false)
            , vStart(0, 0, 0)
            , vEnd(1, 0, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineLine3D(const FVector3& _vStart,
                                    const FVector3& _vEnd)
            : FMeshCreateParam(false, false)
            , vStart(_vStart)
            , vEnd(_vEnd)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineLine3D(const FVector3& _vStart,
                                    const FVector3& _vEnd,
                                    const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vStart(_vStart)
            , vEnd(_vEnd)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_LineLine3D()
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

        virtual bool ParseParam(const String& nameParam);
    };

    //LineTriangle3D
    class foundationExport FMeshCreateParam_LineTriangle3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineTriangle3D()
            : FMeshCreateParam(false, false)
            , vTop( 0.0f,  0.5f, 0.0f)
            , vLeft(-0.5f, -0.5f, 0.0f)
            , vRight( 0.5f, -0.5f, 0.0f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineTriangle3D(const FVector3& _vTop,
                                        const FVector3& _vLeft,
                                        const FVector3& _vRight)
            : FMeshCreateParam(false, false)
            , vTop(_vTop)
            , vLeft(_vLeft)
            , vRight(_vRight)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineTriangle3D(const FVector3& _vTop,
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
        virtual ~FMeshCreateParam_LineTriangle3D()
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

        virtual bool ParseParam(const String& nameParam);
    };

    //LineQuad3D
    class foundationExport FMeshCreateParam_LineQuad3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineQuad3D()
            : FMeshCreateParam(false, false)
            , vLeftTop(-0.5f, 0.5f, 0.0f)
            , vLeftBottom(-0.5f, -0.5f, 0.0f)
            , vRightBottom(0.5f, -0.5f, 0.0f)
            , vRightTop(0.5f, 0.5f, 0.0f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineQuad3D(const FVector3& _vLeftTop,
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
        FMeshCreateParam_LineQuad3D(const FVector3& _vLeftTop,
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
        virtual ~FMeshCreateParam_LineQuad3D()
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

        virtual bool ParseParam(const String& nameParam);
    };

    //LineGrid3D
    class foundationExport FMeshCreateParam_LineGrid3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineGrid3D()
            : FMeshCreateParam(false, false)
            , vLeftTop(-0.5f, 0.5f, 0.0f)
            , vLeftBottom(-0.5f, -0.5f, 0.0f)
            , vRightBottom(0.5f, -0.5f, 0.0f)
            , vRightTop(0.5f, 0.5f, 0.0f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , m(10)
            , n(10)
        {

        }
        FMeshCreateParam_LineGrid3D(const FVector3& _vLeftTop,
                                    const FVector3& _vLeftBottom,
                                    const FVector3& _vRightBottom,
                                    const FVector3& _vRightTop,
                                    uint32 _m,
                                    uint32 _n)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , m(_m)
            , n(_n)
        {

        }
        FMeshCreateParam_LineGrid3D(const FVector3& _vLeftTop,
                                    const FVector3& _vLeftBottom,
                                    const FVector3& _vRightBottom,
                                    const FVector3& _vRightTop,
                                    const FVector4& _vColor,
                                    uint32 _m,
                                    uint32 _n)
            : FMeshCreateParam(false, false)
            , vLeftTop(_vLeftTop)
            , vLeftBottom(_vLeftBottom)
            , vRightBottom(_vRightBottom)
            , vRightTop(_vRightTop)
            , vColor(_vColor)
            , m(_m)
            , n(_n)
        {

        }
        virtual ~FMeshCreateParam_LineGrid3D()
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
        uint32 m;
        uint32 n;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]_[%u_%u]", 
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
                                             this->vRightTop.z,
                                             this->m,
                                             this->n);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineCircle3D
    class foundationExport FMeshCreateParam_LineCircle3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineCircle3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vDir(1, 0, 0)
            , vUp(0, 0, -1)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , segment(100)
            , isDrawCenter(true)
        {

        }
        FMeshCreateParam_LineCircle3D(const FVector3& _vCenter,
                                      const FVector3& _vDir,
                                      const FVector3& _vUp,
                                      float _radius,
                                      uint32 _segment,
                                      bool _isDrawCenter)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vDir(_vDir)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , segment(_segment)
            , isDrawCenter(_isDrawCenter)
        {

        }
        FMeshCreateParam_LineCircle3D(const FVector3& _vCenter,
                                      const FVector3& _vDir,
                                      const FVector3& _vUp,
                                      const FVector4& _vColor,
                                      float _radius,
                                      uint32 _segment,
                                      bool _isDrawCenter)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vDir(_vDir)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , segment(_segment)
            , isDrawCenter(_isDrawCenter)
        {

        }
        virtual ~FMeshCreateParam_LineCircle3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vDir;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        uint32 segment;
        bool isDrawCenter;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]_[%f_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vDir.x,
                                             this->vDir.y,
                                             this->vDir.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->segment);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineAABB3D
    class foundationExport FMeshCreateParam_LineAABB3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineAABB3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vExtent(0.5, 0.5, 0.5)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineAABB3D(const FVector3& _vCenter,
                                    const FVector3& _vExtent)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vExtent(_vExtent)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_LineAABB3D(const FVector3& _vCenter,
                                    const FVector3& _vExtent,
                                    const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vExtent(_vExtent)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_LineAABB3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vExtent;
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vExtent.x,
                                             this->vExtent.y,
                                             this->vExtent.z);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineSphere3D
    class foundationExport FMeshCreateParam_LineSphere3D: public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineSphere3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , sliceCount(30)
            , stackCount(30)
        {

        }
        FMeshCreateParam_LineSphere3D(const FVector3& _vCenter,
                                      const FVector3& _vUp,
                                      float _radius,
                                      uint32 _sliceCount,
                                      uint32 _stackCount)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
        {

        }
        FMeshCreateParam_LineSphere3D(const FVector3& _vCenter,
                                      const FVector3& _vUp,
                                      const FVector4& _vColor,
                                      float _radius,
                                      uint32 _sliceCount,
                                      uint32 _stackCount)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
        {

        }
        virtual ~FMeshCreateParam_LineSphere3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%u_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->sliceCount,
                                             this->stackCount);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineCylinder3D
    class foundationExport FMeshCreateParam_LineCylinder3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineCylinder3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radiusBottom(0.5f)
            , radiusTop(0.5f)
            , height(1.0f)
            , sliceCount(50)
            , isDrawCenter(true)
        {

        }
        FMeshCreateParam_LineCylinder3D(const FVector3& _vCenter,
                                        const FVector3& _vUp,
                                        float _radiusBottom,
                                        float _radiusTop,
                                        float _height,
                                        uint32 _sliceCount,
                                        bool _isDrawCenter)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radiusBottom(_radiusBottom)
            , radiusTop(_radiusTop)
            , height(_height)
            , sliceCount(_sliceCount)
            , isDrawCenter(_isDrawCenter)
        {
        
        }
        FMeshCreateParam_LineCylinder3D(const FVector3& _vCenter,
                                        const FVector3& _vUp,
                                        const FVector4& _vColor,
                                        float _radiusBottom,
                                        float _radiusTop,
                                        float _height,
                                        uint32 _sliceCount,
                                        bool _isDrawCenter)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radiusBottom(_radiusBottom)
            , radiusTop(_radiusTop)
            , height(_height)
            , sliceCount(_sliceCount)
            , isDrawCenter(_isDrawCenter)
        {
        
        }
        virtual ~FMeshCreateParam_LineCylinder3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radiusBottom;
        float radiusTop;
        float height;
        uint32 sliceCount;
        bool isDrawCenter;

    public:
        FVector3 GetCenterTop() const
        {
            return this->vCenter + this->vUp * this->height / 2.0f;
        }
        FVector3 GetCenterBottom() const
        {
            return this->vCenter - this->vUp * this->height / 2.0f;
        }
        float GetRadius() const
        {
            return (this->radiusBottom + this->radiusTop) / 2.0f;
        }

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radiusBottom,
                                             this->radiusTop,
                                             this->height,
                                             this->sliceCount);
        }

        virtual bool ParseParam(const String& nameParam);
    };


    //LineCapsule3D
    class foundationExport FMeshCreateParam_LineCapsule3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineCapsule3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , height(1.0f)
            , numRings(10)
            , numSegments(50)
        {

        }
        FMeshCreateParam_LineCapsule3D(const FVector3& _vCenter,
                                       const FVector3& _vUp,
                                       float _radius,
                                       float _height,
                                       uint32 _numRings,
                                       uint32 _numSegments)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , height(_height)
            , numRings(_numRings)
            , numSegments(_numSegments)
        {

        }
        FMeshCreateParam_LineCapsule3D(const FVector3& _vCenter,
                                       const FVector3& _vUp,
                                       const FVector4& _vColor,
                                       float _radius,
                                       float _height,
                                       uint32 _numRings,
                                       uint32 _numSegments)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , height(_height)
            , numRings(_numRings)
            , numSegments(_numSegments)
        {

        }
        virtual ~FMeshCreateParam_LineCapsule3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        float height;
        uint32 numRings;
        uint32 numSegments;

    public:
        FVector3 GetCenterTop() const
        {
            return this->vCenter + this->vUp * this->height / 2.0f;
        }
        FVector3 GetCenterBottom() const
        {
            return this->vCenter - this->vUp * this->height / 2.0f;
        }
        float GetRadius() const
        {
            return this->radius;
        }

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%u_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->height,
                                             this->numRings,
                                             this->numSegments);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineCone3D
    class foundationExport FMeshCreateParam_LineCone3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineCone3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , height(1.0f)
            , numSegBase(50)
        {

        }
        FMeshCreateParam_LineCone3D(const FVector3& _vCenter,
                                    const FVector3& _vUp,
                                    float _radius,
                                    float _height,
                                    uint32 _numSegBase)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , height(_height)
            , numSegBase(_numSegBase)
        {

        }
        FMeshCreateParam_LineCone3D(const FVector3& _vCenter,
                                    const FVector3& _vUp,
                                    const FVector4& _vColor,
                                    float _radius,
                                    float _height,
                                    uint32 _numSegBase)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , height(_height)
            , numSegBase(_numSegBase)
        {

        }
        virtual ~FMeshCreateParam_LineCone3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        float height;
        uint32 numSegBase;

    public:
        const FVector3& GetCenter() const
        {
            return this->vCenter;
        }
        const FVector3& GetUp() const
        {
            return this->vUp;
        }
        FVector3 GetTop() const
        {
            return this->vCenter + this->vUp * this->height;
        }
        float GetRadius() const
        {
            return this->radius;
        }
        float GetHeight() const
        {
            return this->height;
        }

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->height,
                                             this->numSegBase);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //LineTorus3D
    class foundationExport FMeshCreateParam_LineTorus3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_LineTorus3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , sectionRadius(0.2f)
            , numSegSection(50)
            , numSegCircle(20)
        {

        }
        FMeshCreateParam_LineTorus3D(const FVector3& _vCenter,
                                     const FVector3& _vUp,
                                     float _radius,
                                     float _sectionRadius,
                                     uint32 _numSegSection,
                                     uint32 _numSegCircle)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , sectionRadius(_sectionRadius)
            , numSegSection(_numSegSection)
            , numSegCircle(_numSegCircle)
        {

        }
        FMeshCreateParam_LineTorus3D(const FVector3& _vCenter,
                                     const FVector3& _vUp,
                                     const FVector4& _vColor,
                                     float _radius,
                                     float _sectionRadius,
                                     uint32 _numSegSection,
                                     uint32 _numSegCircle)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , sectionRadius(_sectionRadius)
            , numSegSection(_numSegSection)
            , numSegCircle(_numSegCircle)
        {

        }
        virtual ~FMeshCreateParam_LineTorus3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        float sectionRadius;
        uint32 numSegSection;
        uint32 numSegCircle;

    public:
        const FVector3& GetCenter() const
        {
            return this->vCenter;
        }
        const FVector3& GetUp() const
        {
            return this->vUp;
        }
        float GetRadius() const
        {
            return this->radius;
        }
        float GetSectionRadius() const
        {
            return this->sectionRadius;
        }

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%u_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->sectionRadius,
                                             this->numSegSection,
                                             this->numSegCircle);
        }

        virtual bool ParseParam(const String& nameParam);
    };


    ////////////////////////////////// Flat3D ////////////////////////////////
    //FlatTriangle3D
    class foundationExport FMeshCreateParam_FlatTriangle3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatTriangle3D()
            : FMeshCreateParam(false, false)
            , vTop( 0.0f,  0.5f, 0.0f)
            , vLeft(-0.5f, -0.5f, 0.0f)
            , vRight( 0.5f, -0.5f, 0.0f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_FlatTriangle3D(const FVector3& _vTop,
                                        const FVector3& _vLeft,
                                        const FVector3& _vRight)
            : FMeshCreateParam(false, false)
            , vTop(_vTop)
            , vLeft(_vLeft)
            , vRight(_vRight)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_FlatTriangle3D(const FVector3& _vTop,
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
        virtual ~FMeshCreateParam_FlatTriangle3D()
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

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatQuad3D
    class foundationExport FMeshCreateParam_FlatQuad3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatQuad3D()
            : FMeshCreateParam(false, false)
            , vLeftTop(-0.5f, 0.5f, 0.0f)
            , vLeftBottom(-0.5f, -0.5f, 0.0f)
            , vRightBottom(0.5f, -0.5f, 0.0f)
            , vRightTop(0.5f, 0.5f, 0.0f)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_FlatQuad3D(const FVector3& _vLeftTop,
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
        FMeshCreateParam_FlatQuad3D(const FVector3& _vLeftTop,
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
        virtual ~FMeshCreateParam_FlatQuad3D()
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

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatCircle3D
    class foundationExport FMeshCreateParam_FlatCircle3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatCircle3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vDir(1, 0, 0)
            , vUp(0, 0, -1)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , segment(100)
        {

        }
        FMeshCreateParam_FlatCircle3D(const FVector3& _vCenter,
                                      const FVector3& _vDir,
                                      const FVector3& _vUp,
                                      float _radius,
                                      uint32 _segment)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vDir(_vDir)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , segment(_segment)
        {

        }
        FMeshCreateParam_FlatCircle3D(const FVector3& _vCenter,
                                      const FVector3& _vDir,
                                      const FVector3& _vUp,
                                      const FVector4& _vColor,
                                      float _radius,
                                      uint32 _segment)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vDir(_vDir)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , segment(_segment)
        {

        }
        virtual ~FMeshCreateParam_FlatCircle3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vDir;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        uint32 segment;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]_[%f_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vDir.x,
                                             this->vDir.y,
                                             this->vDir.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->segment);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatAABB3D
    class foundationExport FMeshCreateParam_FlatAABB3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatAABB3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vExtent(0.5, 0.5, 0.5)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_FlatAABB3D(const FVector3& _vCenter,
                                    const FVector3& _vExtent)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vExtent(_vExtent)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
        {

        }
        FMeshCreateParam_FlatAABB3D(const FVector3& _vCenter,
                                    const FVector3& _vExtent,
                                    const FVector4& _vColor)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vExtent(_vExtent)
            , vColor(_vColor)
        {

        }
        virtual ~FMeshCreateParam_FlatAABB3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vExtent;
        FVector4 vColor;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vExtent.x,
                                             this->vExtent.y,
                                             this->vExtent.z);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatSphere3D
    class foundationExport FMeshCreateParam_FlatSphere3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatSphere3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , sliceCount(30)
            , stackCount(30)
        {

        }
        FMeshCreateParam_FlatSphere3D(const FVector3& _vCenter,
                                      const FVector3& _vUp,
                                      float _radius,
                                      uint32 _sliceCount,
                                      uint32 _stackCount)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
        {

        }
        FMeshCreateParam_FlatSphere3D(const FVector3& _vCenter,
                                      const FVector3& _vUp,
                                      const FVector4& _vColor,
                                      float _radius,
                                      uint32 _sliceCount,
                                      uint32 _stackCount)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
        {

        }
        virtual ~FMeshCreateParam_FlatSphere3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%u_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->sliceCount,
                                             this->stackCount);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatCylinder3D
    class foundationExport FMeshCreateParam_FlatCylinder3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatCylinder3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radiusBottom(0.5f)
            , radiusTop(0.5f)
            , height(1.0f)
            , sliceCount(50)
            , stackCount(30)
        {

        }
        FMeshCreateParam_FlatCylinder3D(const FVector3& _vCenter,
                                        const FVector3& _vUp,
                                        float _radiusBottom,
                                        float _radiusTop,
                                        float _height,
                                        uint32 _sliceCount,
                                        uint32 _stackCount)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radiusBottom(_radiusBottom)
            , radiusTop(_radiusTop)
            , height(_height)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
        {
        
        }
        FMeshCreateParam_FlatCylinder3D(const FVector3& _vCenter,
                                        const FVector3& _vUp,
                                        const FVector4& _vColor,
                                        float _radiusBottom,
                                        float _radiusTop,
                                        float _height,
                                        uint32 _sliceCount,
                                        uint32 _stackCount)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radiusBottom(_radiusBottom)
            , radiusTop(_radiusTop)
            , height(_height)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
        {
        
        }
        virtual ~FMeshCreateParam_FlatCylinder3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radiusBottom;
        float radiusTop;
        float height;
        uint32 sliceCount;
        uint32 stackCount;

    public:
        FVector3 GetCenterTop() const
        {
            return this->vCenter + this->vUp * this->height / 2.0f;
        }
        FVector3 GetCenterBottom() const
        {
            return this->vCenter - this->vUp * this->height / 2.0f;
        }
        float GetRadius() const
        {
            return (this->radiusBottom + this->radiusTop) / 2.0f;
        }

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f_%u_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radiusBottom,
                                             this->radiusTop,
                                             this->height,
                                             this->sliceCount,
                                             this->stackCount);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatCapsule3D
    class foundationExport FMeshCreateParam_FlatCapsule3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatCapsule3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , height(1.0f)
            , numRings(10)
            , numSegments(50)
            , numSegHeight(30)
        {

        }
        FMeshCreateParam_FlatCapsule3D(const FVector3& _vCenter,
                                       const FVector3& _vUp,
                                       float _radius,
                                       float _height,
                                       uint32 _numRings,
                                       uint32 _numSegments,
                                       uint32 _numSegHeight)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , height(_height)
            , numRings(_numRings)
            , numSegments(_numSegments)
            , numSegHeight(_numSegHeight)
        {

        }
        FMeshCreateParam_FlatCapsule3D(const FVector3& _vCenter,
                                       const FVector3& _vUp,
                                       const FVector4& _vColor,
                                       float _radius,
                                       float _height,
                                       uint32 _numRings,
                                       uint32 _numSegments,
                                       uint32 _numSegHeight)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , height(_height)
            , numRings(_numRings)
            , numSegments(_numSegments)
            , numSegHeight(_numSegHeight)
        {

        }
        virtual ~FMeshCreateParam_FlatCapsule3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        float height;
        uint32 numRings;
        uint32 numSegments;
        uint32 numSegHeight;

    public:
        FVector3 GetCenterTop() const
        {
            return this->vCenter + this->vUp * this->height / 2.0f;
        }
        FVector3 GetCenterBottom() const
        {
            return this->vCenter - this->vUp * this->height / 2.0f;
        }
        float GetRadius() const
        {
            return this->radius;
        }

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%u_%u_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->height,
                                             this->numRings,
                                             this->numSegments,
                                             this->numSegHeight);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatCone3D
    class foundationExport FMeshCreateParam_FlatCone3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatCone3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , height(1.0f)
            , numSegBase(50)
            , numSegHeight(30)
        {

        }
        FMeshCreateParam_FlatCone3D(const FVector3& _vCenter,
                                    const FVector3& _vUp,
                                    float _radius,
                                    float _height,
                                    uint32 _numSegBase,
                                    uint32 _numSegHeight)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , height(_height)
            , numSegBase(_numSegBase)
            , numSegHeight(_numSegHeight)
        {

        }
        FMeshCreateParam_FlatCone3D(const FVector3& _vCenter,
                                    const FVector3& _vUp,
                                    const FVector4& _vColor,
                                    float _radius,
                                    float _height,
                                    uint32 _numSegBase,
                                    uint32 _numSegHeight)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , height(_height)
            , numSegBase(_numSegBase)
            , numSegHeight(_numSegHeight)
        {

        }
        virtual ~FMeshCreateParam_FlatCone3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        float height;
        uint32 numSegBase;
        uint32 numSegHeight;

    public:
        const FVector3& GetCenter() const
        {
            return this->vCenter;
        }
        const FVector3& GetUp() const
        {
            return this->vUp;
        }
        FVector3 GetTop() const
        {
            return this->vCenter + this->vUp * this->height;
        }
        float GetRadius() const
        {
            return this->radius;
        }
        float GetHeight() const
        {
            return this->height;
        }

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%u_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->height,
                                             this->numSegBase,
                                             this->numSegHeight);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //FlatTorus3D
    class foundationExport FMeshCreateParam_FlatTorus3D : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_FlatTorus3D()
            : FMeshCreateParam(false, false)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(0.5f)
            , sectionRadius(0.2f)
            , numSegSection(50)
            , numSegCircle(20)
        {

        }
        FMeshCreateParam_FlatTorus3D(const FVector3& _vCenter,
                                     const FVector3& _vUp,
                                     float _radius,
                                     float _sectionRadius,
                                     uint32 _numSegSection,
                                     uint32 _numSegCircle)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(1.0f, 1.0f, 1.0f, 1.0f)
            , radius(_radius)
            , sectionRadius(_sectionRadius)
            , numSegSection(_numSegSection)
            , numSegCircle(_numSegCircle)
        {

        }
        FMeshCreateParam_FlatTorus3D(const FVector3& _vCenter,
                                     const FVector3& _vUp,
                                     const FVector4& _vColor,
                                     float _radius,
                                     float _sectionRadius,
                                     uint32 _numSegSection,
                                     uint32 _numSegCircle)
            : FMeshCreateParam(false, false)
            , vCenter(_vCenter)
            , vUp(_vUp)
            , vColor(_vColor)
            , radius(_radius)
            , sectionRadius(_sectionRadius)
            , numSegSection(_numSegSection)
            , numSegCircle(_numSegCircle)
        {

        }
        virtual ~FMeshCreateParam_FlatTorus3D()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vCenter;
        FVector3 vUp;
        FVector4 vColor;
        float radius;
        float sectionRadius;
        uint32 numSegSection;
        uint32 numSegCircle;

    public:
        const FVector3& GetCenter() const
        {
            return this->vCenter;
        }
        const FVector3& GetUp() const
        {
            return this->vUp;
        }
        float GetRadius() const
        {
            return this->radius;
        }
        float GetSectionRadius() const
        {
            return this->sectionRadius;
        }

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%u_%u]", 
                                             ms_nameType.c_str(), 
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z,
                                             this->radius,
                                             this->sectionRadius,
                                             this->numSegSection,
                                             this->numSegCircle);
        }

        virtual bool ParseParam(const String& nameParam);
    };


    ////////////////////////////////// Entity ////////////////////////////////
    //EntityTriangle
    class foundationExport FMeshCreateParam_EntityTriangle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityTriangle()
            : FMeshCreateParam(false, false)
            , vTop( 0.0f,  0.5f, 0.0f)
            , vLeft(-0.5f, -0.5f, 0.0f)
            , vRight( 0.5f, -0.5f, 0.0f)
        {

        }
        FMeshCreateParam_EntityTriangle(bool _flipV, bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , vTop( 0.0f,  0.5f, 0.0f)
            , vLeft(-0.5f, -0.5f, 0.0f)
            , vRight( 0.5f, -0.5f, 0.0f)
        {

        }
        virtual ~FMeshCreateParam_EntityTriangle()
        {

        }

    public:
        static String ms_nameType;

    public:
        FVector3 vTop;
        FVector3 vLeft;
        FVector3 vRight; 

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d_%d_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0,
                                             rightHand ? 1 : 0,
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

        virtual bool ParseParam(const String& nameParam);
    };

    //EntityQuad
    class foundationExport FMeshCreateParam_EntityQuad : public FMeshCreateParam
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
            RefreshVertexPos();
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
            RefreshVertexPos();
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

        FVector3 vLeftTop; 
        FVector3 vLeftBottom; 
        FVector3 vRightBottom; 
        FVector3 vRightTop; 

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%f-[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             centerX,
                                             centerY,
                                             width,
                                             height,
                                             depth,
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

        virtual bool ParseParam(const String& nameParam);

        virtual void RefreshVertexPos();
    };

    //EntityGrid
    class foundationExport FMeshCreateParam_EntityGrid : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityGrid()
            : FMeshCreateParam(false, false)
            , width(1)
            , height(1)
            , m(10)
            , n(10)
        {
            RefreshVertexPos();
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
            RefreshVertexPos();
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

        FVector3 vLeftTop; 
        FVector3 vLeftBottom; 
        FVector3 vRightBottom; 
        FVector3 vRightTop; 

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%u-%u-[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]_[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             width,
                                             height,
                                             m,
                                             n,
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

        virtual bool ParseParam(const String& nameParam);

        virtual void RefreshVertexPos();
    };

    //EntityCircle
    class foundationExport FMeshCreateParam_EntityCircle : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityCircle()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , segment(100)
            , vCenter(0, 0, 0)
            , vDir(1, 0, 0)
            , vUp(0, 0, -1)
        {

        }
        FMeshCreateParam_EntityCircle(float _radius,
                                      uint32 _segment,
                                      bool _flipV,
                                      bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , radius(_radius)
            , segment(_segment)
            , vCenter(0, 0, 0)
            , vDir(1, 0, 0)
            , vUp(0, 0, -1)
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

        FVector3 vCenter; 
        FVector3 vDir;
        FVector3 vUp;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%u-[%f_%f_%f]-[%f_%f_%f]-[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             segment,
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vDir.x,
                                             this->vDir.y,
                                             this->vDir.z,
                                             this->vUp.x,
                                             this->vUp.y,
                                             this->vUp.z);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //EntityAABB
    class foundationExport FMeshCreateParam_EntityAABB : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityAABB()
            : FMeshCreateParam(false, false)
            , width(1)
            , height(1)
            , depth(1)
            , numSubdivisions(0)
            , vCenter(0, 0, 0)
        {
            vExtent.x = width / 2;
            vExtent.y = height / 2;
            vExtent.z = depth / 2;
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
            , vCenter(0, 0, 0)
        {
            vExtent.x = width / 2;
            vExtent.y = height / 2;
            vExtent.z = depth / 2;
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

        FVector3 vCenter;
        FVector3 vExtent;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%u-[%f_%f_%f]_[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             width,
                                             height,
                                             depth,
                                             numSubdivisions,
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z,
                                             this->vExtent.x,
                                             this->vExtent.y,
                                             this->vExtent.z);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //EntitySphere
    class foundationExport FMeshCreateParam_EntitySphere : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntitySphere()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , sliceCount(30)
            , stackCount(30)
            , vCenter(0, 0, 0)
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
            , vCenter(0, 0, 0)
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

        FVector3 vCenter;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%u-%u-[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             sliceCount,
                                             stackCount,
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //EntityGeoSphere
    class foundationExport FMeshCreateParam_EntityGeoSphere : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityGeoSphere()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , numSubdivisions(5)
            , vCenter(0, 0, 0)
        {

        }
        FMeshCreateParam_EntityGeoSphere(float _radius,
                                         uint32 _numSubdivisions,
                                         bool _flipV,
                                         bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , radius(_radius)
            , numSubdivisions(_numSubdivisions)
            , vCenter(0, 0, 0)
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

        FVector3 vCenter;

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%u-[%f_%f_%f]", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radius,
                                             numSubdivisions,
                                             this->vCenter.x,
                                             this->vCenter.y,
                                             this->vCenter.z);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //EntityCylinder
    class foundationExport FMeshCreateParam_EntityCylinder : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityCylinder()
            : FMeshCreateParam(false, false)
            , radiusBottom(0.5f)
            , radiusTop(0.5f)
            , height(1.0f)
            , heightOffset(0.0f)
            , sliceCount(50)
            , stackCount(30)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
        {
            
        }
        FMeshCreateParam_EntityCylinder(float _radiusBottom,
                                        float _radiusTop,
                                        float _height,
                                        float _heightOffset,
                                        uint32 _sliceCount,
                                        uint32 _stackCount,
                                        bool _flipV,
                                        bool _rightHand)
            : FMeshCreateParam(_flipV, _rightHand)
            , radiusBottom(_radiusBottom)
            , radiusTop(_radiusTop)
            , height(_height)
            , heightOffset(_heightOffset)
            , sliceCount(_sliceCount)
            , stackCount(_stackCount)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
        {
            this->vCenter.y += this->heightOffset;
        }
        virtual ~FMeshCreateParam_EntityCylinder()
        {

        }

    public:
        static String ms_nameType;

    public:
        float radiusBottom;
        float radiusTop;
        float height;
        float heightOffset;
        uint32 sliceCount;
        uint32 stackCount;

        FVector3 vCenter;
        FVector3 vUp;

    public:
        FVector3 GetCenterTop() const
        {
            return this->vCenter + this->vUp * this->height / 2.0f;
        }
        FVector3 GetCenterBottom() const
        {
            return this->vCenter - this->vUp * this->height / 2.0f;
        }
        float GetRadius() const
        {
            return (this->radiusBottom + this->radiusTop) / 2.0f;
        }

    public:
        virtual String ToName()
        {
            return FUtilString::FormatString("%s_%d-%d-%f-%f-%f-%f-%u-%u", 
                                             ms_nameType.c_str(), 
                                             flipV ? 1 : 0, 
                                             rightHand ? 1 : 0,
                                             radiusBottom,
                                             radiusTop,
                                             height,
                                             heightOffset,
                                             sliceCount,
                                             stackCount);
        }

        virtual bool ParseParam(const String& nameParam);
    };

    //EntityCapsule
    class foundationExport FMeshCreateParam_EntityCapsule : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityCapsule()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , height(1.0f)
            , heightOffset(0.0f)
            , numRings(10)
            , numSegments(50)
            , numSegHeight(30)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
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
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
        {
            this->vCenter.y += this->heightOffset;
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

        FVector3 vCenter;
        FVector3 vUp;

    public:
        FVector3 GetCenterTop() const
        {
            return this->vCenter + this->vUp * this->height / 2.0f;
        }
        FVector3 GetCenterBottom() const
        {
            return this->vCenter - this->vUp * this->height / 2.0f;
        }
        float GetRadius() const
        {
            return this->radius;
        }

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

        virtual bool ParseParam(const String& nameParam);
    };

    //EntityCone
    class foundationExport FMeshCreateParam_EntityCone : public FMeshCreateParam
    {
    public:
        FMeshCreateParam_EntityCone()
            : FMeshCreateParam(false, false)
            , radius(0.5f)
            , height(1.0f)
            , heightOffset(0.0f)
            , numSegBase(50)
            , numSegHeight(30)
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
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
            , vCenter(0, 0, 0)
            , vUp(0, 1, 0)
        {
            this->vCenter.y += this->heightOffset;
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

        FVector3 vCenter;
        FVector3 vUp;

    public:
        const FVector3& GetCenter() const
        {
            return this->vCenter;
        }
        const FVector3& GetUp() const
        {
            return this->vUp;
        }
        FVector3 GetTop() const
        {
            return this->vCenter + this->vUp * this->height;
        }
        float GetRadius() const
        {
            return this->radius;
        }
        float GetHeight() const
        {
            return this->height;
        }

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

        virtual bool ParseParam(const String& nameParam);
    };

    //EntityTorus
    class foundationExport FMeshCreateParam_EntityTorus : public FMeshCreateParam
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

        FVector3 vCenter;
        FVector3 vUp;

    public:
        const FVector3& GetCenter() const
        {
            return this->vCenter;
        }
        const FVector3& GetUp() const
        {
            return this->vUp;
        }
        float GetRadius() const
        {
            return this->radius;
        }
        float GetSectionRadius() const
        {
            return this->sectionRadius;
        }

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

        virtual bool ParseParam(const String& nameParam);
    };

    //EntitySkyBox
    class foundationExport FMeshCreateParam_EntitySkyBox : public FMeshCreateParam
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

        virtual bool ParseParam(const String& nameParam);
    };

    //EntitySkyDome
    class foundationExport FMeshCreateParam_EntitySkyDome : public FMeshCreateParam
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

        virtual bool ParseParam(const String& nameParam);
    };

    //EntityTerrain
    class foundationExport FMeshCreateParam_EntityTerrain : public FMeshCreateParam
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

        virtual bool ParseParam(const String& nameParam);
    };

    ///////////////////////////////////////// FMeshGeometry //////////////////////////////////////////////
    class foundationExport FMeshGeometry
	{
    public:
        static FMeshCreateParam* CreateParam(FMeshGeometryType typeMeshGeometry);
        static FMeshCreateParam* CreateParam(FMeshGeometryType typeMeshGeometry, const String& nameParam);

    public:
        static bool CreateGeometry(FMeshDataPC* pMeshDataPC, FMeshData* pMeshData, FMeshGeometryType typeMeshGeometry);
		static bool CreateGeometryWithParam(FMeshDataPC* pMeshDataPC, FMeshData* pMeshData, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);

    ////////////////////////////////// Line2D ////////////////////////////////
    public:
        static bool CreateLine2DGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry);
		static bool CreateLine2DGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);

        //LineLine2D
        static void CreateLineLine2D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineLine2D* pParam)
        {
            CreateLineLine2D(meshDataPC, 
                             pParam->vStart,
                             pParam->vEnd,
                             pParam->vColor);
        }
        static void CreateLineLine2D(FMeshDataPC& meshDataPC,
                                     const FVector2& vStart,
                                     const FVector2& vEnd,
                                     const FVector4& vColor);

        //LineTriangle2D
        static void CreateLineTriangle2D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineTriangle2D* pParam)
        {
            CreateLineTriangle2D(meshDataPC,
                                 pParam->vTop,
                                 pParam->vLeft,
                                 pParam->vRight,
                                 pParam->vColor);
        }
        static void CreateLineTriangle2D(FMeshDataPC& meshDataPC,
                                         const FVector2& vTop,
                                         const FVector2& vLeft,
                                         const FVector2& vRight,
                                         const FVector4& vColor);

        //LineQuad2D
        static void CreateLineQuad2D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineQuad2D* pParam)
        {
            CreateLineQuad2D(meshDataPC,
                             pParam->vLeftTop,
                             pParam->vLeftBottom,
                             pParam->vRightBottom,
                             pParam->vRightTop,
                             pParam->vColor);
        }
        static void CreateLineQuad2D(FMeshDataPC& meshDataPC,
                                     const FVector2& vLeftTop,
                                     const FVector2& vLeftBottom,
                                     const FVector2& vRightBottom,
                                     const FVector2& vRightTop,
                                     const FVector4& vColor);

        //LineGrid2D
        static void CreateLineGrid2D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineGrid2D* pParam)
        {
            CreateLineGrid2D(meshDataPC,
                             pParam->vLeftTop,
                             pParam->vLeftBottom,
                             pParam->vRightBottom,
                             pParam->vRightTop,
                             pParam->vColor,
                             pParam->m,
                             pParam->n);
        }
        static void CreateLineGrid2D(FMeshDataPC& meshDataPC,
                                     const FVector2& vLeftTop,
                                     const FVector2& vLeftBottom,
                                     const FVector2& vRightBottom,
                                     const FVector2& vRightTop,
                                     const FVector4& vColor,
                                     uint32 m,
                                     uint32 n);

        //LineCircle2D
        static void CreateLineCircle2D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCircle2D* pParam)
        {
            CreateLineCircle2D(meshDataPC, 
                               pParam->vCenter,
                               pParam->vDir,
                               pParam->vColor,
                               pParam->radius,
                               pParam->viewWidth,
                               pParam->viewHeight,
                               pParam->segment,
                               pParam->isDrawCenter);
        }
        static void CreateLineCircle2D(FMeshDataPC& meshDataPC,
                                       const FVector2& vCenter,
                                       const FVector2& vDir,
                                       const FVector4& vColor,
                                       float radius,
                                       int32 viewWidth,
                                       int32 viewHeight,
                                       uint32 segment,
                                       bool isDrawCenter);
        static void UpdateLineCircle2D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCircle2D* pParam)
        {
            UpdateLineCircle2D(meshDataPC, 
                               pParam->vCenter,
                               pParam->vDir,
                               pParam->vColor,
                               pParam->radius,
                               pParam->viewWidth,
                               pParam->viewHeight,
                               pParam->segment,
                               pParam->isDrawCenter);
        }
        static void UpdateLineCircle2D(FMeshDataPC& meshDataPC, 
                                       const FVector2& vCenter,
                                       const FVector2& vDir,
                                       const FVector4& vColor,
                                       float radius,
                                       int32 viewWidth,
                                       int32 viewHeight,
                                       uint32 segment,
                                       bool isDrawCenter);
        static void UpdateLineCircle2D(FMeshVertexPCVector& aVertexPC, FMeshCreateParam_LineCircle2D* pParam)
        {
            UpdateLineCircle2D(aVertexPC,
                               pParam->vCenter,
                               pParam->vDir,
                               pParam->vColor,
                               pParam->radius,
                               pParam->viewWidth,
                               pParam->viewHeight,
                               pParam->segment,
                               pParam->isDrawCenter);
        }
        static void UpdateLineCircle2D(FMeshVertexPCVector& aVertexPC, 
                                       const FVector2& vCenter,
                                       const FVector2& vDir,
                                       const FVector4& vColor,
                                       float radius,
                                       int32 viewWidth,
                                       int32 viewHeight,
                                       uint32 segment,
                                       bool isDrawCenter);

    ////////////////////////////////// Flat2D ////////////////////////////////
    public:
        static bool CreateFlat2DGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry);
		static bool CreateFlat2DGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);

        //FlatTriangle2D
        static void CreateFlatTriangle2D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatTriangle2D* pParam)
        {
            CreateFlatTriangle2D(meshDataPC,
                                 pParam->vTop,
                                 pParam->vLeft,
                                 pParam->vRight,
                                 pParam->vColor);
        }
        static void CreateFlatTriangle2D(FMeshDataPC& meshDataPC,
                                         const FVector2& vTop,
                                         const FVector2& vLeft,
                                         const FVector2& vRight,
                                         const FVector4& vColor);

        //FlatQuad2D
        static void CreateFlatQuad2D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatQuad2D* pParam)
        {
            CreateFlatQuad2D(meshDataPC,
                             pParam->vLeftTop,
                             pParam->vLeftBottom,
                             pParam->vRightBottom,
                             pParam->vRightTop,
                             pParam->vColor);
        }
        static void CreateFlatQuad2D(FMeshDataPC& meshDataPC,
                                     const FVector2& vLeftTop,
                                     const FVector2& vLeftBottom,
                                     const FVector2& vRightBottom,
                                     const FVector2& vRightTop,
                                     const FVector4& vColor);

        //FlatCircle2D
        static void CreateFlatCircle2D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCircle2D* pParam)
        {
            CreateFlatCircle2D(meshDataPC,
                               pParam->vCenter,
                               pParam->vDir,
                               pParam->vColor,
                               pParam->radius,
                               pParam->viewWidth,
                               pParam->viewHeight,
                               pParam->segment);
        }
        static void CreateFlatCircle2D(FMeshDataPC& meshDataPC,
                                       const FVector2& vCenter,
                                       const FVector2& vDir,
                                       const FVector4& vColor,
                                       float radius,
                                       int32 viewWidth,
                                       int32 viewHeight,
                                       uint32 segment);
        static void UpdateFlatCircle2D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCircle2D* pParam)
        {
            UpdateFlatCircle2D(meshDataPC, 
                               pParam->vCenter,
                               pParam->vDir,
                               pParam->vColor,
                               pParam->radius,
                               pParam->viewWidth,
                               pParam->viewHeight,
                               pParam->segment);
        }
        static void UpdateFlatCircle2D(FMeshDataPC& meshDataPC, 
                                       const FVector2& vCenter,
                                       const FVector2& vDir,
                                       const FVector4& vColor,
                                       float radius,
                                       int32 viewWidth,
                                       int32 viewHeight,
                                       uint32 segment);
        static void UpdateFlatCircle2D(FMeshVertexPCVector& aVertexPC, FMeshCreateParam_FlatCircle2D* pParam)
        {
            UpdateFlatCircle2D(aVertexPC, 
                               pParam->vCenter,
                               pParam->vDir,
                               pParam->vColor,
                               pParam->radius,
                               pParam->viewWidth,
                               pParam->viewHeight,
                               pParam->segment);
        }
        static void UpdateFlatCircle2D(FMeshVertexPCVector& aVertexPC, 
                                       const FVector2& vCenter,
                                       const FVector2& vDir,
                                       const FVector4& vColor,
                                       float radius,
                                       int32 viewWidth,
                                       int32 viewHeight,
                                       uint32 segment);

    ////////////////////////////////// Line3D ////////////////////////////////
    public:
        static bool CreateLine3DGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry);
		static bool CreateLine3DGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);

        //LineLine3D
        static void CreateLineLine3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineLine3D* pParam)
        {
            CreateLineLine3D(meshDataPC, 
                             pParam->vStart,
                             pParam->vEnd,
                             pParam->vColor);
        }
        static void CreateLineLine3D(FMeshDataPC& meshDataPC,
                                     const FVector3& vStart,
                                     const FVector3& vEnd,
                                     const FVector4& vColor);

        //LineTriangle3D
        static void CreateLineTriangle3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineTriangle3D* pParam)
        {
            CreateLineTriangle3D(meshDataPC,
                                 pParam->vTop,
                                 pParam->vLeft,
                                 pParam->vRight,
                                 pParam->vColor);
        }
        static void CreateLineTriangle3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vTop,
                                         const FVector3& vLeft,
                                         const FVector3& vRight,
                                         const FVector4& vColor);

        //LineQuad3D
        static void CreateLineQuad3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineQuad3D* pParam)
        {
            CreateLineQuad3D(meshDataPC,
                             pParam->vLeftTop,
                             pParam->vLeftBottom,
                             pParam->vRightBottom,
                             pParam->vRightTop,
                             pParam->vColor);
        }
        static void CreateLineQuad3D(FMeshDataPC& meshDataPC,
                                     const FVector3& vLeftTop,
                                     const FVector3& vLeftBottom,
                                     const FVector3& vRightBottom,
                                     const FVector3& vRightTop,
                                     const FVector4& vColor);

        //LineGrid3D
        static void CreateLineGrid3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineGrid3D* pParam)
        {
            CreateLineGrid3D(meshDataPC,
                             pParam->vLeftTop,
                             pParam->vLeftBottom,
                             pParam->vRightBottom,
                             pParam->vRightTop,
                             pParam->vColor,
                             pParam->m,
                             pParam->n);
        }
        static void CreateLineGrid3D(FMeshDataPC& meshDataPC,
                                     const FVector3& vLeftTop,
                                     const FVector3& vLeftBottom,
                                     const FVector3& vRightBottom,
                                     const FVector3& vRightTop,
                                     const FVector4& vColor,
                                     uint32 m,
                                     uint32 n);

        //LineCircle3D
        static void CreateLineCircle3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCircle3D* pParam)
        {
            CreateLineCircle3D(meshDataPC, 
                               pParam->vCenter,
                               pParam->vDir,
                               pParam->vUp,
                               pParam->vColor,
                               pParam->radius,
                               pParam->segment,
                               pParam->isDrawCenter);
        }
        static void CreateLineCircle3D(FMeshDataPC& meshDataPC,
                                       const FVector3& vCenter,
                                       const FVector3& vDir,
                                       const FVector3& vUp,
                                       const FVector4& vColor,
                                       float radius,
                                       uint32 segment,
                                       bool isDrawCenter);

        //LineAABB3D
        static void CreateLineAABB3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineAABB3D* pParam)
        {
            CreateLineAABB3D(meshDataPC, 
                             pParam->vCenter, 
                             pParam->vExtent, 
                             pParam->vColor);
        }
        static void CreateLineAABB3D(FMeshDataPC& meshDataPC,
                                     const FVector3& vCenter,
                                     const FVector3& vExtent,
                                     const FVector4& vColor);

        //LineSphere3D
        static void CreateLineSphere3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineSphere3D* pParam)
        {
            CreateLineSphere3D(meshDataPC, 
                               pParam->vCenter,
                               pParam->vUp,
                               pParam->vColor,
                               pParam->radius, 
                               pParam->sliceCount, 
                               pParam->stackCount);
        }
        static void CreateLineSphere3D(FMeshDataPC& meshDataPC,
                                       const FVector3& vCenter,
                                       const FVector3& vUp,
                                       const FVector4& vColor,
                                       float radius,
                                       uint32 sliceCount,
                                       uint32 stackCount);

        //LineCylinder3D
        static void CreateLineCylinder3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCylinder3D* pParam)
        {
            CreateLineCylinder3D(meshDataPC,
                                 pParam->vCenter,
                                 pParam->vUp,
                                 pParam->vColor,
                                 pParam->radiusBottom, 
                                 pParam->radiusTop, 
                                 pParam->height, 
                                 pParam->sliceCount,
                                 pParam->isDrawCenter);
        }
        static void CreateLineCylinder3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vCenter,
                                         const FVector3& vUp,
                                         const FVector4& vColor,
                                         float radiusBottom,
                                         float radiusTop,
                                         float height,
                                         uint32 sliceCount,
                                         bool isDrawCenter);

        //LineCapsule3D
        static void CreateLineCapsule3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCapsule3D* pParam)
        {
            CreateLineCapsule3D(meshDataPC,
                                pParam->vCenter,
                                pParam->vUp,
                                pParam->vColor,
                                pParam->radius, 
                                pParam->height, 
                                pParam->numRings, 
                                pParam->numSegments);
        }
        static void CreateLineCapsule3D(FMeshDataPC& meshDataPC,
                                        const FVector3& vCenter,
                                        const FVector3& vUp,
                                        const FVector4& vColor,
                                        float radius,
                                        float height,
                                        uint32 numRings,
                                        uint32 numSegments);

        //LineCone3D
        static void CreateLineCone3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineCone3D* pParam)
        {
            CreateLineCone3D(meshDataPC,
                             pParam->vCenter,
                             pParam->vUp,
                             pParam->vColor,
                             pParam->radius, 
                             pParam->height, 
                             pParam->numSegBase);
        }
        static void CreateLineCone3D(FMeshDataPC& meshDataPC,
                                     const FVector3& vCenter,
                                     const FVector3& vUp,
                                     const FVector4& vColor,
                                     float radius,
                                     float height,
                                     uint32 numSegBase);

        //LineTorus3D
        static void CreateLineTorus3D(FMeshDataPC& meshDataPC, FMeshCreateParam_LineTorus3D* pParam)
        {
            CreateLineTorus3D(meshDataPC,
                              pParam->vCenter,
                              pParam->vUp,
                              pParam->vColor,
                              pParam->radius, 
                              pParam->sectionRadius, 
                              pParam->numSegSection, 
                              pParam->numSegCircle);
        }
        static void CreateLineTorus3D(FMeshDataPC& meshDataPC,
                                      const FVector3& vCenter,
                                      const FVector3& vUp,
                                      const FVector4& vColor,
                                      float radius,
                                      float sectionRadius,
                                      uint32 numSegSection,
                                      uint32 numSegCircle);


    ////////////////////////////////// Flat3D ////////////////////////////////
    public:
        static bool CreateFlat3DGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry);
		static bool CreateFlat3DGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);

        //FlatTriangle3D
        static void CreateFlatTriangle3D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatTriangle3D* pParam)
        {
            CreateFlatTriangle3D(meshDataPC,
                                 pParam->vTop,
                                 pParam->vLeft,
                                 pParam->vRight,
                                 pParam->vColor);
        }
        static void CreateFlatTriangle3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vTop,
                                         const FVector3& vLeft,
                                         const FVector3& vRight,
                                         const FVector4& vColor);

        //FlatQuad3D
        static void CreateFlatQuad3D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatQuad3D* pParam)
        {
            CreateFlatQuad3D(meshDataPC,
                             pParam->vLeftTop,
                             pParam->vLeftBottom,
                             pParam->vRightBottom,
                             pParam->vRightTop,
                             pParam->vColor);
        }
        static void CreateFlatQuad3D(FMeshDataPC& meshDataPC,
                                     const FVector3& vLeftTop,
                                     const FVector3& vLeftBottom,
                                     const FVector3& vRightBottom,
                                     const FVector3& vRightTop,
                                     const FVector4& vColor);

        //FlatCircle3D
        static void CreateFlatCircle3D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCircle3D* pParam)
        {
            CreateFlatCircle3D(meshDataPC,
                               pParam->vCenter,
                               pParam->vDir,
                               pParam->vUp,
                               pParam->vColor,
                               pParam->radius,
                               pParam->segment);
        }
        static void CreateFlatCircle3D(FMeshDataPC& meshDataPC,
                                       const FVector3& vCenter,
                                       const FVector3& vDir,
                                       const FVector3& vUp,
                                       const FVector4& vColor,
                                       float radius,
                                       uint32 segment);

        //FlatAABB3D
        static void CreateFlatAABB3D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatAABB3D* pParam)
        {
            CreateFlatAABB3D(meshDataPC,
                             pParam->vCenter, 
                             pParam->vExtent, 
                             pParam->vColor);
        }
        static void CreateFlatAABB3D(FMeshDataPC& meshDataPC,
                                     const FVector3& vCenter,
                                     const FVector3& vExtent,
                                     const FVector4& vColor);

        //FlatSphere3D
        static void CreateFlatSphere3D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatSphere3D* pParam)
        {
            CreateFlatSphere3D(meshDataPC,
                               pParam->vCenter,
                               pParam->vUp,
                               pParam->vColor,
                               pParam->radius, 
                               pParam->sliceCount, 
                               pParam->stackCount);
        }
        static void CreateFlatSphere3D(FMeshDataPC& meshDataPC,
                                       const FVector3& vCenter,
                                       const FVector3& vUp,
                                       const FVector4& vColor,
                                       float radius,
                                       uint32 sliceCount,
                                       uint32 stackCount);

        //FlatCylinder3D
        static void CreateFlatCylinder3D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCylinder3D* pParam)
        {
            CreateFlatCylinder3D(meshDataPC,
                                 pParam->vCenter,
                                 pParam->vUp,
                                 pParam->vColor,
                                 pParam->radiusBottom, 
                                 pParam->radiusTop, 
                                 pParam->height, 
                                 pParam->sliceCount,
                                 pParam->stackCount);
        }
        static void CreateFlatCylinder3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vCenter,
                                         const FVector3& vUp,
                                         const FVector4& vColor,
                                         float radiusBottom,
                                         float radiusTop,
                                         float height,
                                         uint32 sliceCount,
                                         uint32 stackCount);

        //FlatCapsule3D
        static void CreateFlatCapsule3D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCapsule3D* pParam)
        {
            CreateFlatCapsule3D(meshDataPC,
                                pParam->vCenter,
                                pParam->vUp,
                                pParam->vColor,
                                pParam->radius, 
                                pParam->height, 
                                pParam->numRings, 
                                pParam->numSegments,
                                pParam->numSegHeight);
        }
        static void CreateFlatCapsule3D(FMeshDataPC& meshDataPC,
                                        const FVector3& vCenter,
                                        const FVector3& vUp,
                                        const FVector4& vColor,
                                        float radius,
                                        float height,
                                        uint32 numRings,
                                        uint32 numSegments,
                                        uint32 numSegHeight);

        //FlatCone3D
        static void CreateFlatCone3D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatCone3D* pParam)
        {
            CreateFlatCone3D(meshDataPC,
                             pParam->vCenter,
                             pParam->vUp,
                             pParam->vColor,
                             pParam->radius, 
                             pParam->height, 
                             pParam->numSegBase,
                             pParam->numSegHeight);
        }
        static void CreateFlatCone3D(FMeshDataPC& meshDataPC,
                                     const FVector3& _vCenter,
                                     const FVector3& _vUp,
                                     const FVector4& _vColor,
                                     float _radius,
                                     float _height,
                                     uint32 _numSegBase,
                                     uint32 _numSegHeight);

        //FlatTorus3D
        static void CreateFlatTorus3D(FMeshDataPC& meshDataPC, FMeshCreateParam_FlatTorus3D* pParam)
        {
            CreateFlatTorus3D(meshDataPC,
                              pParam->vCenter,
                              pParam->vUp,
                              pParam->vColor,
                              pParam->radius, 
                              pParam->sectionRadius, 
                              pParam->numSegSection, 
                              pParam->numSegCircle);
        }
        static void CreateFlatTorus3D(FMeshDataPC& meshDataPC,
                                      const FVector3& vCenter,
                                      const FVector3& vUp,
                                      const FVector4& vColor,
                                      float radius,
                                      float sectionRadius,
                                      uint32 numSegSection,
                                      uint32 numSegCircle);


    ////////////////////////////////// Entity ////////////////////////////////
    public:
        static bool CreateEntityGeometry(FMeshData& meshData, FMeshGeometryType typeMeshGeometry);
		static bool CreateEntityGeometryWithParam(FMeshData& meshData, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam);

	    //EntityTriangle
        static void CreateEntityTriangle(FMeshData& meshData, FMeshCreateParam_EntityTriangle* pParam)
        {
            CreateEntityTriangle(meshData, 
                                 pParam->vTop,
                                 pParam->vLeft,
                                 pParam->vRight,
                                 pParam->flipV, 
                                 pParam->rightHand);
        }
        static void CreateEntityTriangle(FMeshData& meshData, 
                                         const FVector3& vTop,
                                         const FVector3& vLeft,
                                         const FVector3& vRight,
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
                                 pParam->radiusBottom, 
                                 pParam->radiusTop, 
                                 pParam->height, 
                                 pParam->heightOffset,
                                 pParam->sliceCount, 
                                 pParam->stackCount, 
                                 pParam->flipV, 
                                 pParam->rightHand);
        }
        static void CreateEntityCylinder(FMeshData& meshData,
                                         float radiusBottom,
                                         float radiusTop,
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
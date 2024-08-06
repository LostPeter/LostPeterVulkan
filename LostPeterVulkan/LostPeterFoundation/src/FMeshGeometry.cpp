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

#include "../include/FMeshGeometry.h"
#include "../include/FMath.h"
#include "../include/FUtilString.h"

namespace LostPeterFoundation
{
    ////////////////////////////////// Line2D ////////////////////////////////
    //LineLine2D
    String FMeshCreateParam_LineLine2D::ms_nameType = "MeshLineLine2D"; 
    bool FMeshCreateParam_LineLine2D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 4)
        {
            this->vStart.x = FUtilString::ParserFloat(aParam[0]);
            this->vStart.y = FUtilString::ParserFloat(aParam[1]);
            this->vEnd.x = FUtilString::ParserFloat(aParam[2]);
            this->vEnd.y = FUtilString::ParserFloat(aParam[3]);
        }
        else if (count == 8)
        {
            this->vStart.x = FUtilString::ParserFloat(aParam[0]);
            this->vStart.y = FUtilString::ParserFloat(aParam[1]);
            this->vEnd.x = FUtilString::ParserFloat(aParam[2]);
            this->vEnd.y = FUtilString::ParserFloat(aParam[3]);
            this->vColor.x = FUtilString::ParserFloat(aParam[4]);
            this->vColor.y = FUtilString::ParserFloat(aParam[5]);
            this->vColor.z = FUtilString::ParserFloat(aParam[6]);
            this->vColor.w = FUtilString::ParserFloat(aParam[7]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineLine2D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineTriangle2D
    String FMeshCreateParam_LineTriangle2D::ms_nameType = "MeshLineTriangle2D"; 
    bool FMeshCreateParam_LineTriangle2D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 6)
        {
            this->vTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeft.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeft.y = FUtilString::ParserFloat(aParam[3]);
            this->vRight.x = FUtilString::ParserFloat(aParam[4]);
            this->vRight.y = FUtilString::ParserFloat(aParam[5]);
        }
        else if (count == 10)
        {
            this->vTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeft.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeft.y = FUtilString::ParserFloat(aParam[3]);
            this->vRight.x = FUtilString::ParserFloat(aParam[4]);
            this->vRight.y = FUtilString::ParserFloat(aParam[5]);
            this->vColor.x = FUtilString::ParserFloat(aParam[6]);
            this->vColor.y = FUtilString::ParserFloat(aParam[7]);
            this->vColor.z = FUtilString::ParserFloat(aParam[8]);
            this->vColor.w = FUtilString::ParserFloat(aParam[9]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineTriangle2D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineQuad2D
    String FMeshCreateParam_LineQuad2D::ms_nameType = "MeshLineQuad2D"; 
    bool FMeshCreateParam_LineQuad2D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 8)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[3]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[4]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[5]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[7]);
        }
        else if (count == 12)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[3]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[4]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[5]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[7]);
            this->vColor.x = FUtilString::ParserFloat(aParam[8]);
            this->vColor.y = FUtilString::ParserFloat(aParam[9]);
            this->vColor.z = FUtilString::ParserFloat(aParam[10]);
            this->vColor.w = FUtilString::ParserFloat(aParam[11]);
        }   
        else
        {
            F_Assert(false && "FMeshCreateParam_LineQuad2D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineGrid2D
    String FMeshCreateParam_LineGrid2D::ms_nameType = "MeshLineGrid2D"; 
    bool FMeshCreateParam_LineGrid2D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 10)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[3]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[4]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[5]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[7]);
            this->m = FUtilString::ParserUInt(aParam[8]);
            this->n = FUtilString::ParserUInt(aParam[9]);
        }
        else if (count == 14)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[3]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[4]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[5]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[7]);
            this->m = FUtilString::ParserUInt(aParam[8]);
            this->n = FUtilString::ParserUInt(aParam[9]);
            this->vColor.x = FUtilString::ParserFloat(aParam[10]);
            this->vColor.y = FUtilString::ParserFloat(aParam[11]);
            this->vColor.z = FUtilString::ParserFloat(aParam[12]);
            this->vColor.w = FUtilString::ParserFloat(aParam[13]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineGrid2D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineCircle2D
    String FMeshCreateParam_LineCircle2D::ms_nameType = "MeshLineCircle2D"; 
    bool FMeshCreateParam_LineCircle2D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 9)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vDir.x = FUtilString::ParserFloat(aParam[2]);
            this->vDir.y = FUtilString::ParserFloat(aParam[3]);
            this->radius = FUtilString::ParserFloat(aParam[4]);
            this->viewWidth = FUtilString::ParserInt(aParam[5]);
            this->viewHeight = FUtilString::ParserInt(aParam[6]);
            this->segment = FUtilString::ParserUInt(aParam[7]);
            this->isDrawCenter = FUtilString::ParserBool(aParam[8]);
        }
        else if (count == 13)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vDir.x = FUtilString::ParserFloat(aParam[2]);
            this->vDir.y = FUtilString::ParserFloat(aParam[3]);
            this->radius = FUtilString::ParserFloat(aParam[4]);
            this->viewWidth = FUtilString::ParserInt(aParam[5]);
            this->viewHeight = FUtilString::ParserInt(aParam[6]);
            this->segment = FUtilString::ParserUInt(aParam[7]);
            this->isDrawCenter = FUtilString::ParserBool(aParam[8]);
            this->vColor.x = FUtilString::ParserFloat(aParam[9]);
            this->vColor.y = FUtilString::ParserFloat(aParam[10]);
            this->vColor.z = FUtilString::ParserFloat(aParam[11]);
            this->vColor.w = FUtilString::ParserFloat(aParam[12]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineCircle2D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }
    int32 FMeshCreateParam_LineCircle2D::GetRadiusI(const FVector2& scaleWindow)
    {
        if (this->viewWidth >= this->viewHeight)
        {
            return (int32)(this->radius * this->viewHeight / scaleWindow.y);
        }
        return (int32)(this->radius * this->viewWidth / scaleWindow.x);
    }

    ////////////////////////////////// Flat2D ////////////////////////////////
    //FlatTriangle2D
    String FMeshCreateParam_FlatTriangle2D::ms_nameType = "MeshFlatTriangle2D"; 
    bool FMeshCreateParam_FlatTriangle2D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 6)
        {
            this->vTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeft.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeft.y = FUtilString::ParserFloat(aParam[3]);
            this->vRight.x = FUtilString::ParserFloat(aParam[4]);
            this->vRight.y = FUtilString::ParserFloat(aParam[5]);
        }
        else if (count == 10)
        {
            this->vTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeft.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeft.y = FUtilString::ParserFloat(aParam[3]);
            this->vRight.x = FUtilString::ParserFloat(aParam[4]);
            this->vRight.y = FUtilString::ParserFloat(aParam[5]);
            this->vColor.x = FUtilString::ParserFloat(aParam[6]);
            this->vColor.y = FUtilString::ParserFloat(aParam[7]);
            this->vColor.z = FUtilString::ParserFloat(aParam[8]);
            this->vColor.w = FUtilString::ParserFloat(aParam[9]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatTriangle2D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatQuad2D
    String FMeshCreateParam_FlatQuad2D::ms_nameType = "MeshFlatQuad2D"; 
    bool FMeshCreateParam_FlatQuad2D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 8)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[3]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[4]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[5]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[7]);
        }
        else if (count == 12)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[3]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[4]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[5]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[7]);
            this->vColor.x = FUtilString::ParserFloat(aParam[8]);
            this->vColor.y = FUtilString::ParserFloat(aParam[9]);
            this->vColor.z = FUtilString::ParserFloat(aParam[10]);
            this->vColor.w = FUtilString::ParserFloat(aParam[11]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatQuad2D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatCircle2D
    String FMeshCreateParam_FlatCircle2D::ms_nameType = "MeshFlatCircle2D"; 
    bool FMeshCreateParam_FlatCircle2D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 8)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vDir.x = FUtilString::ParserFloat(aParam[2]);
            this->vDir.y = FUtilString::ParserFloat(aParam[3]);
            this->radius = FUtilString::ParserFloat(aParam[4]);
            this->viewWidth = FUtilString::ParserInt(aParam[5]);
            this->viewHeight = FUtilString::ParserInt(aParam[6]);
            this->segment = FUtilString::ParserUInt(aParam[7]);
        }
        else if (count == 12)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vDir.x = FUtilString::ParserFloat(aParam[2]);
            this->vDir.y = FUtilString::ParserFloat(aParam[3]);
            this->radius = FUtilString::ParserFloat(aParam[4]);
            this->viewWidth = FUtilString::ParserInt(aParam[5]);
            this->viewHeight = FUtilString::ParserInt(aParam[6]);
            this->segment = FUtilString::ParserUInt(aParam[7]);
            this->vColor.x = FUtilString::ParserFloat(aParam[8]);
            this->vColor.y = FUtilString::ParserFloat(aParam[9]);
            this->vColor.z = FUtilString::ParserFloat(aParam[10]);
            this->vColor.w = FUtilString::ParserFloat(aParam[11]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatCircle2D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    int32 FMeshCreateParam_FlatCircle2D::GetRadiusI(const FVector2& scaleWindow)
    {
        if (this->viewWidth >= this->viewHeight)
        {
            return (int32)(this->radius * this->viewHeight / scaleWindow.y);
        }
        return (int32)(this->radius * this->viewWidth / scaleWindow.x);
    }


    ////////////////////////////////// Line3D ////////////////////////////////
    //LineLine3D
    String FMeshCreateParam_LineLine3D::ms_nameType = "MeshLine3D"; 
    bool FMeshCreateParam_LineLine3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 6)
        {
            this->vStart.x = FUtilString::ParserFloat(aParam[0]);
            this->vStart.y = FUtilString::ParserFloat(aParam[1]);
            this->vStart.z = FUtilString::ParserFloat(aParam[2]);
            this->vEnd.x = FUtilString::ParserFloat(aParam[3]);
            this->vEnd.y = FUtilString::ParserFloat(aParam[4]);
            this->vEnd.z = FUtilString::ParserFloat(aParam[5]);
        }   
        else if (count == 10)
        {
            this->vStart.x = FUtilString::ParserFloat(aParam[0]);
            this->vStart.y = FUtilString::ParserFloat(aParam[1]);
            this->vStart.z = FUtilString::ParserFloat(aParam[2]);
            this->vEnd.x = FUtilString::ParserFloat(aParam[3]);
            this->vEnd.y = FUtilString::ParserFloat(aParam[4]);
            this->vEnd.z = FUtilString::ParserFloat(aParam[5]);
            this->vColor.x = FUtilString::ParserFloat(aParam[6]);
            this->vColor.y = FUtilString::ParserFloat(aParam[7]);
            this->vColor.z = FUtilString::ParserFloat(aParam[8]);
            this->vColor.w = FUtilString::ParserFloat(aParam[9]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineLine3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineTriangle3D
    String FMeshCreateParam_LineTriangle3D::ms_nameType = "MeshLineTriangle3D"; 
    bool FMeshCreateParam_LineTriangle3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 9)
        {
            this->vTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeft.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeft.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeft.z = FUtilString::ParserFloat(aParam[5]);
            this->vRight.x = FUtilString::ParserFloat(aParam[6]);
            this->vRight.y = FUtilString::ParserFloat(aParam[7]);
            this->vRight.z = FUtilString::ParserFloat(aParam[8]);
        }
        else if (count == 13)
        {
            this->vTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeft.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeft.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeft.z = FUtilString::ParserFloat(aParam[5]);
            this->vRight.x = FUtilString::ParserFloat(aParam[6]);
            this->vRight.y = FUtilString::ParserFloat(aParam[7]);
            this->vRight.z = FUtilString::ParserFloat(aParam[8]);
            this->vColor.x = FUtilString::ParserFloat(aParam[9]);
            this->vColor.y = FUtilString::ParserFloat(aParam[10]);
            this->vColor.z = FUtilString::ParserFloat(aParam[11]);
            this->vColor.w = FUtilString::ParserFloat(aParam[12]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineTriangle3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineQuad3D
    String FMeshCreateParam_LineQuad3D::ms_nameType = "MeshLineQuad3D"; 
    bool FMeshCreateParam_LineQuad3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 12)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeftBottom.z = FUtilString::ParserFloat(aParam[5]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[7]);
            this->vRightBottom.z = FUtilString::ParserFloat(aParam[8]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[9]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[10]);
            this->vRightTop.z = FUtilString::ParserFloat(aParam[11]);
        }
        else if (count == 16)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeftBottom.z = FUtilString::ParserFloat(aParam[5]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[7]);
            this->vRightBottom.z = FUtilString::ParserFloat(aParam[8]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[9]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[10]);
            this->vRightTop.z = FUtilString::ParserFloat(aParam[11]);
            this->vColor.x = FUtilString::ParserFloat(aParam[12]);
            this->vColor.y = FUtilString::ParserFloat(aParam[13]);
            this->vColor.z = FUtilString::ParserFloat(aParam[14]);
            this->vColor.w = FUtilString::ParserFloat(aParam[15]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineQuad3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineGrid3D
    String FMeshCreateParam_LineGrid3D::ms_nameType = "MeshLineGrid3D"; 
    bool FMeshCreateParam_LineGrid3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 14)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeftBottom.z = FUtilString::ParserFloat(aParam[5]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[7]);
            this->vRightBottom.z = FUtilString::ParserFloat(aParam[8]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[9]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[10]);
            this->vRightTop.z = FUtilString::ParserFloat(aParam[11]);
            this->m = FUtilString::ParserUInt(aParam[12]);
            this->n = FUtilString::ParserUInt(aParam[13]);
        }
        else if (count == 18)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeftBottom.z = FUtilString::ParserFloat(aParam[5]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[7]);
            this->vRightBottom.z = FUtilString::ParserFloat(aParam[8]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[9]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[10]);
            this->vRightTop.z = FUtilString::ParserFloat(aParam[11]);
            this->m = FUtilString::ParserUInt(aParam[12]);
            this->n = FUtilString::ParserUInt(aParam[13]);
            this->vColor.x = FUtilString::ParserFloat(aParam[14]);
            this->vColor.y = FUtilString::ParserFloat(aParam[15]);
            this->vColor.z = FUtilString::ParserFloat(aParam[16]);
            this->vColor.w = FUtilString::ParserFloat(aParam[17]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineGrid3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineCircle3D
    String FMeshCreateParam_LineCircle3D::ms_nameType = "MeshLineCircle3D"; 
    bool FMeshCreateParam_LineCircle3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 12)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vDir.x = FUtilString::ParserFloat(aParam[3]);
            this->vDir.y = FUtilString::ParserFloat(aParam[4]);
            this->vDir.z = FUtilString::ParserFloat(aParam[5]);
            this->vUp.x = FUtilString::ParserFloat(aParam[6]);
            this->vUp.y = FUtilString::ParserFloat(aParam[7]);
            this->vUp.z = FUtilString::ParserFloat(aParam[8]);
            this->radius = FUtilString::ParserFloat(aParam[9]);
            this->segment = FUtilString::ParserUInt(aParam[10]);
            this->isDrawCenter = FUtilString::ParserBool(aParam[11]);
        }
        else if (count == 16)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vDir.x = FUtilString::ParserFloat(aParam[3]);
            this->vDir.y = FUtilString::ParserFloat(aParam[4]);
            this->vDir.z = FUtilString::ParserFloat(aParam[5]);
            this->vUp.x = FUtilString::ParserFloat(aParam[6]);
            this->vUp.y = FUtilString::ParserFloat(aParam[7]);
            this->vUp.z = FUtilString::ParserFloat(aParam[8]);
            this->radius = FUtilString::ParserFloat(aParam[9]);
            this->segment = FUtilString::ParserUInt(aParam[10]);
            this->isDrawCenter = FUtilString::ParserBool(aParam[11]);
            this->vColor.x = FUtilString::ParserFloat(aParam[12]);
            this->vColor.y = FUtilString::ParserFloat(aParam[13]);
            this->vColor.z = FUtilString::ParserFloat(aParam[14]);
            this->vColor.w = FUtilString::ParserFloat(aParam[15]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineCircle3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineAABB3D    
    String FMeshCreateParam_LineAABB3D::ms_nameType = "MeshLineAABB3D"; 
    bool FMeshCreateParam_LineAABB3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 6)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vExtent.x = FUtilString::ParserFloat(aParam[3]);
            this->vExtent.y = FUtilString::ParserFloat(aParam[4]);
            this->vExtent.z = FUtilString::ParserFloat(aParam[5]);
        }
        else if (count == 10)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vExtent.x = FUtilString::ParserFloat(aParam[3]);
            this->vExtent.y = FUtilString::ParserFloat(aParam[4]);
            this->vExtent.z = FUtilString::ParserFloat(aParam[5]);
            this->vColor.x = FUtilString::ParserFloat(aParam[6]);
            this->vColor.y = FUtilString::ParserFloat(aParam[7]);
            this->vColor.z = FUtilString::ParserFloat(aParam[8]);
            this->vColor.w = FUtilString::ParserFloat(aParam[9]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineAABB3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineSphere3D
    String FMeshCreateParam_LineSphere3D::ms_nameType = "MeshLineSphere3D"; 
    bool FMeshCreateParam_LineSphere3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 9)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->sliceCount = FUtilString::ParserUInt(aParam[7]);
            this->stackCount = FUtilString::ParserUInt(aParam[8]);
        }
        else if (count == 13)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->sliceCount = FUtilString::ParserUInt(aParam[7]);
            this->stackCount = FUtilString::ParserUInt(aParam[8]);
            this->vColor.x = FUtilString::ParserFloat(aParam[9]);
            this->vColor.y = FUtilString::ParserFloat(aParam[10]);
            this->vColor.z = FUtilString::ParserFloat(aParam[11]);
            this->vColor.w = FUtilString::ParserFloat(aParam[12]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineSphere3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineCylinder3D
    String FMeshCreateParam_LineCylinder3D::ms_nameType = "MeshLineCylinder3D"; 
    bool FMeshCreateParam_LineCylinder3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 11)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radiusBottom = FUtilString::ParserFloat(aParam[6]);
            this->radiusTop = FUtilString::ParserFloat(aParam[7]);
            this->height = FUtilString::ParserFloat(aParam[8]);
            this->sliceCount = FUtilString::ParserUInt(aParam[9]);
            this->isDrawCenter = FUtilString::ParserBool(aParam[10]);
        }
        else if (count == 15)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radiusBottom = FUtilString::ParserFloat(aParam[6]);
            this->radiusTop = FUtilString::ParserFloat(aParam[7]);
            this->height = FUtilString::ParserFloat(aParam[8]);
            this->sliceCount = FUtilString::ParserUInt(aParam[9]);
            this->isDrawCenter = FUtilString::ParserBool(aParam[10]);
            this->vColor.x = FUtilString::ParserFloat(aParam[11]);
            this->vColor.y = FUtilString::ParserFloat(aParam[12]);
            this->vColor.z = FUtilString::ParserFloat(aParam[13]);
            this->vColor.w = FUtilString::ParserFloat(aParam[14]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineCylinder3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineCapsule3D
    String FMeshCreateParam_LineCapsule3D::ms_nameType = "MeshLineCapsule3D"; 
    bool FMeshCreateParam_LineCapsule3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 10)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->height = FUtilString::ParserFloat(aParam[7]);
            this->numRings = FUtilString::ParserUInt(aParam[8]);
            this->numSegments = FUtilString::ParserUInt(aParam[9]);
        }
        else if (count == 14)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->height = FUtilString::ParserFloat(aParam[7]);
            this->numRings = FUtilString::ParserUInt(aParam[8]);
            this->numSegments = FUtilString::ParserUInt(aParam[9]);
            this->vColor.x = FUtilString::ParserFloat(aParam[10]);
            this->vColor.y = FUtilString::ParserFloat(aParam[11]);
            this->vColor.z = FUtilString::ParserFloat(aParam[12]);
            this->vColor.w = FUtilString::ParserFloat(aParam[13]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineCapsule3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineCone3D
    String FMeshCreateParam_LineCone3D::ms_nameType = "MeshLineCone3D"; 
    bool FMeshCreateParam_LineCone3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 9)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->height = FUtilString::ParserFloat(aParam[7]);
            this->numSegBase = FUtilString::ParserUInt(aParam[8]);
        }
        else if (count == 13)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->height = FUtilString::ParserFloat(aParam[7]);
            this->numSegBase = FUtilString::ParserUInt(aParam[8]);
            this->vColor.x = FUtilString::ParserFloat(aParam[9]);
            this->vColor.y = FUtilString::ParserFloat(aParam[10]);
            this->vColor.z = FUtilString::ParserFloat(aParam[11]);
            this->vColor.w = FUtilString::ParserFloat(aParam[12]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineCone3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //LineTorus3D
    String FMeshCreateParam_LineTorus3D::ms_nameType = "MeshLineTorus3D"; 
    bool FMeshCreateParam_LineTorus3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 10)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->sectionRadius = FUtilString::ParserFloat(aParam[7]);
            this->numSegSection = FUtilString::ParserUInt(aParam[8]);
            this->numSegCircle = FUtilString::ParserUInt(aParam[9]);
        }
        else if (count == 14)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->sectionRadius = FUtilString::ParserFloat(aParam[7]);
            this->numSegSection = FUtilString::ParserUInt(aParam[8]);
            this->numSegCircle = FUtilString::ParserUInt(aParam[9]);
            this->vColor.x = FUtilString::ParserFloat(aParam[10]);
            this->vColor.y = FUtilString::ParserFloat(aParam[11]);
            this->vColor.z = FUtilString::ParserFloat(aParam[12]);
            this->vColor.w = FUtilString::ParserFloat(aParam[13]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_LineTorus3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }


    ////////////////////////////////// Flat3D ////////////////////////////////
    //FlatTriangle3D
    String FMeshCreateParam_FlatTriangle3D::ms_nameType = "MeshFlatTriangle3D"; 
    bool FMeshCreateParam_FlatTriangle3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 9)
        {
            this->vTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeft.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeft.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeft.z = FUtilString::ParserFloat(aParam[5]);
            this->vRight.x = FUtilString::ParserFloat(aParam[6]);
            this->vRight.y = FUtilString::ParserFloat(aParam[7]);
            this->vRight.z = FUtilString::ParserFloat(aParam[8]);
        }
        else if (count == 13)
        {
            this->vTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeft.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeft.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeft.z = FUtilString::ParserFloat(aParam[5]);
            this->vRight.x = FUtilString::ParserFloat(aParam[6]);
            this->vRight.y = FUtilString::ParserFloat(aParam[7]);
            this->vRight.z = FUtilString::ParserFloat(aParam[8]);
            this->vColor.x = FUtilString::ParserFloat(aParam[9]);
            this->vColor.y = FUtilString::ParserFloat(aParam[10]);
            this->vColor.z = FUtilString::ParserFloat(aParam[11]);
            this->vColor.w = FUtilString::ParserFloat(aParam[12]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatTriangle3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatQuad3D
    String FMeshCreateParam_FlatQuad3D::ms_nameType = "MeshFlatQuad3D"; 
    bool FMeshCreateParam_FlatQuad3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 12)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeftBottom.z = FUtilString::ParserFloat(aParam[5]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[7]);
            this->vRightBottom.z = FUtilString::ParserFloat(aParam[8]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[9]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[10]);
            this->vRightTop.z = FUtilString::ParserFloat(aParam[11]);
        }
        else if (count == 16)
        {
            this->vLeftTop.x = FUtilString::ParserFloat(aParam[0]);
            this->vLeftTop.y = FUtilString::ParserFloat(aParam[1]);
            this->vLeftTop.z = FUtilString::ParserFloat(aParam[2]);
            this->vLeftBottom.x = FUtilString::ParserFloat(aParam[3]);
            this->vLeftBottom.y = FUtilString::ParserFloat(aParam[4]);
            this->vLeftBottom.z = FUtilString::ParserFloat(aParam[5]);
            this->vRightBottom.x = FUtilString::ParserFloat(aParam[6]);
            this->vRightBottom.y = FUtilString::ParserFloat(aParam[7]);
            this->vRightBottom.z = FUtilString::ParserFloat(aParam[8]);
            this->vRightTop.x = FUtilString::ParserFloat(aParam[9]);
            this->vRightTop.y = FUtilString::ParserFloat(aParam[10]);
            this->vRightTop.z = FUtilString::ParserFloat(aParam[11]);
            this->vColor.x = FUtilString::ParserFloat(aParam[12]);
            this->vColor.y = FUtilString::ParserFloat(aParam[13]);
            this->vColor.z = FUtilString::ParserFloat(aParam[14]);
            this->vColor.w = FUtilString::ParserFloat(aParam[15]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatQuad3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatCircle3D
    String FMeshCreateParam_FlatCircle3D::ms_nameType = "MeshFlatCircle3D"; 
    bool FMeshCreateParam_FlatCircle3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 11)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vDir.x = FUtilString::ParserFloat(aParam[3]);
            this->vDir.y = FUtilString::ParserFloat(aParam[4]);
            this->vDir.z = FUtilString::ParserFloat(aParam[5]);
            this->vUp.x = FUtilString::ParserFloat(aParam[6]);
            this->vUp.y = FUtilString::ParserFloat(aParam[7]);
            this->vUp.z = FUtilString::ParserFloat(aParam[8]);
            this->radius = FUtilString::ParserFloat(aParam[9]);
            this->segment = FUtilString::ParserUInt(aParam[10]);
        }
        else if (count == 15)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vDir.x = FUtilString::ParserFloat(aParam[3]);
            this->vDir.y = FUtilString::ParserFloat(aParam[4]);
            this->vDir.z = FUtilString::ParserFloat(aParam[5]);
            this->vUp.x = FUtilString::ParserFloat(aParam[6]);
            this->vUp.y = FUtilString::ParserFloat(aParam[7]);
            this->vUp.z = FUtilString::ParserFloat(aParam[8]);
            this->radius = FUtilString::ParserFloat(aParam[9]);
            this->segment = FUtilString::ParserUInt(aParam[10]);
            this->vColor.x = FUtilString::ParserFloat(aParam[11]);
            this->vColor.y = FUtilString::ParserFloat(aParam[12]);
            this->vColor.z = FUtilString::ParserFloat(aParam[13]);
            this->vColor.w = FUtilString::ParserFloat(aParam[14]);
        }   
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatCircle3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatAABB3D
    String FMeshCreateParam_FlatAABB3D::ms_nameType = "MeshFlatAABB3D"; 
    bool FMeshCreateParam_FlatAABB3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 6)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vExtent.x = FUtilString::ParserFloat(aParam[3]);
            this->vExtent.y = FUtilString::ParserFloat(aParam[4]);
            this->vExtent.z = FUtilString::ParserFloat(aParam[5]);
        }
        else if (count == 10)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vExtent.x = FUtilString::ParserFloat(aParam[3]);
            this->vExtent.y = FUtilString::ParserFloat(aParam[4]);
            this->vExtent.z = FUtilString::ParserFloat(aParam[5]);
            this->vColor.x = FUtilString::ParserFloat(aParam[6]);
            this->vColor.y = FUtilString::ParserFloat(aParam[7]);
            this->vColor.z = FUtilString::ParserFloat(aParam[8]);
            this->vColor.w = FUtilString::ParserFloat(aParam[9]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatAABB3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatSphere3D
    String FMeshCreateParam_FlatSphere3D::ms_nameType = "MeshFlatSphere3D"; 
    bool FMeshCreateParam_FlatSphere3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 9)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->sliceCount = FUtilString::ParserUInt(aParam[7]);
            this->stackCount = FUtilString::ParserUInt(aParam[8]);
        }
        else if (count == 13)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->sliceCount = FUtilString::ParserUInt(aParam[7]);
            this->stackCount = FUtilString::ParserUInt(aParam[8]);
            this->vColor.x = FUtilString::ParserFloat(aParam[9]);
            this->vColor.y = FUtilString::ParserFloat(aParam[10]);
            this->vColor.z = FUtilString::ParserFloat(aParam[11]);
            this->vColor.w = FUtilString::ParserFloat(aParam[12]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatSphere3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatCylinder3D
    String FMeshCreateParam_FlatCylinder3D::ms_nameType = "MeshFlatCylinder3D"; 
    bool FMeshCreateParam_FlatCylinder3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 11)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radiusBottom = FUtilString::ParserFloat(aParam[6]);
            this->radiusTop = FUtilString::ParserFloat(aParam[7]);
            this->height = FUtilString::ParserFloat(aParam[8]);
            this->sliceCount = FUtilString::ParserUInt(aParam[9]);
            this->stackCount = FUtilString::ParserUInt(aParam[10]);
        }
        else if (count == 15)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radiusBottom = FUtilString::ParserFloat(aParam[6]);
            this->radiusTop = FUtilString::ParserFloat(aParam[7]);
            this->height = FUtilString::ParserFloat(aParam[8]);
            this->sliceCount = FUtilString::ParserUInt(aParam[9]);
            this->stackCount = FUtilString::ParserUInt(aParam[10]);
            this->vColor.x = FUtilString::ParserFloat(aParam[11]);
            this->vColor.y = FUtilString::ParserFloat(aParam[12]);
            this->vColor.z = FUtilString::ParserFloat(aParam[13]);
            this->vColor.w = FUtilString::ParserFloat(aParam[14]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatCylinder3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatCapsule3D
    String FMeshCreateParam_FlatCapsule3D::ms_nameType = "MeshFlatCapsule3D"; 
    bool FMeshCreateParam_FlatCapsule3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 11)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->height = FUtilString::ParserFloat(aParam[7]);
            this->numRings = FUtilString::ParserUInt(aParam[8]);
            this->numSegments = FUtilString::ParserUInt(aParam[9]);
            this->numSegHeight = FUtilString::ParserUInt(aParam[10]);
        }
        else if (count == 15)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->height = FUtilString::ParserFloat(aParam[7]);
            this->numRings = FUtilString::ParserUInt(aParam[8]);
            this->numSegments = FUtilString::ParserUInt(aParam[9]);
            this->numSegHeight = FUtilString::ParserUInt(aParam[10]);
            this->vColor.x = FUtilString::ParserFloat(aParam[11]);
            this->vColor.y = FUtilString::ParserFloat(aParam[12]);
            this->vColor.z = FUtilString::ParserFloat(aParam[13]);
            this->vColor.w = FUtilString::ParserFloat(aParam[14]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatCapsule3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatCone3D
    String FMeshCreateParam_FlatCone3D::ms_nameType = "MeshFlatCone3D"; 
    bool FMeshCreateParam_FlatCone3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 10)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->height = FUtilString::ParserFloat(aParam[7]);
            this->numSegBase = FUtilString::ParserUInt(aParam[8]);
            this->numSegHeight = FUtilString::ParserUInt(aParam[9]);
        }
        else if (count == 14)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->height = FUtilString::ParserFloat(aParam[7]);
            this->numSegBase = FUtilString::ParserUInt(aParam[8]);
            this->numSegHeight = FUtilString::ParserUInt(aParam[9]);
            this->vColor.x = FUtilString::ParserFloat(aParam[10]);
            this->vColor.y = FUtilString::ParserFloat(aParam[11]);
            this->vColor.z = FUtilString::ParserFloat(aParam[12]);
            this->vColor.w = FUtilString::ParserFloat(aParam[13]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatCone3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //FlatTorus3D
    String FMeshCreateParam_FlatTorus3D::ms_nameType = "MeshFlatTorus3D"; 
    bool FMeshCreateParam_FlatTorus3D::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 10)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->sectionRadius = FUtilString::ParserFloat(aParam[7]);
            this->numSegSection = FUtilString::ParserUInt(aParam[8]);
            this->numSegCircle = FUtilString::ParserUInt(aParam[9]);
        }
        else if (count == 14)
        {
            this->vCenter.x = FUtilString::ParserFloat(aParam[0]);
            this->vCenter.y = FUtilString::ParserFloat(aParam[1]);
            this->vCenter.z = FUtilString::ParserFloat(aParam[2]);
            this->vUp.x = FUtilString::ParserFloat(aParam[3]);
            this->vUp.y = FUtilString::ParserFloat(aParam[4]);
            this->vUp.z = FUtilString::ParserFloat(aParam[5]);
            this->radius = FUtilString::ParserFloat(aParam[6]);
            this->sectionRadius = FUtilString::ParserFloat(aParam[7]);
            this->numSegSection = FUtilString::ParserUInt(aParam[8]);
            this->numSegCircle = FUtilString::ParserUInt(aParam[9]);
            this->vColor.x = FUtilString::ParserFloat(aParam[10]);
            this->vColor.y = FUtilString::ParserFloat(aParam[11]);
            this->vColor.z = FUtilString::ParserFloat(aParam[12]);
            this->vColor.w = FUtilString::ParserFloat(aParam[13]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_FlatTorus3D::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }


    ////////////////////////////////// Entity ////////////////////////////////
    //EntityTriangle
    String FMeshCreateParam_EntityTriangle::ms_nameType = "MeshEntityTriangle"; 
    bool FMeshCreateParam_EntityTriangle::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 2)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityTriangle::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntityQuad
    String FMeshCreateParam_EntityQuad::ms_nameType = "MeshEntityQuad"; 
    bool FMeshCreateParam_EntityQuad::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 7)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->centerX = FUtilString::ParserFloat(aParam[2]);
            this->centerY = FUtilString::ParserFloat(aParam[3]);
            this->width = FUtilString::ParserFloat(aParam[4]);
            this->height = FUtilString::ParserFloat(aParam[5]);
            this->depth = FUtilString::ParserFloat(aParam[6]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityQuad::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }
    void FMeshCreateParam_EntityQuad::RefreshVertexPos()
    {
        this->vLeftTop = FVector3(this->centerX - this->width/2, this->centerY + this->height/2, this->depth);
        this->vLeftBottom = FVector3(this->centerX - this->width/2, this->centerY - this->height/2, this->depth);
        this->vRightBottom = FVector3(this->centerX + this->width/2, this->centerY - this->height/2, this->depth);
        this->vRightTop = FVector3(this->centerX + this->width/2, this->centerY + this->height/2, this->depth);
    }

    //EntityGrid   
    String FMeshCreateParam_EntityGrid::ms_nameType = "MeshEntityGrid"; 
    bool FMeshCreateParam_EntityGrid::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 6)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->width = FUtilString::ParserFloat(aParam[2]);
            this->height = FUtilString::ParserFloat(aParam[3]);
            this->m = FUtilString::ParserUInt(aParam[4]);
            this->n = FUtilString::ParserUInt(aParam[5]);
        }   
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityGrid::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }
    void FMeshCreateParam_EntityGrid::RefreshVertexPos()
    {
        this->vLeftTop = FVector3(-this->width/2, this->height/2, 0);
        this->vLeftBottom = FVector3(-this->width/2, - this->height/2, 0);
        this->vRightBottom = FVector3(this->width/2, - this->height/2, 0);
        this->vRightTop = FVector3(this->width/2, this->height/2, 0);
    }

    //EntityCircle
    String FMeshCreateParam_EntityCircle::ms_nameType = "MeshEntityCircle";  
    bool FMeshCreateParam_EntityCircle::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 4)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->radius = FUtilString::ParserFloat(aParam[2]);
            this->segment = FUtilString::ParserUInt(aParam[3]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityCircle::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntityAABB
    String FMeshCreateParam_EntityAABB::ms_nameType = "MeshEntityAABB"; 
    bool FMeshCreateParam_EntityAABB::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 6)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->width = FUtilString::ParserFloat(aParam[2]);
            this->height = FUtilString::ParserFloat(aParam[3]);
            this->depth = FUtilString::ParserFloat(aParam[4]);
            this->numSubdivisions = FUtilString::ParserUInt(aParam[5]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityAABB::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntitySphere
    String FMeshCreateParam_EntitySphere::ms_nameType = "MeshEntitySphere"; 
    bool FMeshCreateParam_EntitySphere::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 5)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->radius = FUtilString::ParserFloat(aParam[2]);
            this->sliceCount = FUtilString::ParserUInt(aParam[3]);
            this->stackCount = FUtilString::ParserUInt(aParam[4]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntitySphere::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntityGeoSphere
    String FMeshCreateParam_EntityGeoSphere::ms_nameType = "MeshEntityGeoSphere"; 
    bool FMeshCreateParam_EntityGeoSphere::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 4)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->radius = FUtilString::ParserFloat(aParam[2]);
            this->numSubdivisions = FUtilString::ParserUInt(aParam[3]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityGeoSphere::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntityCylinder
    String FMeshCreateParam_EntityCylinder::ms_nameType = "MeshEntityCylinder"; 
    bool FMeshCreateParam_EntityCylinder::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 8)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->radiusBottom = FUtilString::ParserFloat(aParam[2]);
            this->radiusTop = FUtilString::ParserFloat(aParam[3]);
            this->height = FUtilString::ParserFloat(aParam[4]);
            this->heightOffset = FUtilString::ParserFloat(aParam[5]);
            this->sliceCount = FUtilString::ParserUInt(aParam[6]);
            this->stackCount = FUtilString::ParserUInt(aParam[7]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityCylinder::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntityCapsule
    String FMeshCreateParam_EntityCapsule::ms_nameType = "MeshEntityCapsule"; 
    bool FMeshCreateParam_EntityCapsule::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 8)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->radius = FUtilString::ParserFloat(aParam[2]);
            this->height = FUtilString::ParserFloat(aParam[3]);
            this->heightOffset = FUtilString::ParserFloat(aParam[4]);
            this->numRings = FUtilString::ParserUInt(aParam[5]);
            this->numSegments = FUtilString::ParserUInt(aParam[6]);
            this->numSegHeight = FUtilString::ParserUInt(aParam[7]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityCapsule::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntityCone
    String FMeshCreateParam_EntityCone::ms_nameType = "MeshEntityCone"; 
    bool FMeshCreateParam_EntityCone::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 7)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->radius = FUtilString::ParserFloat(aParam[2]);
            this->height = FUtilString::ParserFloat(aParam[3]);
            this->heightOffset = FUtilString::ParserFloat(aParam[4]);
            this->numSegBase = FUtilString::ParserUInt(aParam[5]);
            this->numSegHeight = FUtilString::ParserUInt(aParam[6]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityCone::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntityTorus
    String FMeshCreateParam_EntityTorus::ms_nameType = "MeshEntityTorus"; 
    bool FMeshCreateParam_EntityTorus::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 6)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
            this->radius = FUtilString::ParserFloat(aParam[2]);
            this->sectionRadius = FUtilString::ParserFloat(aParam[3]);
            this->numSegSection = FUtilString::ParserUInt(aParam[4]);
            this->numSegCircle = FUtilString::ParserUInt(aParam[5]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityTorus::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntitySkyBox
    String FMeshCreateParam_EntitySkyBox::ms_nameType = "MeshEntitySkyBox"; 
    bool FMeshCreateParam_EntitySkyBox::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 10)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntitySkyBox::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntitySkyDome
    String FMeshCreateParam_EntitySkyDome::ms_nameType = "MeshEntitySkyDome"; 
    bool FMeshCreateParam_EntitySkyDome::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 10)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntitySkyDome::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }

    //EntityTerrain
    String FMeshCreateParam_EntityTerrain::ms_nameType = "MeshEntityTerrain"; 
    bool FMeshCreateParam_EntityTerrain::ParseParam(const String& nameParam)
    {
        StringVector aParam = FUtilString::Split(nameParam, ";");
        size_t count = aParam.size();
        if (count == 2)
        {
            this->flipV = FUtilString::ParserBool(aParam[0]);
            this->rightHand = FUtilString::ParserBool(aParam[1]);
        }
        else
        {
            F_Assert(false && "FMeshCreateParam_EntityTerrain::ParseParam: Wrong param count !");
            return false;
        }

        return true;
    }
    

    ///////////////////////////////////////// FMeshGeometry //////////////////////////////////////////////
    FMeshCreateParam* FMeshGeometry::CreateParam(FMeshGeometryType typeMeshGeometry)
    {
        FMeshCreateParam* pParam = nullptr;
        switch ((int)typeMeshGeometry)
        {
        ////////////////////////////////// Line2D ////////////////////////////////
            case F_MeshGeometry_LineLine2D:         { pParam = new FMeshCreateParam_LineLine2D(); return pParam; }          //LineLine2D
            case F_MeshGeometry_LineTriangle2D:     { pParam = new FMeshCreateParam_LineTriangle2D(); return pParam; }      //LineTriangle2D
            case F_MeshGeometry_LineQuad2D:         { pParam = new FMeshCreateParam_LineQuad2D(); return pParam; }          //LineQuad2D
            case F_MeshGeometry_LineGrid2D:         { pParam = new FMeshCreateParam_LineGrid2D(); return pParam; }          //LineGrid2D
            case F_MeshGeometry_LineCircle2D:       { pParam = new FMeshCreateParam_LineCircle2D(); return pParam; }        //LineCircle2D

        ////////////////////////////////// Flat2D ////////////////////////////////
            case F_MeshGeometry_FlatTriangle2D:     { pParam = new FMeshCreateParam_FlatTriangle2D(); return pParam; }      //FlatTriangle2D
            case F_MeshGeometry_FlatQuad2D:         { pParam = new FMeshCreateParam_FlatQuad2D(); return pParam; }          //FlatQuad2D
            case F_MeshGeometry_FlatCircle2D:       { pParam = new FMeshCreateParam_FlatCircle2D(); return pParam; }        //FlatCircle2D

        ////////////////////////////////// Line3D ////////////////////////////////
            case F_MeshGeometry_LineLine3D:         { pParam = new FMeshCreateParam_LineLine3D(); return pParam; }          //LineLine3D
            case F_MeshGeometry_LineTriangle3D:     { pParam = new FMeshCreateParam_LineTriangle3D(); return pParam; }      //LineTriangle3D
            case F_MeshGeometry_LineQuad3D:         { pParam = new FMeshCreateParam_LineQuad3D(); return pParam; }          //LineQuad3D
            case F_MeshGeometry_LineGrid3D:         { pParam = new FMeshCreateParam_LineGrid3D(); return pParam; }          //LineGrid3D
            case F_MeshGeometry_LineCircle3D:       { pParam = new FMeshCreateParam_LineCircle3D(); return pParam; }        //LineCircle3D
            case F_MeshGeometry_LineAABB3D:         { pParam = new FMeshCreateParam_LineAABB3D(); return pParam; }          //LineAABB3D
            case F_MeshGeometry_LineSphere3D:       { pParam = new FMeshCreateParam_LineSphere3D(); return pParam; }        //LineSphere3D
            case F_MeshGeometry_LineCylinder3D:     { pParam = new FMeshCreateParam_LineCylinder3D(); return pParam; }      //LineCylinder3D
            case F_MeshGeometry_LineCapsule3D:      { pParam = new FMeshCreateParam_LineCapsule3D(); return pParam; }       //LineCapsule3D
            case F_MeshGeometry_LineCone3D:         { pParam = new FMeshCreateParam_LineCone3D(); return pParam; }          //LineCone3D
            case F_MeshGeometry_LineTorus3D:        { pParam = new FMeshCreateParam_LineTorus3D(); return pParam; }         //LineTorus3D

        ////////////////////////////////// Flat3D ////////////////////////////////
            case F_MeshGeometry_FlatTriangle3D:     { pParam = new FMeshCreateParam_FlatTriangle3D(); return pParam; }      //FlatTriangle3D
            case F_MeshGeometry_FlatQuad3D:         { pParam = new FMeshCreateParam_FlatQuad3D(); return pParam; }          //FlatQuad3D
            case F_MeshGeometry_FlatCircle3D:       { pParam = new FMeshCreateParam_FlatCircle3D(); return pParam; }        //FlatCircle3D
            case F_MeshGeometry_FlatAABB3D:         { pParam = new FMeshCreateParam_FlatAABB3D(); return pParam; }          //FlatAABB3D
            case F_MeshGeometry_FlatSphere3D:       { pParam = new FMeshCreateParam_FlatSphere3D(); return pParam; }        //FlatSphere3D
            case F_MeshGeometry_FlatCylinder3D:     { pParam = new FMeshCreateParam_FlatCylinder3D(); return pParam; }      //FlatCylinder3D
            case F_MeshGeometry_FlatCapsule3D:      { pParam = new FMeshCreateParam_FlatCapsule3D(); return pParam; }       //FlatCapsule3D
            case F_MeshGeometry_FlatCone3D:         { pParam = new FMeshCreateParam_FlatCone3D(); return pParam; }          //FlatCone3D
            case F_MeshGeometry_FlatTorus3D:        { pParam = new FMeshCreateParam_FlatTorus3D(); return pParam; }         //FlatTorus3D

        ////////////////////////////////// Entity ////////////////////////////////
            case F_MeshGeometry_EntityTriangle:     { pParam = new FMeshCreateParam_EntityTriangle(); return pParam; }      //EntityTriangle
            case F_MeshGeometry_EntityQuad:         { pParam = new FMeshCreateParam_EntityQuad(); return pParam; }          //EntityQuad
            case F_MeshGeometry_EntityGrid:         { pParam = new FMeshCreateParam_EntityGrid(); return pParam; }          //EntityGrid
            case F_MeshGeometry_EntityCircle:       { pParam = new FMeshCreateParam_EntityCircle(); return pParam; }        //EntityCircle
            case F_MeshGeometry_EntityAABB:         { pParam = new FMeshCreateParam_EntityAABB(); return pParam; }          //EntityAABB
            case F_MeshGeometry_EntitySphere:       { pParam = new FMeshCreateParam_EntitySphere(); return pParam; }        //EntitySphere
            case F_MeshGeometry_EntityGeoSphere:    { pParam = new FMeshCreateParam_EntityGeoSphere(); return pParam; }     //EntityGeoSphere
            case F_MeshGeometry_EntityCylinder:     { pParam = new FMeshCreateParam_EntityCylinder(); return pParam; }      //EntityCylinder
            case F_MeshGeometry_EntityCapsule:      { pParam = new FMeshCreateParam_EntityCapsule(); return pParam; }       //EntityCapsule
            case F_MeshGeometry_EntityCone:         { pParam = new FMeshCreateParam_EntityCone(); return pParam; }          //EntityCone
            case F_MeshGeometry_EntityTorus:        { pParam = new FMeshCreateParam_EntityTorus(); return pParam; }         //EntityTorus
            case F_MeshGeometry_EntitySkyBox:       { pParam = new FMeshCreateParam_EntitySkyBox(); return pParam; }        //EntitySkyBox
            case F_MeshGeometry_EntitySkyDome:      { pParam = new FMeshCreateParam_EntitySkyDome(); return pParam; }       //EntitySkyDome
            case F_MeshGeometry_EntityTerrain:      { pParam = new FMeshCreateParam_EntityTerrain(); return pParam; }       //EntityTerrain

        default:
            F_Assert(false && "FMeshGeometry::CreateParam: Wrong geometry type !");
        }
        return pParam;
    }
    FMeshCreateParam* FMeshGeometry::CreateParam(FMeshGeometryType typeMeshGeometry, const String& nameParam)
    {   
        FMeshCreateParam* pParam = CreateParam(typeMeshGeometry);
        if (pParam == nullptr)
            return nullptr;
        pParam->ParseParam(nameParam);
        return pParam; 
    }

    bool FMeshGeometry::CreateGeometry(FMeshDataPC* pMeshDataPC, FMeshData* pMeshData, FMeshGeometryType typeMeshGeometry)
    {
        if (F_IsMeshGeometryType_Entity(typeMeshGeometry))
        {
            F_Assert(pMeshData != nullptr && "FMeshGeometry::CreateGeometry: CreateEntityGeometry");
            return CreateEntityGeometry(*pMeshData, typeMeshGeometry);
        }
        else if (F_IsMeshGeometryType_Flat3D(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometry: CreateFlat3DGeometry");
            return CreateFlat3DGeometry(*pMeshDataPC, typeMeshGeometry);
        }
        else if (F_IsMeshGeometryType_Line3D(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometry: CreateLine3DGeometry");
            return CreateLine3DGeometry(*pMeshDataPC, typeMeshGeometry);
        }
        else if (F_IsMeshGeometryType_Flat2D(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometry: CreateFlat2DGeometry");
            return CreateFlat2DGeometry(*pMeshDataPC, typeMeshGeometry);
        }
        else if (F_IsMeshGeometryType_Line2D(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometry: CreateLine2DGeometry");
            return CreateLine2DGeometry(*pMeshDataPC, typeMeshGeometry);
        }
        
        F_Assert(false && "FMeshGeometry::CreateGeometry: Wrong MeshGeometry type !");
        return false;
    }
	bool FMeshGeometry::CreateGeometryWithParam(FMeshDataPC* pMeshDataPC, FMeshData* pMeshData, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {
        if (F_IsMeshGeometryType_Entity(typeMeshGeometry))
        {
            F_Assert(pMeshData != nullptr && "FMeshGeometry::CreateGeometryWithParam: CreateEntityGeometryWithParam");
            return CreateEntityGeometryWithParam(*pMeshData, typeMeshGeometry, pParam);
        }
        else if (F_IsMeshGeometryType_Flat3D(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometryWithParam: CreateFlat3DGeometryWithParam");
            return CreateFlat3DGeometryWithParam(*pMeshDataPC, typeMeshGeometry, pParam);
        }
        else if (F_IsMeshGeometryType_Line3D(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometryWithParam: CreateLine3DGeometryWithParam");
            return CreateLine3DGeometryWithParam(*pMeshDataPC, typeMeshGeometry, pParam);
        }
        else if (F_IsMeshGeometryType_Flat2D(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometryWithParam: CreateFlat2DGeometryWithParam");
            return CreateFlat2DGeometryWithParam(*pMeshDataPC, typeMeshGeometry, pParam);
        }
        else if (F_IsMeshGeometryType_Line2D(typeMeshGeometry))
        {
            F_Assert(pMeshDataPC != nullptr && "FMeshGeometry::CreateGeometryWithParam: CreateLine2DGeometryWithParam");
            return CreateLine2DGeometryWithParam(*pMeshDataPC, typeMeshGeometry, pParam);
        }
        
        F_Assert(false && "FMeshGeometry::CreateGeometryWithParam: Wrong MeshGeometry type !");
        return false;
    }


    ////////////////////////////////// Line2D ////////////////////////////////
    bool FMeshGeometry::CreateLine2DGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry)
    {
        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_LineLine2D:
            {
                FMeshCreateParam_LineLine2D param_LineLine2D;
                FMeshGeometry::CreateLineLine2D(meshDataPC, &param_LineLine2D);
                return true;
            }
        case F_MeshGeometry_LineTriangle2D:
            {
                FMeshCreateParam_LineTriangle2D param_LineTriangle2D;
                FMeshGeometry::CreateLineTriangle2D(meshDataPC, &param_LineTriangle2D);
                return true;
            }
        case F_MeshGeometry_LineQuad2D:
            {
                FMeshCreateParam_LineQuad2D param_LineQuad2D;
                FMeshGeometry::CreateLineQuad2D(meshDataPC, &param_LineQuad2D);
                return true;
            }
        case F_MeshGeometry_LineGrid2D:
            {
                FMeshCreateParam_LineGrid2D param_LineGrid2D;
                FMeshGeometry::CreateLineGrid2D(meshDataPC, &param_LineGrid2D);
                return true;
            }
        case F_MeshGeometry_LineCircle2D:
            {
                FMeshCreateParam_LineCircle2D param_LineCircle2D;
                FMeshGeometry::CreateLineCircle2D(meshDataPC, &param_LineCircle2D);
                return true;
            }
        }
        return false;
    }
	bool FMeshGeometry::CreateLine2DGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {
        if (pParam == nullptr)
        {
            return CreateLine2DGeometry(meshDataPC, typeMeshGeometry);
        }

        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_LineLine2D:
            {
                FMeshCreateParam_LineLine2D* pParam_LineLine2D = static_cast<FMeshCreateParam_LineLine2D*>(pParam);
                FMeshGeometry::CreateLineLine2D(meshDataPC, pParam_LineLine2D);
                return true;
            }
        case F_MeshGeometry_LineTriangle2D:
            {
                FMeshCreateParam_LineTriangle2D* pParam_LineTriangle2D = static_cast<FMeshCreateParam_LineTriangle2D*>(pParam);
                FMeshGeometry::CreateLineTriangle2D(meshDataPC, pParam_LineTriangle2D);
                return true;
            }
        case F_MeshGeometry_LineQuad2D:
            {
                FMeshCreateParam_LineQuad2D* pParam_LineQuad2D = static_cast<FMeshCreateParam_LineQuad2D*>(pParam);
                FMeshGeometry::CreateLineQuad2D(meshDataPC, pParam_LineQuad2D);
                return true;
            }
        case F_MeshGeometry_LineGrid2D:
            {
                FMeshCreateParam_LineGrid2D* pParam_LineGrid2D = static_cast<FMeshCreateParam_LineGrid2D*>(pParam);
                FMeshGeometry::CreateLineGrid2D(meshDataPC, pParam_LineGrid2D);
                return true;
            }
        case F_MeshGeometry_LineCircle2D:
            {
                FMeshCreateParam_LineCircle2D* pParam_LineCircle2D = static_cast<FMeshCreateParam_LineCircle2D*>(pParam);
                FMeshGeometry::CreateLineCircle2D(meshDataPC, pParam_LineCircle2D);
                return true;
            }
        }
        return false;
    }

    //LineLine2D
    void FMeshGeometry::CreateLineLine2D(FMeshDataPC& meshDataPC,
                                         const FVector2& vStart,
                                         const FVector2& vEnd,
                                         const FVector4& vColor)
    {
        // 0 ----- 1

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vStart.x, vStart.y, 0.0f), vColor));   //0
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vEnd.x, vEnd.y, 0.0f), vColor));       //1

        //Index
        AddIndexLine(meshDataPC, 0, 1);
    }

    //LineTriangle2D
    void FMeshGeometry::CreateLineTriangle2D(FMeshDataPC& meshDataPC,
                                             const FVector2& vTop,
                                             const FVector2& vLeft,
                                             const FVector2& vRight,
                                             const FVector4& vColor)
    {
        //        0 
        //        /\
        //       /  \
        //    1 ------ 2

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vTop.x, vTop.y, 0.0f), vColor));       //0
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vLeft.x, vLeft.y, 0.0f), vColor));     //1
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vRight.x, vRight.y, 0.0f), vColor));   //2
        
        //Index
        AddIndexLine(meshDataPC, 0, 1);
        AddIndexLine(meshDataPC, 1, 2);
        AddIndexLine(meshDataPC, 2, 0);
    }

    //LineQuad2D
    void FMeshGeometry::CreateLineQuad2D(FMeshDataPC& meshDataPC,
                                         const FVector2& vLeftTop,
                                         const FVector2& vLeftBottom,
                                         const FVector2& vRightBottom,
                                         const FVector2& vRightTop,
                                         const FVector4& vColor)
    {
        //  0       3
        //   --------
        //   |      |
        //   |      |
        //   |      |
        //   --------
        //  1        2

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vLeftTop.x, vLeftTop.y, 0.0f), vColor));           //0
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vLeftBottom.x, vLeftBottom.y, 0.0f), vColor));     //1
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vRightBottom.x, vRightBottom.y, 0.0f), vColor));   //2
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vRightTop.x, vRightTop.y, 0.0f), vColor));         //3

        //Index
        AddIndexLine(meshDataPC, 0, 1);
        AddIndexLine(meshDataPC, 1, 2);
        AddIndexLine(meshDataPC, 2, 3);
        AddIndexLine(meshDataPC, 3, 0);
    }

    //LineGrid2D
    void FMeshGeometry::CreateLineGrid2D(FMeshDataPC& meshDataPC,
                                         const FVector2& vLeftTop,
                                         const FVector2& vLeftBottom,
                                         const FVector2& vRightBottom,
                                         const FVector2& vRightTop,
                                         const FVector4& vColor,
                                         uint32 m,
                                         uint32 n)
    {
        // m >= 2, n >= 2
        //
        // (n-1)*(m-2)                  (n-1)*(m-1)
        //      |      |      |
        //      |      |      |
        //      |      |      |
        //  n*2 ----------------------- (n-1)*3
        //      |      |      |
        //      |      |      |
        //      |      |      |
        //  n   ----------------------- (n-1)*2
        //      |      |(n+1) |(n+2)
        //      |      |      |
        //      |      |      |
        //      -----------------------
        //  0          1      2         (n-1)

        //Vertex
        FVector3 vRB(vRightBottom.x, vRightBottom.y, 0.0f);
        FVector3 vLB(vLeftBottom.x, vLeftBottom.y, 0.0f);
        FVector3 vLT(vLeftTop.x, vLeftTop.y, 0.0f);
        FVector3 vDirX = FMath::Normalize(vRB - vLB);
        FVector3 vDirY = FMath::Normalize(vLT - vLB);
        float fDisX = FMath::Distance(vRB, vLB);
        float fDisY = FMath::Distance(vLT, vLB);
        float dx = fDisX / (n - 1);
        float dy = fDisY / (m - 1);
        //Bottom/Top
        for (uint32 i = 0; i < n; ++i)
        {
            FVector3 vX = vLB + vDirX * (dx * i);
            for (uint32 j = 0; j < 2; j++)
            {
                FVector3 vPos = vX + vDirY * (fDisY * j);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));
            }   
        }
        //Left/Right
        for (uint32 i = 0; i < m; i++)
        {
            FVector3 vY = vLB + vDirY * (dy * i);
            for (uint32 j = 0; j < 2; j ++)
            {
                FVector3 vPos = vY + vDirX * (fDisX * j);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));
            }
        }
        
        //Index
        uint nVertexCount = n * 2 + m * 2;
        for (uint32 i = 0; i < nVertexCount / 2; ++i)
        {
            AddIndexLine(meshDataPC, 2 * i, 2 * i + 1);
        }
    }

    //LineCircle2D
    void FMeshGeometry::CreateLineCircle2D(FMeshDataPC& meshDataPC,
                                           const FVector2& vCenter,
                                           const FVector2& vDir,
                                           const FVector4& vColor,
                                           float radius,
                                           int32 viewWidth,
                                           int32 viewHeight,
                                           uint32 segment,
                                           bool isDrawCenter)
    {
        //          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *
        
        bool isW = false;
        bool isH = false;
        float fPerf = 1.0f;
        if (viewWidth >= viewHeight)
        {
            isW = true;
            fPerf = (float)viewHeight / viewWidth;
        }
        else
        {
            isH = true;
            fPerf = (float)viewWidth / viewHeight;
        }

        //Vertex
        FVector3 vC(vCenter.x, vCenter.y, 0.0f);
        FVector3 vD(vDir.x, vDir.y, 0.0f);
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        for (uint32 i = 0; i < segment; i++)
        {
            FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(thetaStep * i, - FMath::ms_v3UnitZ);
            FVector3 vCur = FMath::Transform(qRot, vD);
            vCur *= radius;
            if (isW)
            {
                vCur.x *= fPerf;
            }
            if (isH)
            {
                vCur.y *= fPerf;
            }
            FVector3 vPos = vC + vCur;
            AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));
        }
        if (isDrawCenter)
        {
            AddVertex(meshDataPC, FMeshVertexPC(vC, vColor));
        }

        //Index
        for (uint32 i = 0; i < segment; ++i)    
        {
            if (i != segment - 1)
            {
                AddIndexLine(meshDataPC, i, i + 1);
            }
            else
            {
                AddIndexLine(meshDataPC, i, 0);
            }

            if (isDrawCenter)
            {
                AddIndexLine(meshDataPC, i, segment);
            }
        }
    }
    void FMeshGeometry::UpdateLineCircle2D(FMeshDataPC& meshDataPC, 
                                           const FVector2& vCenter,
                                           const FVector2& vDir,
                                           const FVector4& vColor,
                                           float radius,
                                           int32 viewWidth,
                                           int32 viewHeight,
                                           uint32 segment,
                                           bool isDrawCenter)
    {
        //          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *
        
        uint32 vertexCount = segment;
        if (isDrawCenter)
            vertexCount += 1;
        uint32 count = meshDataPC.GetVertexCount();
        if (count != vertexCount)
        {
            F_Assert(false && "FMeshGeometry::UpdateLineCircle2D: Wrong vertex count !");
            return;
        }

        bool isW = false;
        bool isH = false;
        float fPerf = 1.0f;
        if (viewWidth >= viewHeight)
        {
            isW = true;
            fPerf = (float)viewHeight / viewWidth;
        }
        else
        {
            isH = true;
            fPerf = (float)viewWidth / viewHeight;
        }

        //Vertex
        FVector3 vC(vCenter.x, vCenter.y, 0.0f);
        FVector3 vD(vDir.x, vDir.y, 0.0f);
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        for (uint32 i = 0; i < segment; i++)
        {
            FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(thetaStep * i, - FMath::ms_v3UnitZ);
            FVector3 vCur = FMath::Transform(qRot, vD);
            vCur *= radius;
            if (isW)
            {
                vCur.x *= fPerf;
            }
            if (isH)
            {
                vCur.y *= fPerf;
            }
            FVector3 vPos = vC + vCur;

            FMeshVertexPC& vertexPC = GetVertex(meshDataPC, i);
            vertexPC.pos = vPos;
            vertexPC.color = vColor;
        }
        if (isDrawCenter)
        {
            FMeshVertexPC& vertexPC = GetVertex(meshDataPC, segment);
            vertexPC.pos = vC;
            vertexPC.color = vColor;
        }
    }
    void FMeshGeometry::UpdateLineCircle2D(FMeshVertexPCVector& aVertexPC, 
                                           const FVector2& vCenter,
                                           const FVector2& vDir,
                                           const FVector4& vColor,
                                           float radius,
                                           int32 viewWidth,
                                           int32 viewHeight,
                                           uint32 segment,
                                           bool isDrawCenter)
    {
        //          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *
        
        uint32 vertexCount = segment;
        if (isDrawCenter)
            vertexCount += 1;
        uint32 count = (uint32)aVertexPC.size();
        if (count != vertexCount)
        {
            F_Assert(false && "FMeshGeometry::UpdateLineCircle2D: Wrong vertex count !");
            return;
        }

        bool isW = false;
        bool isH = false;
        float fPerf = 1.0f;
        if (viewWidth >= viewHeight)
        {
            isW = true;
            fPerf = (float)viewHeight / viewWidth;
        }
        else
        {
            isH = true;
            fPerf = (float)viewWidth / viewHeight;
        }

        //Vertex
        FVector3 vC(vCenter.x, vCenter.y, 0.0f);
        FVector3 vD(vDir.x, vDir.y, 0.0f);
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        for (uint32 i = 0; i < segment; i++)
        {
            FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(thetaStep * i, - FMath::ms_v3UnitZ);
            FVector3 vCur = FMath::Transform(qRot, vD);
            vCur *= radius;
            if (isW)
            {
                vCur.x *= fPerf;
            }
            if (isH)
            {
                vCur.y *= fPerf;
            }
            FVector3 vPos = vC + vCur;

            FMeshVertexPC& vertexPC = aVertexPC[i];
            vertexPC.pos = vPos;
            vertexPC.color = vColor;
        }
        if (isDrawCenter)
        {
            FMeshVertexPC& vertexPC = aVertexPC[segment];
            vertexPC.pos = vC;
            vertexPC.color = vColor;
        }
    }

    ////////////////////////////////// Flat2D ////////////////////////////////
    bool FMeshGeometry::CreateFlat2DGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry)
    {
        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_FlatTriangle2D:
            {
                FMeshCreateParam_FlatTriangle2D param_FlatTriangle2D;
                FMeshGeometry::CreateFlatTriangle2D(meshDataPC, &param_FlatTriangle2D);
                return true;
            }
        case F_MeshGeometry_FlatQuad2D:
            {
                FMeshCreateParam_FlatQuad2D param_FlatQuad2D;
                FMeshGeometry::CreateFlatQuad2D(meshDataPC, &param_FlatQuad2D);
                return true;
            }
        case F_MeshGeometry_FlatCircle2D:
            {
                FMeshCreateParam_FlatCircle2D param_FlatCircle2D;
                FMeshGeometry::CreateFlatCircle2D(meshDataPC, &param_FlatCircle2D);
                return true;
            }
        }
        return false;
    }
	bool FMeshGeometry::CreateFlat2DGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {
        if (pParam == nullptr)
        {
            return CreateFlat2DGeometry(meshDataPC, typeMeshGeometry);
        }

        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_FlatTriangle2D:
            {
                FMeshCreateParam_FlatTriangle2D* pParam_FlatTriangle2D = static_cast<FMeshCreateParam_FlatTriangle2D*>(pParam);
                FMeshGeometry::CreateFlatTriangle2D(meshDataPC, pParam_FlatTriangle2D);
                return true;
            }
        case F_MeshGeometry_FlatQuad2D:
            {
                FMeshCreateParam_FlatQuad2D* pParam_FlatQuad2D = static_cast<FMeshCreateParam_FlatQuad2D*>(pParam);
                FMeshGeometry::CreateFlatQuad2D(meshDataPC, pParam_FlatQuad2D);
                return true;
            }
        case F_MeshGeometry_FlatCircle2D:
            {
                FMeshCreateParam_FlatCircle2D* pParam_FlatCircle2D = static_cast<FMeshCreateParam_FlatCircle2D*>(pParam);
                FMeshGeometry::CreateFlatCircle2D(meshDataPC, pParam_FlatCircle2D);
                return true;
            }
        }
        return false;
    }

    //FlatTriangle2D
    void FMeshGeometry::CreateFlatTriangle2D(FMeshDataPC& meshDataPC,
                                             const FVector2& vTop,
                                             const FVector2& vLeft,
                                             const FVector2& vRight,
                                             const FVector4& vColor)
    {
        //        0 
        //        /\
        //       /  \
        //    1 ------ 2

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vTop.x, vTop.y, 0.0f), vColor));       //0
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vLeft.x, vLeft.y, 0.0f), vColor));     //1
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vRight.x, vRight.y, 0.0f), vColor));   //2
        
        //Index
        AddIndexTriangle(meshDataPC, 0, 2, 1);        
    }

    //FlatQuad2D
    void FMeshGeometry::CreateFlatQuad2D(FMeshDataPC& meshDataPC,
                                         const FVector2& vLeftTop,
                                         const FVector2& vLeftBottom,
                                         const FVector2& vRightBottom,
                                         const FVector2& vRightTop,
                                         const FVector4& vColor)
    {
        //  0       3
        //   --------
        //   |      |
        //   |      |
        //   |      |
        //   --------
        //  1        2

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vLeftTop.x, vLeftTop.y, 0.0f), vColor));           //0
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vLeftBottom.x, vLeftBottom.y, 0.0f), vColor));     //1
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vRightBottom.x, vRightBottom.y, 0.0f), vColor));   //2
        AddVertex(meshDataPC, FMeshVertexPC(FVector3(vRightTop.x, vRightTop.y, 0.0f), vColor));         //3

        //Index
        AddIndexTriangle(meshDataPC, 0, 3, 2);
        AddIndexTriangle(meshDataPC, 2, 1, 0);
    }

    //FlatCircle2D
    void FMeshGeometry::CreateFlatCircle2D(FMeshDataPC& meshDataPC,
                                           const FVector2& vCenter,
                                           const FVector2& vDir,
                                           const FVector4& vColor,
                                           float radius,
                                           int32 viewWidth,
                                           int32 viewHeight,
                                           uint32 segment)
    {
        //          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *

        //Vertex
        FVector3 vC(vCenter.x, vCenter.y, 0.0f);
        FVector3 vD(vDir.x, vDir.y, 0.0f);
        uint32 vertexCount = segment + 1;
        uint32 faceCount = segment;
        bool isW = false;
        bool isH = false;
        float fPerf = 1.0f;
        if (viewWidth >= viewHeight)
        {
            isW = true;
            fPerf = (float)viewHeight / viewWidth;
        }
        else
        {
            isH = true;
            fPerf = (float)viewWidth / viewHeight;
        }
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        ResizeVertexCount(meshDataPC, vertexCount);
        SetVertex(meshDataPC, 0, FMeshVertexPC(vC, vColor));
        int index = 1;
        for (uint32 i = 0; i < segment; i++)
        {
            FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(thetaStep * i, -FMath::ms_v3UnitZ);
            FVector3 vCur = FMath::Transform(qRot, vD);
            vCur *= radius;
            if (isW)
            {
                vCur.x *= fPerf;
            }
            if (isH)
            {
                vCur.y *= fPerf;
            }
            FVector3 vPos = vC + vCur;
            SetVertex(meshDataPC, index, FMeshVertexPC(vPos, vColor));
            index++;
        }
        meshDataPC.RefreshSphereAndAABB(vC);

        //Index
        ResizeIndexCount(meshDataPC, faceCount * 3);
        for (uint32 i = 0; i < segment; ++i)    
        {
            if (i != segment - 1)
            {
                SetIndexTriangle(meshDataPC, i * 3 + 0, 0, i + 2, i + 1);
            }
            else
            {
                SetIndexTriangle(meshDataPC, i * 3 + 0, 0, 1, i);
            }
        }
    }
    void FMeshGeometry::UpdateFlatCircle2D(FMeshDataPC& meshDataPC, 
                                           const FVector2& vCenter,
                                           const FVector2& vDir,
                                           const FVector4& vColor,
                                           float radius,
                                           int32 viewWidth,
                                           int32 viewHeight,
                                           uint32 segment)
    {
        //          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *

        //Vertex
        uint32 vertexCount = segment + 1;
        uint32 count = meshDataPC.GetVertexCount();
        if (count != vertexCount)
        {
            F_Assert(false && "FMeshGeometry::UpdateFlatCircle2D: Wrong vertex count !");
            return;
        }

        FVector3 vC(vCenter.x, vCenter.y, 0.0f);
        FVector3 vD(vDir.x, vDir.y, 0.0f);
        
        uint32 faceCount = segment;
        bool isW = false;
        bool isH = false;
        float fPerf = 1.0f;
        if (viewWidth >= viewHeight)
        {
            isW = true;
            fPerf = (float)viewHeight / viewWidth;
        }
        else
        {
            isH = true;
            fPerf = (float)viewWidth / viewHeight;
        }
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        FMeshVertexPC& vertexPC_C = GetVertex(meshDataPC, 0);
        vertexPC_C.pos = vC;
        vertexPC_C.color = vColor;
        int index = 1;
        for (uint32 i = 0; i < segment; i++)
        {
            FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(thetaStep * i, -FMath::ms_v3UnitZ);
            FVector3 vCur = FMath::Transform(qRot, vD);
            vCur *= radius;
            if (isW)
            {
                vCur.x *= fPerf;
            }
            if (isH)
            {
                vCur.y *= fPerf;
            }
            FVector3 vPos = vC + vCur;

            FMeshVertexPC& vertexPC = GetVertex(meshDataPC, index);
            vertexPC.pos = vPos;
            vertexPC.color = vColor;

            index++;
        }
        meshDataPC.RefreshSphereAndAABB(vC);
    }
    void FMeshGeometry::UpdateFlatCircle2D(FMeshVertexPCVector& aVertexPC, 
                                           const FVector2& vCenter,
                                           const FVector2& vDir,
                                           const FVector4& vColor,
                                           float radius,
                                           int32 viewWidth,
                                           int32 viewHeight,
                                           uint32 segment)
    {
        //          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *

        //Vertex
        uint32 vertexCount = segment + 1;
        uint32 count = (uint32)aVertexPC.size();
        if (count != vertexCount)
        {
            F_Assert(false && "FMeshGeometry::UpdateFlatCircle2D: Wrong vertex count !");
            return;
        }

        FVector3 vC(vCenter.x, vCenter.y, 0.0f);
        FVector3 vD(vDir.x, vDir.y, 0.0f);
        
        uint32 faceCount = segment;
        bool isW = false;
        bool isH = false;
        float fPerf = 1.0f;
        if (viewWidth >= viewHeight)
        {
            isW = true;
            fPerf = (float)viewHeight / viewWidth;
        }
        else
        {
            isH = true;
            fPerf = (float)viewWidth / viewHeight;
        }
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        FMeshVertexPC& vertexPC_C = aVertexPC[0];
        vertexPC_C.pos = vC;
        vertexPC_C.color = vColor;
        int index = 1;
        for (uint32 i = 0; i < segment; i++)
        {
            FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(thetaStep * i, -FMath::ms_v3UnitZ);
            FVector3 vCur = FMath::Transform(qRot, vD);
            vCur *= radius;
            if (isW)
            {
                vCur.x *= fPerf;
            }
            if (isH)
            {
                vCur.y *= fPerf;
            }
            FVector3 vPos = vC + vCur;

            FMeshVertexPC& vertexPC = aVertexPC[index];
            vertexPC.pos = vPos;
            vertexPC.color = vColor;

            index++;
        }
    }

    ////////////////////////////////// Line3D ////////////////////////////////
    bool FMeshGeometry::CreateLine3DGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry)
    {
        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_LineLine3D:
            {
                FMeshCreateParam_LineLine3D param_LineLine3D;
                FMeshGeometry::CreateLineLine3D(meshDataPC, &param_LineLine3D);
                return true;
            }
        case F_MeshGeometry_LineTriangle3D:
            {
                FMeshCreateParam_LineTriangle3D param_LineTriangle3D;
                FMeshGeometry::CreateLineTriangle3D(meshDataPC, &param_LineTriangle3D);
                return true;
            }
        case F_MeshGeometry_LineQuad3D:
            {
                FMeshCreateParam_LineQuad3D param_LineQuad3D;
                FMeshGeometry::CreateLineQuad3D(meshDataPC, &param_LineQuad3D);
                return true;
            }
        case F_MeshGeometry_LineGrid3D:
            {
                FMeshCreateParam_LineGrid3D param_LineGrid3D;
                FMeshGeometry::CreateLineGrid3D(meshDataPC, &param_LineGrid3D);
                return true;
            }
        case F_MeshGeometry_LineCircle3D:
            {
                FMeshCreateParam_LineCircle3D param_LineCircle3D;
                FMeshGeometry::CreateLineCircle3D(meshDataPC, &param_LineCircle3D);
                return true;
            }
        case F_MeshGeometry_LineAABB3D:
            {
                FMeshCreateParam_LineAABB3D param_LineAABB3D;
                FMeshGeometry::CreateLineAABB3D(meshDataPC, &param_LineAABB3D);
                return true;
            }
        case F_MeshGeometry_LineSphere3D:
            {
                FMeshCreateParam_LineSphere3D param_LineSphere3D;
                FMeshGeometry::CreateLineSphere3D(meshDataPC, &param_LineSphere3D);
                return true;
            }
        case F_MeshGeometry_LineCylinder3D:
            {
                FMeshCreateParam_LineCylinder3D param_LineCylinder3D;
                FMeshGeometry::CreateLineCylinder3D(meshDataPC, &param_LineCylinder3D);
                return true;
            }
        case F_MeshGeometry_LineCapsule3D:
            {
                FMeshCreateParam_LineCapsule3D param_LineCapsule3D;
                FMeshGeometry::CreateLineCapsule3D(meshDataPC, &param_LineCapsule3D);
                return true;
            }
        case F_MeshGeometry_LineCone3D:
            {
                FMeshCreateParam_LineCone3D param_LineCone3D;
                FMeshGeometry::CreateLineCone3D(meshDataPC, &param_LineCone3D);
                return true;
            }
        case F_MeshGeometry_LineTorus3D:
            {
                FMeshCreateParam_LineTorus3D param_LineTorus3D;
                FMeshGeometry::CreateLineTorus3D(meshDataPC, &param_LineTorus3D);
                return true;
            }
        }
        return false;
    }
	bool FMeshGeometry::CreateLine3DGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {
        if (pParam == nullptr)
        {
            return CreateLine3DGeometry(meshDataPC, typeMeshGeometry);
        }

        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_LineLine3D:
            {
                FMeshCreateParam_LineLine3D* pParam_LineLine3D = static_cast<FMeshCreateParam_LineLine3D*>(pParam);
                FMeshGeometry::CreateLineLine3D(meshDataPC, pParam_LineLine3D);
                return true;
            }
        case F_MeshGeometry_LineTriangle3D:
            {
                FMeshCreateParam_LineTriangle3D* pParam_LineTriangle3D = static_cast<FMeshCreateParam_LineTriangle3D*>(pParam);
                FMeshGeometry::CreateLineTriangle3D(meshDataPC, pParam_LineTriangle3D);
                return true;
            }
        case F_MeshGeometry_LineQuad3D:
            {
                FMeshCreateParam_LineQuad3D* pParam_LineQuad3D = static_cast<FMeshCreateParam_LineQuad3D*>(pParam);
                FMeshGeometry::CreateLineQuad3D(meshDataPC, pParam_LineQuad3D);
                return true;
            }
        case F_MeshGeometry_LineGrid3D:
            {
                FMeshCreateParam_LineGrid3D* pParam_LineGrid3D = static_cast<FMeshCreateParam_LineGrid3D*>(pParam);
                FMeshGeometry::CreateLineGrid3D(meshDataPC, pParam_LineGrid3D);
                return true;
            }
        case F_MeshGeometry_LineCircle3D:
            {
                FMeshCreateParam_LineCircle3D* pParam_LineCircle3D = static_cast<FMeshCreateParam_LineCircle3D*>(pParam);
                FMeshGeometry::CreateLineCircle3D(meshDataPC, pParam_LineCircle3D);
                return true;
            }
        case F_MeshGeometry_LineAABB3D:
            {
                FMeshCreateParam_LineAABB3D* pParam_LineAABB3D = static_cast<FMeshCreateParam_LineAABB3D*>(pParam);
                FMeshGeometry::CreateLineAABB3D(meshDataPC, pParam_LineAABB3D);
                return true;
            }
        case F_MeshGeometry_LineSphere3D:
            {
                FMeshCreateParam_LineSphere3D* pParam_LineSphere3D = static_cast<FMeshCreateParam_LineSphere3D*>(pParam);
                FMeshGeometry::CreateLineSphere3D(meshDataPC, pParam_LineSphere3D);
                return true;
            }
        case F_MeshGeometry_LineCylinder3D:
            {
                FMeshCreateParam_LineCylinder3D* pParam_LineCylinder3D = static_cast<FMeshCreateParam_LineCylinder3D*>(pParam);
                FMeshGeometry::CreateLineCylinder3D(meshDataPC, pParam_LineCylinder3D);
                return true;
            }
        case F_MeshGeometry_LineCapsule3D:
            {
                FMeshCreateParam_LineCapsule3D* pParam_LineCapsule3D = static_cast<FMeshCreateParam_LineCapsule3D*>(pParam);
                FMeshGeometry::CreateLineCapsule3D(meshDataPC, pParam_LineCapsule3D);
                return true;
            }
        case F_MeshGeometry_LineCone3D:
            {
                FMeshCreateParam_LineCone3D* pParam_LineCone3D = static_cast<FMeshCreateParam_LineCone3D*>(pParam);
                FMeshGeometry::CreateLineCone3D(meshDataPC, pParam_LineCone3D);
                return true;
            }
        case F_MeshGeometry_LineTorus3D:
            {
                FMeshCreateParam_LineTorus3D* pParam_LineTorus3D = static_cast<FMeshCreateParam_LineTorus3D*>(pParam);
                FMeshGeometry::CreateLineTorus3D(meshDataPC, pParam_LineTorus3D);
                return true;
            }
        }
        return false;
    }

    //LineLine3D
    void FMeshGeometry::CreateLineLine3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vStart,
                                         const FVector3& vEnd,
                                         const FVector4& vColor)
    {
        // 0 ----- 1

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(vStart, vColor));   //0
        AddVertex(meshDataPC, FMeshVertexPC(vEnd, vColor));     //1

        //Index
        AddIndexLine(meshDataPC, 0, 1);
    }

    //LineTriangle3D
    void FMeshGeometry::CreateLineTriangle3D(FMeshDataPC& meshDataPC,
                                             const FVector3& vTop,
                                             const FVector3& vLeft,
                                             const FVector3& vRight,
                                             const FVector4& vColor)
    {
        //        0 
        //        /\
        //       /  \
        //    1 ------ 2

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(vTop, vColor));     //0
        AddVertex(meshDataPC, FMeshVertexPC(vLeft, vColor));    //1
        AddVertex(meshDataPC, FMeshVertexPC(vRight, vColor));   //2
        
        //Index
        AddIndexLine(meshDataPC, 0, 1);
        AddIndexLine(meshDataPC, 1, 2);
        AddIndexLine(meshDataPC, 2, 0);
    }

    //LineQuad3D
    void FMeshGeometry::CreateLineQuad3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vLeftTop,
                                         const FVector3& vLeftBottom,
                                         const FVector3& vRightBottom,
                                         const FVector3& vRightTop,
                                         const FVector4& vColor)
    {
        //  0       3
        //   --------
        //   |      |
        //   |      |
        //   |      |
        //   --------
        //  1        2

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(vLeftTop, vColor));     //0
        AddVertex(meshDataPC, FMeshVertexPC(vLeftBottom, vColor));  //1
        AddVertex(meshDataPC, FMeshVertexPC(vRightBottom, vColor)); //2
        AddVertex(meshDataPC, FMeshVertexPC(vRightTop, vColor));    //3

        //Index
        AddIndexLine(meshDataPC, 0, 1);
        AddIndexLine(meshDataPC, 1, 2);
        AddIndexLine(meshDataPC, 2, 3);
        AddIndexLine(meshDataPC, 3, 0);
    }

    //LineGrid3D
    void FMeshGeometry::CreateLineGrid3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vLeftTop,
                                         const FVector3& vLeftBottom,
                                         const FVector3& vRightBottom,
                                         const FVector3& vRightTop,
                                         const FVector4& vColor,
                                         uint32 m,
                                         uint32 n)
    {
        // m >= 2, n >= 2
        //
        // (n-1)*(m-2)                  (n-1)*(m-1)
        //      |      |      |
        //      |      |      |
        //      |      |      |
        //  n*2 ----------------------- (n-1)*3
        //      |      |      |
        //      |      |      |
        //      |      |      |
        //  n   ----------------------- (n-1)*2
        //      |      |(n+1) |(n+2)
        //      |      |      |
        //      |      |      |
        //      -----------------------
        //  0          1      2         (n-1)

        //Vertex
        FVector3 vDirX = FMath::Normalize(vRightBottom - vLeftBottom);
        FVector3 vDirY = FMath::Normalize(vLeftTop - vLeftBottom);
        float fDisX = FMath::Distance(vRightBottom, vLeftBottom);
        float fDisY = FMath::Distance(vLeftTop, vLeftBottom);
        float dx = fDisX / (n - 1);
        float dy = fDisY / (m - 1);
        //Bottom/Top
        for (uint32 i = 0; i < n; ++i)
        {
            FVector3 vX = vLeftBottom + vDirX * (dx * i);
            for (uint32 j = 0; j < 2; j++)
            {
                FVector3 vPos = vX + vDirY * (fDisY * j);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));
            }   
        }
        //Left/Right
        for (uint32 i = 0; i < m; i++)
        {
            FVector3 vY = vLeftBottom + vDirY * (dy * i);
            for (uint32 j = 0; j < 2; j ++)
            {
                FVector3 vPos = vY + vDirX * (fDisX * j);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));
            }
        }
        
        //Index
        uint nVertexCount = n * 2 + m * 2;
        for (uint32 i = 0; i < nVertexCount / 2; ++i)
        {
            AddIndexLine(meshDataPC, 2 * i, 2 * i + 1);
        }
    }

    //LineCircle3D
    void FMeshGeometry::CreateLineCircle3D(FMeshDataPC& meshDataPC,
                                           const FVector3& vCenter,
                                           const FVector3& vDir,
                                           const FVector3& vUp,
                                           const FVector4& vColor,
                                           float radius,
                                           uint32 segment,
                                           bool isDrawCenter)
    {
        //          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *
        
        uint32 vertexCount = segment + 1;
        uint32 faceCount = segment;

        //Vertex
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        for (uint32 i = 0; i < segment; i++)
        {
            FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(thetaStep * i, vUp);
            FVector3 vCur = FMath::Transform(qRot, vDir);
            FVector3 vPos = vCenter + vCur * radius;
            AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));
        }
        if (isDrawCenter)
        {
            AddVertex(meshDataPC, FMeshVertexPC(vCenter, vColor));
        }

        //Index
        for (uint32 i = 0; i < segment; ++i)    
        {
            if (i != segment - 1)
            {
                AddIndexLine(meshDataPC, i, i + 1);
            }
            else
            {
                AddIndexLine(meshDataPC, i, 0);
            }

            if (isDrawCenter)
            {
                AddIndexLine(meshDataPC, i, segment);
            }
        }
    }

    //LineAABB3D
    void FMeshGeometry::CreateLineAABB3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vCenter,
                                         const FVector3& vExtent,
                                         const FVector4& vColor)
    {
        //     7+------+4			  0: -+-
		//     /|     /|			  1: ---
		//    / |    / |			  2: +--
		//   / 6+---/--+5	 y		  3: ++-
		// 0+------+3 /		 | z	  4: +++
		//  | /    | /    	 |/		  5: +-+
		//  |/     |/     	 *---x	  6: --+
		// 1+------+2        		  7: -++

        //Vertex
        FVector3 vX = FMath::ms_v3UnitX * vExtent.x;
        FVector3 vY = FMath::ms_v3UnitY * vExtent.y;
        FVector3 vZ = FMath::ms_v3UnitZ * vExtent.z;

        AddVertex(meshDataPC, FMeshVertexPC(vCenter - vX + vY - vZ, vColor)); //0 -+-
        AddVertex(meshDataPC, FMeshVertexPC(vCenter - vX - vY - vZ, vColor)); //1 ---
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vX - vY - vZ, vColor)); //2 +--
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vX + vY - vZ, vColor)); //3 ++-
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vX + vY + vZ, vColor)); //4 +++
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vX - vY + vZ, vColor)); //5 +-+
        AddVertex(meshDataPC, FMeshVertexPC(vCenter - vX - vY + vZ, vColor)); //6 --+
        AddVertex(meshDataPC, FMeshVertexPC(vCenter - vX + vY + vZ, vColor)); //7 -++

        //Index
        AddIndexLine(meshDataPC, 0, 1);
        AddIndexLine(meshDataPC, 1, 2);
        AddIndexLine(meshDataPC, 2, 3);
        AddIndexLine(meshDataPC, 3, 0);
        AddIndexLine(meshDataPC, 4, 5);
        AddIndexLine(meshDataPC, 5, 6);
        AddIndexLine(meshDataPC, 6, 7);
        AddIndexLine(meshDataPC, 7, 4);
        AddIndexLine(meshDataPC, 0, 7);
        AddIndexLine(meshDataPC, 1, 6);
        AddIndexLine(meshDataPC, 2, 5);
        AddIndexLine(meshDataPC, 3, 4);
    }

    //LineSphere3D
    void FMeshGeometry::CreateLineSphere3D(FMeshDataPC& meshDataPC,
                                           const FVector3& vCenter,
                                           const FVector3& vUp,
                                           const FVector4& vColor,
                                           float radius,
                                           uint32 sliceCount,
                                           uint32 stackCount)
    {
        //           0 
        //           *   
	    //	    *    - 2  *  1          
	    //        -     -          y                   
	    //   *	     *       *     | z              
	    //	      -     - l-2      |/
	    //      *    -    *  l-1   *---x
	    //           *
        //           l=(stackCount-1)*(sliceCount+1)+1

        //Vertex
        //Vertex Top
        FMeshVertexPC vertexTop(vCenter + vUp * radius, vColor);
        AddVertex(meshDataPC, vertexTop);
        {
            FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
            //Vertex
            float phiStep = FMath::ms_fPI / stackCount;
            float thetaStep = 2.0f * FMath::ms_fPI / sliceCount;
            for (uint32 i = 1; i <= stackCount - 1; ++i)
            {
                float phi = i * phiStep;
                for (uint32 j = 0; j <= sliceCount; ++j)
                {
                    float theta = j * thetaStep;
                    FVector3 vPos(radius * sinf(phi) * cosf(theta),
                                  radius * cosf(phi),
                                  radius * sinf(phi) * sinf(theta));
                    vPos = vCenter + FMath::Transform(qRot, vPos);
                    FMeshVertexPC vertex(vPos, vColor);
                    AddVertex(meshDataPC, vertex);
                }
            }
        }
        //Vertex Bottom
        FMeshVertexPC vertexBottom(vCenter - vUp * radius, vColor);
        AddVertex(meshDataPC, vertexBottom);

        //Index Top
        for (uint32 i = 1; i <= sliceCount; ++i)
        {
            AddIndexLine(meshDataPC, 0, i);
        }

        //Index Inner
        uint32 baseIndex = 1;
        uint32 ringVertexCount = sliceCount + 1;
        for (uint32 i = 0; i < stackCount - 2; ++i)
        {
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                AddIndexLine(meshDataPC, baseIndex + i * ringVertexCount + j, baseIndex + i * ringVertexCount + j + 1);
                AddIndexLine(meshDataPC, baseIndex + i * ringVertexCount + j, baseIndex + (i + 1) * ringVertexCount + j);
            }
        }

        //Index Bottom
        uint32 southPoleIndex = GetVertexCount(meshDataPC) - 1;
        baseIndex = southPoleIndex - ringVertexCount;
        for (uint32 i = 0; i <= sliceCount; ++i)
        {
            AddIndexLine(meshDataPC, southPoleIndex, baseIndex + i);
        }
    }

    //LineCylinder3D
    void FMeshGeometry::CreateLineCylinder3D(FMeshDataPC& meshDataPC,
                                             const FVector3& vCenter,
                                             const FVector3& vUp,
                                             const FVector4& vColor,
                                             float radiusBottom,
                                             float radiusTop,
                                             float height,
                                             uint32 sliceCount,
                                             bool isDrawCenter)
    {
        //       * 
        //    *     *
        //    *  *  *
        //    *     *
        //    *     *
        //    *     *
        //    *  *  *
        //    *     *
        //       *       

        //Vertex
        FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
        float dTheta = 2.0f * FMath::ms_fPI / sliceCount;
        //Bottom 
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            float y = - 0.5f * height;
            float r = radiusBottom;
            float c = cosf(i * dTheta);
            float s = sinf(i * dTheta);
            FVector3 vPos(r * c, y, r * s);
            vPos = vCenter +  FMath::Transform(qRot, vPos);
            FMeshVertexPC vertex(vPos, vColor);
            AddVertex(meshDataPC, vertex);
        }
        //Top
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            float y = 0.5f * height;
            float r = radiusBottom;
            float c = cosf(i * dTheta);
            float s = sinf(i * dTheta);
            FVector3 vPos(r * c, y, r * s);
            vPos = vCenter +  FMath::Transform(qRot, vPos);
            AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));
        }
        if (isDrawCenter)
        {
            //sliceCount * 2 + 0
            AddVertex(meshDataPC, FMeshVertexPC(vCenter - vUp * (0.5f * height), vColor)); //Bottom

            //sliceCount * 2 + 1
            AddVertex(meshDataPC, FMeshVertexPC(vCenter + vUp * (0.5f * height), vColor)); //Top
        }

        //Index
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (i == sliceCount - 1)
            {
                AddIndexLine(meshDataPC, i, 0);
                AddIndexLine(meshDataPC, sliceCount + i, sliceCount);
            }
            else
            {
                AddIndexLine(meshDataPC, i, i + 1);
                AddIndexLine(meshDataPC, sliceCount + i, sliceCount + i + 1);
            }
            AddIndexLine(meshDataPC, i, sliceCount + i);

            if (isDrawCenter)
            {
                AddIndexLine(meshDataPC, sliceCount * 2, i);
                AddIndexLine(meshDataPC, sliceCount * 2 + 1, sliceCount + i);
            }
        }
    }

    //LineCapsule3D
    void FMeshGeometry::CreateLineCapsule3D(FMeshDataPC& meshDataPC,
                                            const FVector3& vCenter,
                                            const FVector3& vUp,
                                            const FVector4& vColor,
                                            float radius,
                                            float height,
                                            uint32 numRings,
                                            uint32 numSegments)
    {
        //      *** 
        //    *  *  *
        //    *  *  *
        //    *     *
        //    *     *
        //    *     *
        //    *  *  *
        //    *  *  *
        //      ***  

        //Vertex
        FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
        float fDeltaRingAngle = (FMath::ms_fPI_Half / numRings);
        float fDeltaSegAngle = (FMath::ms_fPI_Two / numSegments);
        float sphereRatio = radius / (2 * radius + height);
        float cylinderRatio = height / (2 * radius + height);

        //1> Top half sphere
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf(ring * fDeltaRingAngle);
            float y0 = radius * cosf(ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 vPos(x0, +0.5f * height + y0, z0);
                vPos = vCenter +  FMath::Transform(qRot, vPos);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));

                if (seg != numSegments)
                {
                    AddIndexLine(meshDataPC, ring * (numSegments + 1) + seg, ring * (numSegments + 1) + seg + 1);
                }
                if (ring > 0)
                {
                    AddIndexLine(meshDataPC, (ring - 1) * (numSegments + 1) + seg, ring * (numSegments + 1) + seg);
                }
            }
        }

        //2> Bottom half sphere
        int offset = (numRings + 1) * (numSegments + 1);
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf (FMath::ms_fPI_Half + ring * fDeltaRingAngle);
            float y0 =  radius * cosf (FMath::ms_fPI_Half + ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 vPos(x0, -0.5f * height + y0, z0);
                vPos = vCenter +  FMath::Transform(qRot, vPos);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));

                if (seg != numSegments)
                {
                    AddIndexLine(meshDataPC, offset + ring * (numSegments + 1) + seg, offset + ring * (numSegments + 1) + seg + 1);
                }
                if (ring > 0)
                {
                    AddIndexLine(meshDataPC, offset + (ring - 1) * (numSegments + 1) + seg, offset + ring * (numSegments + 1) + seg);
                }
            } 
        } 

        //Index
        int topIndex = offset - (numSegments + 1);
        int bottomIndex = offset;
        for (uint32 ring = 0; ring <= numSegments; ring++)
        {
            AddIndexLine(meshDataPC, topIndex + ring, bottomIndex + ring);
        }
    }

    //LineCone3D
    void FMeshGeometry::CreateLineCone3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vCenter,
                                         const FVector3& vUp,
                                         const FVector4& vColor,
                                         float radius,
                                         float height,
                                         uint32 numSegBase)
    {
        //               *
        //             *   *
        //           *       *
        //         *           *
        //       *       +       *
        //     *   +           +   *
        //    *          *           *
        //      +                  +
        //          +          +
        //               +

        //Vertex
        FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
        float deltaAngle = (FMath::ms_fPI_Two / numSegBase);
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vUp * height, vColor));
        int offset = 1;
        for (uint32 i = 0; i <= numSegBase; i++)
        {
            float x0 = radius * cosf(i * deltaAngle);
            float z0 = radius * sinf(i * deltaAngle);
            FVector3 vPos(x0, 0, z0);
            vPos = vCenter +  FMath::Transform(qRot, vPos);
            AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));

            //Index
            AddIndexLine(meshDataPC, 0, offset + i);
            if (i != numSegBase)
            {
                AddIndexLine(meshDataPC, offset + i, offset + i + 1);
            }
        }
        AddVertex(meshDataPC, FMeshVertexPC(vCenter, vColor));

        //Index
        for (uint32 i = 0; i < numSegBase; i++)
        {
            AddIndexLine(meshDataPC, offset + i, offset + numSegBase + 1);
        }
    }

    //LineTorus3D
    void FMeshGeometry::CreateLineTorus3D(FMeshDataPC& meshDataPC,
                                          const FVector3& vCenter,
                                          const FVector3& vUp,
                                          const FVector4& vColor,
                                          float radius,
                                          float sectionRadius,
                                          uint32 numSegSection,
                                          uint32 numSegCircle)
    {
        //         + +
        //       + * *  +  
        //     + * - -  *  +
        //    + * -    - * +
        //     + * - -  *  + 
        //       + * *  +
        //         +  +

        //Vertex
        FQuaternion qRotUpAxis = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
        float deltaSection = (FMath::ms_fPI_Two / numSegSection);
        float deltaCircle = (FMath::ms_fPI_Two / numSegCircle);
        for (uint32 i = 0; i <= numSegCircle; i++)
        {   
            for (uint32 j = 0; j<= numSegSection; j++)
            {
                FVector3 c0(radius, 0.0, 0.0);
                FVector3 v0(radius + sectionRadius * cosf(j * deltaSection), sectionRadius * sinf(j * deltaSection), 0.0);
                FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(i * deltaCircle, FMath::ms_v3UnitY);
                FVector3 vPos = FMath::Transform(qRot, v0);
                vPos = vCenter +  FMath::Transform(qRotUpAxis, vPos);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));

                //Index
                if (i != numSegCircle)
                {
                    if (j != numSegSection)
                    {
                        AddIndexLine(meshDataPC, i * (numSegSection + 1) + j, i * (numSegSection + 1) + j + 1);
                    }
                    
                    AddIndexLine(meshDataPC, i * (numSegSection + 1) + j, (i + 1) * (numSegSection + 1) + j);
                }
            }
        }    
    }


    ////////////////////////////////// Flat3D ////////////////////////////////
    bool FMeshGeometry::CreateFlat3DGeometry(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry)
    {
        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_FlatTriangle3D:
            {
                FMeshCreateParam_FlatTriangle3D param_FlatTriangle3D;
                FMeshGeometry::CreateFlatTriangle3D(meshDataPC, &param_FlatTriangle3D);
                return true;
            }
        case F_MeshGeometry_FlatQuad3D:
            {
                FMeshCreateParam_FlatQuad3D param_FlatQuad3D;
                FMeshGeometry::CreateFlatQuad3D(meshDataPC, &param_FlatQuad3D);
                return true;
            }
        case F_MeshGeometry_FlatCircle3D:
            {
                FMeshCreateParam_FlatCircle3D param_FlatCircle3D;
                FMeshGeometry::CreateFlatCircle3D(meshDataPC, &param_FlatCircle3D);
                return true;
            }
        case F_MeshGeometry_FlatAABB3D:
            {
                FMeshCreateParam_FlatAABB3D param_FlatAABB3D;
                FMeshGeometry::CreateFlatAABB3D(meshDataPC, &param_FlatAABB3D);
                return true;
            }
        case F_MeshGeometry_FlatSphere3D:
            {
                FMeshCreateParam_FlatSphere3D param_FlatSphere3D;
                FMeshGeometry::CreateFlatSphere3D(meshDataPC, &param_FlatSphere3D);
                return true;
            }
        case F_MeshGeometry_FlatCylinder3D:
            {
                FMeshCreateParam_FlatCylinder3D param_FlatCylinder3D;
                FMeshGeometry::CreateFlatCylinder3D(meshDataPC, &param_FlatCylinder3D);
                return true;
            }
        case F_MeshGeometry_FlatCapsule3D:
            {
                FMeshCreateParam_FlatCapsule3D param_FlatCapsule3D;
                FMeshGeometry::CreateFlatCapsule3D(meshDataPC, &param_FlatCapsule3D);
                return true;
            }
        case F_MeshGeometry_FlatCone3D:
            {
                FMeshCreateParam_FlatCone3D param_FlatCone3D;
                FMeshGeometry::CreateFlatCone3D(meshDataPC, &param_FlatCone3D);
                return true;
            }
        case F_MeshGeometry_FlatTorus3D:
            {
                FMeshCreateParam_FlatTorus3D param_FlatTorus3D;
                FMeshGeometry::CreateFlatTorus3D(meshDataPC, &param_FlatTorus3D);
                return true;
            }
        }
        return false;
    }
	bool FMeshGeometry::CreateFlat3DGeometryWithParam(FMeshDataPC& meshDataPC, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {
        if (pParam == nullptr)
        {
            return CreateFlat3DGeometry(meshDataPC, typeMeshGeometry);
        }

        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_FlatTriangle3D:
            {
                FMeshCreateParam_FlatTriangle3D* pParam_FlatTriangle3D = static_cast<FMeshCreateParam_FlatTriangle3D*>(pParam);
                FMeshGeometry::CreateFlatTriangle3D(meshDataPC, pParam_FlatTriangle3D);
                return true;
            }
        case F_MeshGeometry_FlatQuad3D:
            {
                FMeshCreateParam_FlatQuad3D* pParam_FlatQuad3D = static_cast<FMeshCreateParam_FlatQuad3D*>(pParam);
                FMeshGeometry::CreateFlatQuad3D(meshDataPC, pParam_FlatQuad3D);
                return true;
            }
        case F_MeshGeometry_FlatCircle3D:
            {
                FMeshCreateParam_FlatCircle3D* pParam_FlatCircle3D = static_cast<FMeshCreateParam_FlatCircle3D*>(pParam);
                FMeshGeometry::CreateFlatCircle3D(meshDataPC, pParam_FlatCircle3D);
                return true;
            }
        case F_MeshGeometry_FlatAABB3D:
            {
                FMeshCreateParam_FlatAABB3D* pParam_FlatAABB3D = static_cast<FMeshCreateParam_FlatAABB3D*>(pParam);
                FMeshGeometry::CreateFlatAABB3D(meshDataPC, pParam_FlatAABB3D);
                return true;
            }
        case F_MeshGeometry_FlatSphere3D:
            {
                FMeshCreateParam_FlatSphere3D* pParam_FlatSphere3D = static_cast<FMeshCreateParam_FlatSphere3D*>(pParam);
                FMeshGeometry::CreateFlatSphere3D(meshDataPC, pParam_FlatSphere3D);
                return true;
            }
        case F_MeshGeometry_FlatCylinder3D:
            {
                FMeshCreateParam_FlatCylinder3D* pParam_FlatCylinder3D = static_cast<FMeshCreateParam_FlatCylinder3D*>(pParam);
                FMeshGeometry::CreateFlatCylinder3D(meshDataPC, pParam_FlatCylinder3D);
                return true;
            }
        case F_MeshGeometry_FlatCapsule3D:
            {
                FMeshCreateParam_FlatCapsule3D* pParam_FlatCapsule3D = static_cast<FMeshCreateParam_FlatCapsule3D*>(pParam);
                FMeshGeometry::CreateFlatCapsule3D(meshDataPC, pParam_FlatCapsule3D);
                return true;
            }
        case F_MeshGeometry_FlatCone3D:
            {
                FMeshCreateParam_FlatCone3D* pParam_FlatCone3D = static_cast<FMeshCreateParam_FlatCone3D*>(pParam);
                FMeshGeometry::CreateFlatCone3D(meshDataPC, pParam_FlatCone3D);
                return true;
            }
        case F_MeshGeometry_FlatTorus3D:
            {
                FMeshCreateParam_FlatTorus3D* pParam_FlatTorus3D = static_cast<FMeshCreateParam_FlatTorus3D*>(pParam);
                FMeshGeometry::CreateFlatTorus3D(meshDataPC, pParam_FlatTorus3D);
                return true;
            }
        }
        return false;
    }

    //FlatTriangle3D
    void FMeshGeometry::CreateFlatTriangle3D(FMeshDataPC& meshDataPC,
                                             const FVector3& vTop,
                                             const FVector3& vLeft,
                                             const FVector3& vRight,
                                             const FVector4& vColor)
    {
        //        0 
        //        /\
        //       /  \
        //    1 ------ 2

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(vTop, vColor));
        AddVertex(meshDataPC, FMeshVertexPC(vLeft, vColor));
        AddVertex(meshDataPC, FMeshVertexPC(vRight, vColor));
        
        //Index
        AddIndexTriangle(meshDataPC, 0, 2, 1);
    }

    //FlatQuad3D
    void FMeshGeometry::CreateFlatQuad3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vLeftTop,
                                         const FVector3& vLeftBottom,
                                         const FVector3& vRightBottom,
                                         const FVector3& vRightTop,
                                         const FVector4& vColor)
    {
        //  0       3
        //   --------
        //   |\     |
        //   |  \   |
        //   |    \ |
        //   --------
        //  1        2

        //Vertex
        AddVertex(meshDataPC, FMeshVertexPC(vLeftTop, vColor));
        AddVertex(meshDataPC, FMeshVertexPC(vLeftBottom, vColor));
        AddVertex(meshDataPC, FMeshVertexPC(vRightBottom, vColor));
        AddVertex(meshDataPC, FMeshVertexPC(vRightTop, vColor));

        //Index
        AddIndexTriangle(meshDataPC, 0, 3, 2);
        AddIndexTriangle(meshDataPC, 2, 1, 0);
    }

    //FlatCircle3D
    void FMeshGeometry::CreateFlatCircle3D(FMeshDataPC& meshDataPC,
                                           const FVector3& vCenter,
                                           const FVector3& vDir,
                                           const FVector3& vUp,
                                           const FVector4& vColor,
                                           float radius,
                                           uint32 segment)
    {
        //          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *

        //Vertex
        uint32 vertexCount = segment + 1;
        uint32 faceCount = segment;
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        ResizeVertexCount(meshDataPC, vertexCount);
        SetVertex(meshDataPC, 0, FMeshVertexPC(vCenter, vColor));
        int index = 1;
        for (uint32 i = 0; i < segment; i++)
        {
            FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(thetaStep * i, vUp);
            FVector3 vCur = FMath::Transform(qRot, vDir);
            FVector3 vPos = vCenter + vCur * radius;
            SetVertex(meshDataPC, index, FMeshVertexPC(vPos, vColor));
            index++;
        }
        meshDataPC.RefreshSphereAndAABB(vCenter);

        //Index
        ResizeIndexCount(meshDataPC, faceCount * 3);
        for (uint32 i = 0; i < segment; ++i)    
        {
            if (i != segment - 1)
            {
                SetIndexTriangle(meshDataPC, i * 3 + 0, 0, i + 2, i + 1);
            }
            else
            {
                SetIndexTriangle(meshDataPC, i * 3 + 0, 0, 1, i);
            }
        }
    }

    //FlatAABB3D
    void FMeshGeometry::CreateFlatAABB3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vCenter,
                                         const FVector3& vExtent,
                                         const FVector4& vColor)
    {
        //     7+------+4			  0: -+-
		//     /|     /|			  1: ---
		//    / |    / |			  2: +--
		//   / 6+---/--+5	 y		  3: ++-
		// 0+------+3 /		 | z	  4: +++
		//  | /    | /    	 |/		  5: +-+
		//  |/     |/     	 *---x	  6: --+
		// 1+------+2        		  7: -++

        //Vertex
        FVector3 vX = FMath::ms_v3UnitX * vExtent.x;
        FVector3 vY = FMath::ms_v3UnitY * vExtent.y;
        FVector3 vZ = FMath::ms_v3UnitZ * vExtent.z;
        AddVertex(meshDataPC, FMeshVertexPC(vCenter - vX + vY - vZ, vColor)); //0 -+-
        AddVertex(meshDataPC, FMeshVertexPC(vCenter - vX - vY - vZ, vColor)); //1 ---
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vX - vY - vZ, vColor)); //2 +--
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vX + vY - vZ, vColor)); //3 ++-
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vX + vY + vZ, vColor)); //4 +++
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vX - vY + vZ, vColor)); //5 +-+
        AddVertex(meshDataPC, FMeshVertexPC(vCenter - vX - vY + vZ, vColor)); //6 --+
        AddVertex(meshDataPC, FMeshVertexPC(vCenter - vX + vY + vZ, vColor)); //7 -++

        //Index
        AddIndexTriangle(meshDataPC, 0, 3, 2); //Front
        AddIndexTriangle(meshDataPC, 2, 1, 0); 
        AddIndexTriangle(meshDataPC, 4, 7, 6); //Back
        AddIndexTriangle(meshDataPC, 6, 5, 4);
        AddIndexTriangle(meshDataPC, 7, 4, 3); //Top
        AddIndexTriangle(meshDataPC, 3, 0, 7);
        AddIndexTriangle(meshDataPC, 1, 2, 5); //Bottom
        AddIndexTriangle(meshDataPC, 5, 6, 1);
        AddIndexTriangle(meshDataPC, 7, 0, 1); //Left
        AddIndexTriangle(meshDataPC, 1, 6, 7);
        AddIndexTriangle(meshDataPC, 3, 4, 5); //Right
        AddIndexTriangle(meshDataPC, 5, 2, 3);
    }

    //FlatSphere3D
    void FMeshGeometry::CreateFlatSphere3D(FMeshDataPC& meshDataPC,
                                           const FVector3& vCenter,
                                           const FVector3& vUp,
                                           const FVector4& vColor,
                                           float radius,
                                           uint32 sliceCount,
                                           uint32 stackCount)
    {
        //           0 
        //           *   
	    //	    *    - 2  *  1          
	    //        -     -          y                   
	    //   *	     *       *     | z              
	    //	      -     - l-2      |/
	    //      *    -    *  l-1   *---x
	    //           *
        //           l=(stackCount-1)*(sliceCount+1)+1

        //Vertex
        //Vertex Top
        FMeshVertexPC vertexTop(vCenter + vUp * radius, vColor);
        AddVertex(meshDataPC, vertexTop);
        {
            FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
            //Vertex
            float phiStep = FMath::ms_fPI / stackCount;
            float thetaStep = 2.0f * FMath::ms_fPI / sliceCount;
            for (uint32 i = 1; i <= stackCount - 1; ++i)
            {
                float phi = i * phiStep;
                for (uint32 j = 0; j <= sliceCount; ++j)
                {
                    float theta = j * thetaStep;
                    FVector3 vPos(radius * sinf(phi) * cosf(theta),
                                  radius * cosf(phi),
                                  radius * sinf(phi) * sinf(theta));
                    vPos = vCenter + FMath::Transform(qRot, vPos);
                    FMeshVertexPC vertex(vPos, vColor);
                    AddVertex(meshDataPC, vertex);
                }
            }
        }
        //Vertex Bottom
        FMeshVertexPC vertexBottom(vCenter - vUp * radius, vColor);
        AddVertex(meshDataPC, vertexBottom);

        //Index Top
        for (uint32 i = 1; i <= sliceCount; ++i)
        {
            AddIndexTriangle(meshDataPC, 
                             0, 
                             i + 1, 
                             i);
        }

        //Index Inner
        uint32 baseIndex = 1;
        uint32 ringVertexCount = sliceCount + 1;
        for (uint32 i = 0; i < stackCount - 2; ++i)
        {
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                AddIndexTriangle(meshDataPC, 
                                 baseIndex + i * ringVertexCount + j,
                                 baseIndex + (i + 1) * ringVertexCount + j + 1,
                                 baseIndex + (i + 1) * ringVertexCount + j);

                AddIndexTriangle(meshDataPC, 
                                 baseIndex + (i + 1) * ringVertexCount + j + 1,
                                 baseIndex + i * ringVertexCount + j,
                                 baseIndex + i * ringVertexCount + j + 1);
            }
        }

        //Index Bottom
        uint32 southPoleIndex = GetVertexCount(meshDataPC) - 1;
        baseIndex = southPoleIndex - ringVertexCount;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            AddIndexTriangle(meshDataPC, 
                             southPoleIndex,
                             baseIndex + i,
                             baseIndex + i + 1);
        }
    }

    //FlatCylinder3D
    void FMeshGeometry::CreateFlatCylinder3D(FMeshDataPC& meshDataPC,
                                             const FVector3& vCenter,
                                             const FVector3& vUp,
                                             const FVector4& vColor,
                                             float radiusBottom,
                                             float radiusTop,
                                             float height,
                                             uint32 sliceCount,
                                             uint32 stackCount)
    {
        //       * 
        //    *     *
        //    *  *  *
        //    *     *
        //    *     *
        //    *     *
        //    *  *  *
        //    *     *
        //       *       

        //Vertex
        FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
        float dTheta = 2.0f * FMath::ms_fPI / sliceCount;
        AddVertex(meshDataPC, FMeshVertexPC(vCenter - vUp * (0.5f * height), vColor)); //0 - Bottom
        AddVertex(meshDataPC, FMeshVertexPC(vCenter + vUp * (0.5f * height), vColor)); //1 - Top
        int indexBottom = 2;
        float stackHeight = height / stackCount;
        float radiusStep = (radiusTop - radiusBottom) / stackCount;
        for (uint32 i = 0; i <= stackCount; ++i)
        {
            float y = - 0.5f * height + i * stackHeight;
            float r = radiusBottom + i * radiusStep;

            float dTheta = 2.0f * FMath::ms_fPI / sliceCount;
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                float c = cosf(j * dTheta);
                float s = sinf(j * dTheta);
                FVector3 vPos(r * c, y, r * s);
                vPos = vCenter +  FMath::Transform(qRot, vPos);
                FMeshVertexPC vertex(vPos, vColor);
                AddVertex(meshDataPC, vertex);
            }
        }
        
        //Index
        //Bottom/Top
        int indexTop = indexBottom + stackCount * sliceCount;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (i == sliceCount - 1)
            {
                //Bottom
                AddIndexTriangle(meshDataPC, 
                                 0,
                                 indexBottom,
                                 indexBottom + i);
                //Top
                AddIndexTriangle(meshDataPC, 
                                 1,
                                 indexTop + i,
                                 indexTop);
            }
            else
            {
                //Bottom
                AddIndexTriangle(meshDataPC, 
                                 0,
                                 indexBottom + i + 1,
                                 indexBottom + i);
                //Top
                AddIndexTriangle(meshDataPC, 
                                 1,
                                 indexTop + i,
                                 indexTop + i + 1);
            }
        }
        //Center
        for (uint32 i = 0; i < stackCount; ++i)
        {
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                if (j == sliceCount - 1)
                {
                    AddIndexTriangle(meshDataPC, 
                                     indexBottom + i * sliceCount + j,
                                     indexBottom + (i + 1) * sliceCount + j,
                                     indexBottom + (i + 1) * sliceCount);
                    AddIndexTriangle(meshDataPC, 
                                     indexBottom + (i + 1) * sliceCount,
                                     indexBottom + i * sliceCount,
                                     indexBottom + i * sliceCount + j);
                }
                else
                {
                    AddIndexTriangle(meshDataPC, 
                                     indexBottom + i * sliceCount + j,
                                     indexBottom + (i + 1) * sliceCount + j,
                                     indexBottom + (i + 1) * sliceCount + j + 1);
                    AddIndexTriangle(meshDataPC, 
                                     indexBottom + (i + 1) * sliceCount + j + 1,
                                     indexBottom + i * sliceCount + j + 1,
                                     indexBottom + i * sliceCount + j);
                }
            }
        }
    }

    //FlatCapsule3D
    void FMeshGeometry::CreateFlatCapsule3D(FMeshDataPC& meshDataPC,
                                            const FVector3& vCenter,
                                            const FVector3& vUp,
                                            const FVector4& vColor,
                                            float radius,
                                            float height,
                                            uint32 numRings,
                                            uint32 numSegments,
                                            uint32 numSegHeight)
    {
        //      *** 
        //    *  *  *
        //    *  *  *
        //    *     *
        //    *     *
        //    *     *
        //    *  *  *
        //    *  *  *
        //      ***  

        //Vertex
        FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
        uint32 nVertexCount = (2 * numRings + 2) * (numSegments + 1) + (numSegHeight - 1) * (numSegments + 1);
        uint32 nIndexCount = (2 * numRings + 1) * (numSegments + 1) * 6 + (numSegHeight - 1) * (numSegments + 1) * 6;
        ReserveVertexCount(meshDataPC, nVertexCount);
        ReserveIndexCount(meshDataPC, nIndexCount);

        float fDeltaRingAngle = (FMath::ms_fPI_Half / numRings);
        float fDeltaSegAngle = (FMath::ms_fPI_Two / numSegments);

        float sphereRatio = radius / (2 * radius + height);
        float cylinderRatio = height / (2 * radius + height);
        int offset = 0;

        //1> Top half sphere
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf(ring * fDeltaRingAngle);
            float y0 = radius * cosf(ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 vPos(x0, +0.5f * height + y0, z0);
                vPos = vCenter +  FMath::Transform(qRot, vPos);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));

                //Index
                AddIndexTriangle(meshDataPC, offset + numSegments + 1, offset + numSegments, offset);
                AddIndexTriangle(meshDataPC, offset + numSegments + 1, offset, offset + 1);

                offset ++;
            }
        }

        //2> Cylinder part
        float deltaAngle = (FMath::ms_fPI_Two / numSegments);
        float deltamHeight = height/(float)numSegHeight;
        for (uint32 i = 1; i < numSegHeight; i++)
        {
            for (uint32 j = 0; j<=numSegments; j++)
            {
                float x0 = radius * cosf(j * deltaAngle);
                float z0 = radius * sinf(j * deltaAngle);
                FVector3 vPos(x0, 0.5f * height - i * deltamHeight, z0);
                vPos = vCenter +  FMath::Transform(qRot, vPos);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));
                
                //Index
                AddIndexTriangle(meshDataPC, offset + numSegments + 1, offset + numSegments, offset);
                AddIndexTriangle(meshDataPC, offset + numSegments + 1, offset, offset + 1);
                
                offset ++;
            }
        }

        //3> Bottom half sphere
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf (FMath::ms_fPI_Half + ring * fDeltaRingAngle);
            float y0 =  radius * cosf (FMath::ms_fPI_Half + ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 vPos(x0, - 0.5f * height + y0, z0);
                vPos = vCenter +  FMath::Transform(qRot, vPos);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));

                if (ring != numRings)
                {
                    AddIndexTriangle(meshDataPC, offset + numSegments + 1, offset + numSegments, offset);
                    AddIndexTriangle(meshDataPC, offset + numSegments + 1, offset, offset + 1);
                }

                offset ++;
            } 
        } 
    }

    //FlatCone3D
    void FMeshGeometry::CreateFlatCone3D(FMeshDataPC& meshDataPC,
                                         const FVector3& vCenter,
                                         const FVector3& vUp,
                                         const FVector4& vColor,
                                         float radius,
                                         float height,
                                         uint32 numSegBase,
                                         uint32 numSegHeight)
    {
        //               *
        //             *   *
        //           *       *
        //         *           *
        //       *       +       *
        //     *   +           +   *
        //    *          *           *
        //      +                  +
        //          +          +
        //               +

        //Vertex
        FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
        uint32 nVertexCount = (numSegHeight + 1) * (numSegBase + 1) + numSegBase + 2;
        uint32 nIndexCount = numSegHeight * numSegBase * 6 + 3 * numSegBase;
        ReserveVertexCount(meshDataPC, nVertexCount);
        ReserveIndexCount(meshDataPC, nIndexCount);

        float deltaAngle = (FMath::ms_fPI_Two / numSegBase);
        float deltaHeight = height / (float)numSegHeight;
        int offset = 0;

        //1> Cone
        for (uint32 i = 0; i <= numSegHeight; i++)
        {
            float r0 = radius * (1 - i / (float)numSegHeight);
            for (uint32 j = 0; j <= numSegBase; j++)
            {
                float x0 = r0 * cosf(j * deltaAngle);
                float z0 = r0 * sinf(j * deltaAngle);
                FVector3 vPos(x0, i * deltaHeight, z0);
                vPos = vCenter +  FMath::Transform(qRot, vPos);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));

                if (i != numSegHeight && j != numSegBase)
                {
                    //Index
                    AddIndexTriangle(meshDataPC, offset + numSegBase + 2, offset, offset + numSegBase + 1);
                    AddIndexTriangle(meshDataPC, offset + numSegBase + 2, offset + 1, offset);
                }

                offset ++;
            }
        }

        //2> Low Cap
        int centerIndex = offset;
        AddVertex(meshDataPC, FMeshVertexPC(vCenter, vColor));
        offset++;
        for (uint32 j = 0; j <= numSegBase; j++)
        {
            float x0 = radius * cosf(j * deltaAngle);
            float z0 = radius * sinf(j * deltaAngle);
            FVector3 vPos(x0, 0.0f, z0);
            vPos = vCenter +  FMath::Transform(qRot, vPos);
            AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));

            if (j != numSegBase)
            {
                AddIndexTriangle(meshDataPC, centerIndex, offset, offset + 1);
            }
            offset++;
        }
    }

    //FlatTorus3D
    void FMeshGeometry::CreateFlatTorus3D(FMeshDataPC& meshDataPC,
                                          const FVector3& vCenter,
                                          const FVector3& vUp,
                                          const FVector4& vColor,
                                          float radius,
                                          float sectionRadius,
                                          uint32 numSegSection,
                                          uint32 numSegCircle)
    {
        //         + +
        //       + * *  +  
        //     + * - -  *  +
        //    + * -    - * +
        //     + * - -  *  + 
        //       + * *  +
        //         +  +

        //Vertex
        FQuaternion qRot = FMath::ToQuaternionFromSrc2Dst(FMath::ms_v3UnitY, vUp);
        uint32 nVertexCount = (numSegCircle + 1) * (numSegSection + 1);
        uint32 nIndexCount = (numSegCircle) * (numSegSection + 1) * 6;
        ReserveVertexCount(meshDataPC, nVertexCount);
        ReserveIndexCount(meshDataPC, nIndexCount);

        float deltaSection = (FMath::ms_fPI_Two / numSegSection);
        float deltaCircle = (FMath::ms_fPI_Two / numSegCircle);
        int offset = 0;

        for (uint32 i = 0; i <= numSegCircle; i++)
        {   
            for (uint32 j = 0; j<= numSegSection; j++)
            {
                FVector3 c0(radius, 0.0, 0.0);
                FVector3 v0(radius + sectionRadius * cosf(j * deltaSection), sectionRadius * sinf(j * deltaSection), 0.0);
                FQuaternion qRotAxis = FMath::ToQuaternionFromRadianAxis(i * deltaCircle, FMath::ms_v3UnitY);
                FVector3 vPos = FMath::Transform(qRotAxis, v0);
                vPos = vCenter +  FMath::Transform(qRot, vPos);
                AddVertex(meshDataPC, FMeshVertexPC(vPos, vColor));

                if (i != numSegCircle)
                {
                    //Index
                    AddIndexTriangle(meshDataPC, offset + numSegSection + 1, offset, offset + numSegSection);
                    AddIndexTriangle(meshDataPC, offset + numSegSection + 1, offset + 1, offset);
                }
                offset ++;
            }
        }       
    }


    ////////////////////////////////// Entity ////////////////////////////////
    bool FMeshGeometry::CreateEntityGeometry(FMeshData& meshData, FMeshGeometryType typeMeshGeometry)
    {
        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_EntityTriangle:
            {
                FMeshCreateParam_EntityTriangle param_EntityTriangle;
                FMeshGeometry::CreateEntityTriangle(meshData, &param_EntityTriangle);
                return true;
            }
        case F_MeshGeometry_EntityQuad:
            {
                FMeshCreateParam_EntityQuad param_EntityQuad;
                FMeshGeometry::CreateEntityQuad(meshData, &param_EntityQuad);
                return true;
            }
        case F_MeshGeometry_EntityGrid:
            {
                FMeshCreateParam_EntityGrid param_EntityGrid;
                FMeshGeometry::CreateEntityGrid(meshData, &param_EntityGrid);
                return true;
            }
        case F_MeshGeometry_EntityCircle:
            {
                FMeshCreateParam_EntityCircle param_EntityCircle;
                FMeshGeometry::CreateEntityCircle(meshData, &param_EntityCircle);
                return true;
            }
        case F_MeshGeometry_EntityAABB:
            {
                FMeshCreateParam_EntityAABB param_EntityAABB;
                FMeshGeometry::CreateEntityAABB(meshData, &param_EntityAABB);
                return true;
            }
        case F_MeshGeometry_EntitySphere:
            {
                FMeshCreateParam_EntitySphere param_EntitySphere;
                FMeshGeometry::CreateEntitySphere(meshData, &param_EntitySphere);
                return true;
            }
         case F_MeshGeometry_EntityGeoSphere:
            {
                FMeshCreateParam_EntityGeoSphere param_EntityGeoSphere;
                FMeshGeometry::CreateEntityGeoSphere(meshData, &param_EntityGeoSphere);
                return true;
            }
        case F_MeshGeometry_EntityCylinder:
            {
                FMeshCreateParam_EntityCylinder param_EntityCylinder;
                FMeshGeometry::CreateEntityCylinder(meshData, &param_EntityCylinder);
                return true;
            }
        case F_MeshGeometry_EntityCapsule:
            {
                FMeshCreateParam_EntityCapsule param_EntityCapsule;
                FMeshGeometry::CreateEntityCapsule(meshData, &param_EntityCapsule);
                return true;
            }
        case F_MeshGeometry_EntityCone:
            {
                FMeshCreateParam_EntityCone param_EntityCone;
                FMeshGeometry::CreateEntityCone(meshData, &param_EntityCone);
                return true;
            }
        case F_MeshGeometry_EntityTorus:
            {
                FMeshCreateParam_EntityTorus param_EntityTorus;
                FMeshGeometry::CreateEntityTorus(meshData, &param_EntityTorus);
                return true;
            }
        case F_MeshGeometry_EntitySkyBox:
            {
                FMeshCreateParam_EntitySkyBox param_EntitySkyBox;
                FMeshGeometry::CreateEntitySkyBox(meshData, &param_EntitySkyBox);
                return true;
            }
        case F_MeshGeometry_EntitySkyDome:
            {
                FMeshCreateParam_EntitySkyDome param_EntitySkyDome;
                FMeshGeometry::CreateEntitySkyDome(meshData, &param_EntitySkyDome);
                return true;
            }
        case F_MeshGeometry_EntityTerrain:
            {
                FMeshCreateParam_EntityTerrain param_EntityTerrain;
                FMeshGeometry::CreateEntityTerrain(meshData, &param_EntityTerrain);
                return true;
            }
        }
        return false;
    }
    bool FMeshGeometry::CreateEntityGeometryWithParam(FMeshData& meshData, FMeshGeometryType typeMeshGeometry, FMeshCreateParam* pParam)
    {   
        if (pParam == nullptr)
        {
            return CreateEntityGeometry(meshData, typeMeshGeometry);
        }

        switch ((int)typeMeshGeometry)
        {
        case F_MeshGeometry_EntityTriangle:
            {
                FMeshCreateParam_EntityTriangle* pParam_EntityTriangle = static_cast<FMeshCreateParam_EntityTriangle*>(pParam);
                FMeshGeometry::CreateEntityTriangle(meshData, pParam_EntityTriangle);
                return true;
            }
        case F_MeshGeometry_EntityQuad:
            {
                FMeshCreateParam_EntityQuad* pParam_EntityQuad = static_cast<FMeshCreateParam_EntityQuad*>(pParam);
                FMeshGeometry::CreateEntityQuad(meshData, pParam_EntityQuad);
                return true;
            }
        case F_MeshGeometry_EntityGrid:
            {
                FMeshCreateParam_EntityGrid* pParam_EntityGrid = static_cast<FMeshCreateParam_EntityGrid*>(pParam);
                FMeshGeometry::CreateEntityGrid(meshData, pParam_EntityGrid);
                return true;
            }
        case F_MeshGeometry_EntityCircle:
            {
                FMeshCreateParam_EntityCircle* pParam_EntityCircle = static_cast<FMeshCreateParam_EntityCircle*>(pParam);
                FMeshGeometry::CreateEntityCircle(meshData, pParam_EntityCircle);
                return true;
            }
        case F_MeshGeometry_EntityAABB:
            {
                FMeshCreateParam_EntityAABB* pParam_EntityAABB = static_cast<FMeshCreateParam_EntityAABB*>(pParam);
                FMeshGeometry::CreateEntityAABB(meshData, pParam_EntityAABB);
                return true;
            }
        case F_MeshGeometry_EntitySphere:
            {
                FMeshCreateParam_EntitySphere* pParam_EntitySphere = static_cast<FMeshCreateParam_EntitySphere*>(pParam);
                FMeshGeometry::CreateEntitySphere(meshData, pParam_EntitySphere);
                return true;
            }
        case F_MeshGeometry_EntityGeoSphere:
            {
                FMeshCreateParam_EntityGeoSphere* pParam_EntityGeoSphere = static_cast<FMeshCreateParam_EntityGeoSphere*>(pParam);
                FMeshGeometry::CreateEntityGeoSphere(meshData, pParam_EntityGeoSphere);
                return true;
            }
        case F_MeshGeometry_EntityCylinder:
            {
                FMeshCreateParam_EntityCylinder* pParam_EntityCylinder = static_cast<FMeshCreateParam_EntityCylinder*>(pParam);
                FMeshGeometry::CreateEntityCylinder(meshData, pParam_EntityCylinder);
                return true;
            }
        case F_MeshGeometry_EntityCapsule:
            {
                FMeshCreateParam_EntityCapsule* pParam_EntityCapsule = static_cast<FMeshCreateParam_EntityCapsule*>(pParam);
                FMeshGeometry::CreateEntityCapsule(meshData, pParam_EntityCapsule);
                return true;
            }
        case F_MeshGeometry_EntityCone:
            {
                FMeshCreateParam_EntityCone* pParam_EntityCone = static_cast<FMeshCreateParam_EntityCone*>(pParam);
                FMeshGeometry::CreateEntityCone(meshData, pParam_EntityCone);
                return true;
            }
        case F_MeshGeometry_EntityTorus:
            {
                FMeshCreateParam_EntityTorus* pParam_EntityTorus = static_cast<FMeshCreateParam_EntityTorus*>(pParam);
                FMeshGeometry::CreateEntityTorus(meshData, pParam_EntityTorus);
                return true;
            }
        case F_MeshGeometry_EntitySkyBox:
            {
                FMeshCreateParam_EntitySkyBox* pParam_EntitySkyBox = static_cast<FMeshCreateParam_EntitySkyBox*>(pParam);
                FMeshGeometry::CreateEntitySkyBox(meshData, pParam_EntitySkyBox);
                return true;
            }
        case F_MeshGeometry_EntitySkyDome:
            {
                FMeshCreateParam_EntitySkyDome* pParam_EntitySkyDome = static_cast<FMeshCreateParam_EntitySkyDome*>(pParam);
                FMeshGeometry::CreateEntitySkyDome(meshData, pParam_EntitySkyDome);
                return true;
            }
        case F_MeshGeometry_EntityTerrain:
            {
                FMeshCreateParam_EntityTerrain* pParam_EntityTerrain = static_cast<FMeshCreateParam_EntityTerrain*>(pParam);
                FMeshGeometry::CreateEntityTerrain(meshData, pParam_EntityTerrain);
                return true;
            }
        }
        return false;
    }

    //EntityTriangle
    void FMeshGeometry::CreateEntityTriangle(FMeshData& meshData, 
                                             const FVector3& vTop,
                                             const FVector3& vLeft,
                                             const FVector3& vRight,
                                             bool flipV,
                                             bool rightHand)
    {
        //        0 
        //        /\
        //       /  \
        //    1 ------ 2

        //Vertex
        AddVertex(meshData, FMeshVertex( vTop.x, vTop.y, vTop.z,
                                         0.0f,  0.0f,  -1.0f,
                                         1.0f,  0.0f,   0.0f,
                                         0.5f,  flipV ? 1.0f : 0.0f));

        AddVertex(meshData, FMeshVertex( vLeft.x, vLeft.y, vLeft.z,
                                         0.0f,  0.0f,  -1.0f,
                                         1.0f,  0.0f,   0.0f,
                                         0.0f,  flipV ? 0.0f : 1.0f));

        AddVertex(meshData, FMeshVertex( vRight.x, vRight.y, vRight.z,
                                         0.0f,  0.0f,  -1.0f,
                                         1.0f,  0.0f,   0.0f,
                                         1.0f,  flipV ? 0.0f : 1.0f));
        
        //Index
        if (rightHand)
        {
            AddIndexTriangle(meshData, 0, 1, 2);
        }
        else
        {
            AddIndexTriangle(meshData, 0, 2, 1);
        }
    }

    //EntityQuad
    void FMeshGeometry::CreateEntityQuad(FMeshData& meshData, 
                                         float centerX, 
                                         float centerY, 
                                         float width, 
                                         float height, 
                                         float depth,
                                         bool flipV,
                                         bool rightHand)
    {
        //  0       3
        //   --------
        //   |\     |
        //   |  \   |
        //   |    \ |
        //   --------
        //  1        2

        //Vertex
        AddVertex(meshData, FMeshVertex(centerX - width/2, centerY + height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        0.0f, flipV ? 1.0f : 0.0f));

        AddVertex(meshData, FMeshVertex(centerX - width/2, centerY - height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        0.0f, flipV ? 0.0f : 1.0f));

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY - height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        1.0f, flipV ? 0.0f : 1.0f));

        AddVertex(meshData, FMeshVertex(centerX + width/2, centerY + height/2, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f,  0.0f,
                                        1.0f, flipV ? 1.0f : 0.0f));

        //Index
        if (rightHand)
        {
            AddIndexTriangle(meshData, 0, 1, 2);
            AddIndexTriangle(meshData, 2, 3, 0);
        }
        else
        {
            AddIndexTriangle(meshData, 0, 3, 2);
            AddIndexTriangle(meshData, 2, 1, 0);
        }
    }

    //EntityGrid
    void FMeshGeometry::CreateEntityGrid(FMeshData& meshData, 
                                         float width, 
                                         float height, 
                                         uint32 m, 
                                         uint32 n,
                                         bool flipV,
                                         bool rightHand)
    {
        // (n-1)*(m-2)                  (n-1)*(m-1)
        //      |
        //      |
        //      |
        //  n*2 ----------------        (n-1)*3
        //      |\     |\     |
        //      |  \   |  \   |
        //      |    \ |    \ |
        //  n   ----------------        (n-1)*2
        //      |\     |\(n+1)|(n+2)
        //      |  \   |  \   |
        //      |    \ |    \ |
        //      ------------------------
        //  0          1      2         (n-1)

        uint32 vertexCount = m * n;
        uint32 faceCount = (m - 1) * (n - 1) * 2;

        //Vertex
        float halfW = 0.5f * width;
        float halfH = 0.5f * height;

        float dx = width / (n - 1);
        float dz = height / (m - 1);

        float du = 1.0f / (n - 1);
        float dv = 1.0f / (m - 1);

        ResizeVertexCount(meshData, vertexCount);
        for (uint32 i = 0; i < m; ++i)
        {
            float y = -halfH + i * dz;
            for (uint32 j = 0; j < n; ++j)
            {
                float x = -halfW + j * dx;

                FMeshVertex vertex(   x,    y,  0.0f,
                                   0.0f, 0.0f, -1.0f,
                                   1.0f, 0.0f,  0.0f,
                                   j * du, flipV ? (1.0f - i * dv) : (i * dv));
                SetVertex(meshData, i * n + j, vertex);
            }
        }
        meshData.RefreshSphereAndAABB(FVector3(0, 0, 0));

        //Index
        ResizeIndexCount(meshData, faceCount * 3);
        uint32 k = 0;
        for (uint32 i = 0; i < m - 1; ++i)
        {
            for (uint32 j = 0; j < n - 1; ++j)
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, k + 0,
                                     (i + 1) * n + j,
                                     i * n + j,
                                     i * n + j + 1);

                    SetIndexTriangle(meshData, k + 3,
                                     i * n + j + 1,
                                     (i + 1) * n + j + 1,
                                     (i + 1) * n + j);
                }
                else
                {
                    SetIndexTriangle(meshData, k + 0,
                                     (i + 1) * n + j,
                                     (i + 1) * n + j + 1,
                                     i * n + j + 1);

                    SetIndexTriangle(meshData, k + 3,
                                     i * n + j + 1,
                                     i * n + j,
                                     (i + 1) * n + j);
                }

                k += 6;
            }
        }
    }

    //EntityCircle
    void FMeshGeometry::CreateEntityCircle(FMeshData& meshData,
                                           float radius,
                                           uint32 segment,
                                           bool flipV,
                                           bool rightHand)
    {
		//          *  * 
		//		*		   * 2
		//
		//	   *	 * 0    * 1
		//			
		//      *          * segment
		//          *   *
        
        uint32 vertexCount = segment + 1;
        uint32 faceCount = segment;

        //Vertex
        float thetaStep = 2.0f * FMath::ms_fPI / segment;
        ResizeVertexCount(meshData, vertexCount);
        SetVertex(meshData, 
                  0, 
                  FMeshVertex(0.0f, 0.0f,  0.0f,
                              0.0f, 0.0f, -1.0f,
                              1.0f, 0.0f,  0.0f,
                              0.5f, 0.5f));
        int index = 1;
        for (uint32 i = 0; i < segment; i++)
        {
            float ux = cos(thetaStep * i);
            float uy = sin(thetaStep * i);

            SetVertex(meshData, 
                      index,
                      FMeshVertex(radius * ux,  radius * uy, 0.0f,
                                  0.0f, 0.0f, -1.0f,
                                  1.0f, 0.0f,  0.0f,
                                  (1.0f + ux) / 2.0f,  flipV ? (1.0f - (1.0f - uy) / 2.0f) : (1.0f - uy) / 2.0f));
            index++;
        }
        meshData.RefreshSphereAndAABB(FVector3(0, 0, 0));

        //Index
        ResizeIndexCount(meshData, faceCount * 3);
        for (uint32 i = 0; i < segment; ++i)    
        {
            if (i != segment - 1)
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, i * 3 + 0,
                                     0,
                                     i + 1,
                                     i + 2);
                }
                else
                {
                    SetIndexTriangle(meshData, i * 3 + 0,
                                     0,
                                     i + 2,
                                     i + 1);
                }
            }
            else
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, i * 3 + 0,
                                     0,
                                     i,
                                     1);
                }
                else
                {
                    SetIndexTriangle(meshData, i * 3 + 0,
                                     0,
                                     1,
                                     i);
                }
            }
        }
    }

    //EntityAABB
    void FMeshGeometry::CreateEntityAABB(FMeshData& meshData, 
                                         float width, 
                                         float height, 
                                         float depth, 
                                         uint32 numSubdivisions,
                                         bool flipV,
                                         bool rightHand)
    {
        //     7+------+4			  0 -  9 - 19  -+-
		//     /|     /|			  1 - 12 - 18  ---
		//    / |    / |			  2 - 15 - 21  +--
		//   / 6+---/--+5	 y		  3 - 10 - 20  ++-
		// 0+------+3 /		 | z	  4 - 11 - 23  +++
		//  | /    | /    	 |/		  5 - 14 - 22  +-+
		//  |/     |/     	 *---x	  6 - 13 - 17  --+
		// 1+------+2        		  7 -  8 - 16  -++

        //Vertex
        FMeshVertex v[24];
        float w2 = 0.5f * width;
        float h2 = 0.5f * height;
        float d2 = 0.5f * depth;
        //Front
        v[0]  = FMeshVertex(-w2, +h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //0 -+-
        v[1]  = FMeshVertex(-w2, -h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //1 ---
        v[2]  = FMeshVertex(+w2, -h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //2 +--
        v[3]  = FMeshVertex(+w2, +h2, -d2,   0.0f,   0.0f,  -1.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //3 ++-
        //Back
        v[4]  = FMeshVertex(+w2, +h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //4 +++
        v[5]  = FMeshVertex(+w2, -h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //5 +-+
        v[6]  = FMeshVertex(-w2, -h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //6 --+
        v[7]  = FMeshVertex(-w2, +h2, +d2,   0.0f,   0.0f,   1.0f,  -1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //7 -++
        //Top
        v[8]  = FMeshVertex(-w2, +h2, +d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //8  - 7
        v[9]  = FMeshVertex(-w2, +h2, -d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //9  - 0
        v[10] = FMeshVertex(+w2, +h2, -d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //10 - 3
        v[11] = FMeshVertex(+w2, +h2, +d2,   0.0f,   1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //11 - 4
        //Bottom
        v[12] = FMeshVertex(-w2, -h2, -d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 1.0f : 0.0f); //12 - 1
        v[13] = FMeshVertex(-w2, -h2, +d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   0.0f, flipV ? 0.0f : 1.0f); //13 - 6
        v[14] = FMeshVertex(+w2, -h2, +d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 0.0f : 1.0f); //14 - 5
        v[15] = FMeshVertex(+w2, -h2, -d2,   0.0f,  -1.0f,   0.0f,   1.0f,  0.0f,   0.0f,   1.0f, flipV ? 1.0f : 0.0f); //15 - 2
        //Left
        v[16] = FMeshVertex(-w2, +h2, +d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   0.0f, flipV ? 1.0f : 0.0f); //16 - 7
        v[17] = FMeshVertex(-w2, -h2, +d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   0.0f, flipV ? 0.0f : 1.0f); //17 - 6
        v[18] = FMeshVertex(-w2, -h2, -d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   1.0f, flipV ? 0.0f : 1.0f); //18 - 1
        v[19] = FMeshVertex(-w2, +h2, -d2,  -1.0f,   0.0f,   0.0f,   0.0f,  0.0f,  -1.0f,   1.0f, flipV ? 1.0f : 0.0f); //19 - 0
        //Right
        v[20] = FMeshVertex(+w2, +h2, -d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   0.0f, flipV ? 1.0f : 0.0f); //20 - 3
        v[21] = FMeshVertex(+w2, -h2, -d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   0.0f, flipV ? 0.0f : 1.0f); //21 - 2
        v[22] = FMeshVertex(+w2, -h2, +d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   1.0f, flipV ? 0.0f : 1.0f); //22 - 5
        v[23] = FMeshVertex(+w2, +h2, +d2,   1.0f,   0.0f,   0.0f,   0.0f,  0.0f,   1.0f,   1.0f, flipV ? 1.0f : 0.0f); //23 - 4

        for (int i = 0; i < 24; i++)
        {
            AddVertex(meshData, v[i]);
        }

        //Index
        uint32 i[36];
        if (rightHand)
        {
            //Front 
            i[0]  = 0;  i[1]  = 1;  i[2]  = 2;
            i[3]  = 2;  i[4]  = 3;  i[5]  = 0;
            //Back
            i[6]  = 4;  i[7]  = 5;  i[8]  = 6;
            i[9]  = 6;  i[10] = 7;  i[11] = 4;
            //Top
            i[12] = 8;  i[13] = 9;  i[14] = 10;
            i[15] = 10; i[16] = 11; i[17] = 8;
            //Bottom
            i[18] = 12; i[19] = 13; i[20] = 14;
            i[21] = 14; i[22] = 15; i[23] = 12;
            //Left
            i[24] = 16; i[25] = 17; i[26] = 18;
            i[27] = 18; i[28] = 19; i[29] = 16;
            //Right
            i[30] = 20; i[31] = 21; i[32] = 22;
            i[33] = 22; i[34] = 23; i[35] = 20;
        }
        else
        {
            //Front 
            i[0]  = 0;  i[1]  = 3;  i[2]  = 2;
            i[3]  = 2;  i[4]  = 1;  i[5]  = 0;
            //Back
            i[6]  = 4;  i[7]  = 7;  i[8]  = 6;
            i[9]  = 6;  i[10] = 5;  i[11] = 4;
            //Top
            i[12] = 8;  i[13] = 11;  i[14] = 10;
            i[15] = 10; i[16] =  9; i[17] = 8;
            //Bottom
            i[18] = 12; i[19] = 15; i[20] = 14;
            i[21] = 14; i[22] = 13; i[23] = 12;
            //Left
            i[24] = 16; i[25] = 19; i[26] = 18;
            i[27] = 18; i[28] = 17; i[29] = 16;
            //Right
            i[30] = 20; i[31] = 23; i[32] = 22;
            i[33] = 22; i[34] = 21; i[35] = 20;
        }
        AddIndices(meshData, 36, i);

        //SubDivide
        numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
        for (uint32 i = 0; i < numSubdivisions; ++i)
        {
            SubDivide(meshData, rightHand);
        }
    }

    //EntitySphere
    void FMeshGeometry::CreateEntitySphere(FMeshData& meshData, 
                                           float radius, 
                                           uint32 sliceCount, 
                                           uint32 stackCount,
                                           bool flipV,
                                           bool rightHand)
    {       
        //           0 
        //           *   
	    //	    *    - 2  *  1          
	    //        -     -          y                   
	    //   *	     *       *     | z              
	    //	      -     - l-2      |/
	    //      *    -    *  l-1   *---x
	    //           *
        //           l=(stackCount-1)*(sliceCount+1)+1

        //Vertex
        //Vertex Top
        FMeshVertex vertexTop(0.0f, +radius, 0.0f,  0.0f, +1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 1.0f : 0.0f);
        AddVertex(meshData, vertexTop);
        {
            //Vertex
            float phiStep = FMath::ms_fPI / stackCount;
            float thetaStep = 2.0f * FMath::ms_fPI / sliceCount;
            for (uint32 i = 1; i <= stackCount - 1; ++i)
            {
                float phi = i * phiStep;
                for (uint32 j = 0; j <= sliceCount; ++j)
                {
                    float theta = j * thetaStep;

                    FMeshVertex vertex;
                    //pos
                    vertex.pos.x = radius * sinf(phi) * cosf(theta);
                    vertex.pos.y = radius * cosf(phi);
                    vertex.pos.z = radius * sinf(phi) * sinf(theta);
                    //color
                    vertex.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                    //normal
                    vertex.normal = FMath::Normalize(vertex.pos);
                    //tangent
                    vertex.tangent.x = -radius * sinf(phi) * sinf(theta);
                    vertex.tangent.y = 0.0f;
                    vertex.tangent.z = +radius * sinf(phi) * cosf(theta);
                    vertex.tangent = FMath::Normalize(vertex.tangent);
                    //texCoord
                    vertex.texCoord.x = theta / FMath::ms_fPI_Two;
                    vertex.texCoord.y = flipV ? (1.0f - phi / FMath::ms_fPI) : (phi / FMath::ms_fPI);

                    AddVertex(meshData, vertex);
                }
            }
        }
        //Vertex Bottom
        FMeshVertex vertexBottom(0.0f, -radius, 0.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, flipV ? 0.0f : 1.0f);
        AddVertex(meshData, vertexBottom);

        //Index Top
        for (uint32 i = 1; i <= sliceCount; ++i)
        {
            if (rightHand)
            {
                AddIndexTriangle(meshData, 
                                 0,
                                 i,
                                 i + 1);
            }
            else
            {
                AddIndexTriangle(meshData, 
                                 0,
                                 i + 1,
                                 i);
            }
        }

        //Index Inner
        uint32 baseIndex = 1;
        uint32 ringVertexCount = sliceCount + 1;
        for (uint32 i = 0; i < stackCount - 2; ++i)
        {
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                if (rightHand)
                {
                    AddIndexTriangle(meshData, 
                                     baseIndex + i * ringVertexCount + j,
                                     baseIndex + (i + 1) * ringVertexCount + j,
                                     baseIndex + (i + 1) * ringVertexCount + j + 1);

                    AddIndexTriangle(meshData, 
                                     baseIndex + (i + 1) * ringVertexCount + j + 1,
                                     baseIndex + i * ringVertexCount + j + 1,
                                     baseIndex + i * ringVertexCount + j);
                }
                else
                {
                    AddIndexTriangle(meshData, 
                                     baseIndex + i * ringVertexCount + j,
                                     baseIndex + (i + 1) * ringVertexCount + j + 1,
                                     baseIndex + (i + 1) * ringVertexCount + j);

                    AddIndexTriangle(meshData, 
                                     baseIndex + (i + 1) * ringVertexCount + j + 1,
                                     baseIndex + i * ringVertexCount + j,
                                     baseIndex + i * ringVertexCount + j + 1);
                }
                
            }
        }

        //Index Bottom
        uint32 southPoleIndex = GetVertexCount(meshData) - 1;
        baseIndex = southPoleIndex - ringVertexCount;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (rightHand)
            {
                AddIndexTriangle(meshData, 
                                 southPoleIndex,
                                 baseIndex + i + 1,
                                 baseIndex + i);
            }
            else
            {
                AddIndexTriangle(meshData, 
                                 southPoleIndex,
                                 baseIndex + i,
                                 baseIndex + i + 1);
            }
        }
    }

    //EntityGeoSphere
    void FMeshGeometry::CreateEntityGeoSphere(FMeshData& meshData, 
                                              float radius, 
                                              uint32 numSubdivisions,
                                              bool flipV,
                                              bool rightHand)
    {
		//             * 4    
		//			 * 5 	    
		//         * 0  * 1           
        //    * 9              * 8    y
		//	 * 11	         * 10     | z
		//		   * 2	* 3           |/
		//             * 6            *---x
		//           * 7    

        //Vertex
        numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
        const float X = 0.525731f;
        const float Z = 0.850651f;
        FVector3 pos[12] =
        {
            FVector3(   -X,  0.0f,     Z),  //0
            FVector3(    X,  0.0f,     Z),  //1
            FVector3(   -X,  0.0f,    -Z),  //2
            FVector3(    X,  0.0f,    -Z),  //3
            FVector3( 0.0f,     Z,     X),  //4
            FVector3( 0.0f,     Z,    -X),  //5
            FVector3( 0.0f,    -Z,     X),  //6
            FVector3( 0.0f,    -Z,    -X),  //7
            FVector3(    Z,     X,  0.0f),  //8
            FVector3(   -Z,     X,  0.0f),  //9
            FVector3(    Z,    -X,  0.0f),  //10
            FVector3(   -Z,    -X,  0.0f)   //11
        };

        //Vertex
        ResizeVertexCount(meshData, 12);
        for (uint32 i = 0; i < 12; ++i)
        {
            FMeshVertex& vertex = GetVertex(meshData, i);
            vertex.pos = pos[i];
        }

        //Index
        if (rightHand)
        {
            uint32 k[60] =
            {
                1,  4,  0,    4, 9, 0,   4,  5, 9,   8, 5,  4,   1, 8, 4,
                1, 10,  8,   10, 3, 8,   8,  3, 5,   3, 2,  5,   3, 7, 2,
                3, 10,  7,   10, 6, 7,   6, 11, 7,   6, 0, 11,   6, 1, 0,
               10,  1,  6,   11, 0, 9,   2, 11, 9,   5, 2,  9,  11, 2, 7
            };
            AddIndices(meshData, 60, k);
        }
        else
        {
            uint32 k[60] =
            {
                1,  0,  4,    4, 0, 9,   4, 9,  5,   8,  4, 5,   1, 4, 8,
                1,  8, 10,   10, 8, 3,   8, 5,  3,   3,  5, 2,   3, 2, 7,
                3,  7, 10,   10, 7, 6,   6, 7, 11,   6, 11, 0,   6, 0, 1,
               10,  6,  1,   11, 9, 0,   2, 9, 11,   5,  9, 2,  11, 7, 2
            };
            AddIndices(meshData, 60, k);
        }

        //SubDivide
        for (uint32 i = 0; i < numSubdivisions; ++i)
        {
            SubDivide(meshData, rightHand);
        }

        uint32 countVertex = GetVertexCount(meshData);
        for (uint32 i = 0; i < countVertex; ++i)
        {
            FMeshVertex& vertex = GetVertex(meshData, i);

            vertex.normal = FMath::Normalize(vertex.pos);
            vertex.pos = radius * vertex.normal;

            float theta = atan2f(vertex.pos.z, vertex.pos.x);
            if (theta < 0.0f)
                theta += FMath::ms_fPI_Two;

            float phi = acosf(vertex.pos.y / radius);

            vertex.texCoord.x = theta / FMath::ms_fPI_Two;
            vertex.texCoord.y = flipV ? (1.0f - phi / FMath::ms_fPI) : (phi / FMath::ms_fPI);

            vertex.tangent.x = -radius * sinf(phi) * sinf(theta);
            vertex.tangent.y = 0.0f;
            vertex.tangent.z = +radius * sinf(phi) * cosf(theta);
            vertex.tangent = FMath::Normalize(vertex.tangent);
        }
        meshData.RefreshSphereAndAABB(FVector3(0, 0 ,0));
    }

    //EntityCylinder
    void s_BuildCylinderTopCap(FMeshData& meshData, 
                               float radiusBottom, 
                               float radiusTop, 
                               float height, 
                               float heightOffset,
                               uint32 sliceCount, 
                               uint32 stackCount, 
                               bool flipV,
                               bool rightHand)
    {
        uint32 baseIndex = FMeshGeometry::GetVertexCount(meshData);
        float y = heightOffset + 0.5f * height;
        float dTheta = 2.0f * FMath::ms_fPI / sliceCount;
        for (uint32 i = 0; i <= sliceCount; ++i)
        {
            float x = radiusTop * cosf(i * dTheta);
            float z = radiusTop * sinf(i * dTheta);

            float u = x / height + 0.5f;
            float v = flipV ? (1.0f - z / height + 0.5f) : (z / height + 0.5f);

            FMeshGeometry::AddVertex(meshData, FMeshVertex(x, y, z, 
                                                           0.0f, 1.0f, 0.0f, 
                                                           1.0f, 0.0f, 0.0f, 
                                                           u, v));
        }
        FMeshGeometry::AddVertex(meshData, FMeshVertex(0.0f, y, 0.0f, 
                                                       0.0f, 1.0f, 0.0f, 
                                                       1.0f, 0.0f, 0.0f, 
                                                       0.5f, 0.5f));

        uint32 centerIndex = FMeshGeometry::GetVertexCount(meshData) - 1;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (rightHand)
            {
                FMeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i,
                                                baseIndex + i + 1);
            }
            else
            {
                FMeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i + 1,
                                                baseIndex + i);
            }
        }
    }

    void s_BuildCylinderBottomCap(FMeshData& meshData, 
                                  float radiusBottom, 
                                  float radiusTop, 
                                  float height, 
                                  float heightOffset,
                                  uint32 sliceCount, 
                                  uint32 stackCount, 
                                  bool flipV,
                                  bool rightHand)
    {
        uint32 baseIndex = FMeshGeometry::GetVertexCount(meshData);
        float y = heightOffset - 0.5f * height;
        float dTheta = 2.0f * FMath::ms_fPI / sliceCount;
        for (uint32 i = 0; i <= sliceCount; ++i)
        {
            float x = radiusBottom * cosf(i * dTheta);
            float z = radiusBottom * sinf(i * dTheta);

            float u = x / height + 0.5f;
            float v = flipV ? (1.0f - z / height + 0.5f) : (z / height + 0.5f);

            FMeshGeometry::AddVertex(meshData, FMeshVertex(x, y, z, 
                                                           0.0f, -1.0f, 0.0f, 
                                                           1.0f, 0.0f, 0.0f, 
                                                           u, v));
        }
        FMeshGeometry::AddVertex(meshData, FMeshVertex(0.0f, y, 0.0f, 
                                                       0.0f, -1.0f, 0.0f, 
                                                       1.0f,  0.0f, 0.0f, 
                                                       0.5f,  0.5f));

        uint32 centerIndex = FMeshGeometry::GetVertexCount(meshData) - 1;
        for (uint32 i = 0; i < sliceCount; ++i)
        {
            if (rightHand)
            {
                FMeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i + 1,
                                                baseIndex + i);
            }
            else
            {
                FMeshGeometry::AddIndexTriangle(meshData, 
                                                centerIndex,
                                                baseIndex + i,
                                                baseIndex + i + 1);
            }
        }
    }

    void FMeshGeometry::CreateEntityCylinder(FMeshData& meshData, 
                                             float radiusBottom, 
                                             float radiusTop, 
                                             float height, 
                                             float heightOffset,
                                             uint32 sliceCount, 
                                             uint32 stackCount,
                                             bool flipV,
                                             bool rightHand)
    {
        //       * 
        //    *     *
        //    *  *  *
        //    *     *
        //    *     *
        //    *     *
        //    *  *  *
        //    *     *
        //       *       

        //Vertex
        float stackHeight = height / stackCount;
        float radiusStep = (radiusTop - radiusBottom) / stackCount;
        uint32 ringCount = stackCount + 1;
        for (uint32 i = 0; i < ringCount; ++i)
        {
            float y = heightOffset - 0.5f * height + i * stackHeight;
            float r = radiusBottom + i * radiusStep;

            float dTheta = 2.0f * FMath::ms_fPI / sliceCount;
            for (uint32 j = 0; j <= sliceCount; ++j)
            {
                FMeshVertex vertex;

                float c = cosf(j * dTheta);
                float s = sinf(j * dTheta);

                vertex.pos = FVector3(r * c, y, r * s);
                vertex.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

                vertex.texCoord.x = (float)j / sliceCount;
                vertex.texCoord.y = flipV ? ((float)i / stackCount) : (1.0f - (float)i / stackCount);

                vertex.tangent = FVector3(-s, 0.0f, c);

                float dr = radiusBottom - radiusTop;
                FVector3 T = vertex.tangent;
                FVector3 B = FVector3(dr * c, -height, dr * s);
                vertex.normal = FMath::Normalize(FMath::Cross(T, B));

                AddVertex(meshData, vertex);
            }
        }

        //Index
        uint32 ringVertexCount = sliceCount + 1;
        for (uint32 i = 0; i < stackCount; ++i)
        {
            for (uint32 j = 0; j < sliceCount; ++j)
            {
                if (rightHand)
                {
                    AddIndexTriangle(meshData, 
                                     i * ringVertexCount + j,
                                     (i + 1) * ringVertexCount + j + 1,
                                     (i + 1) * ringVertexCount + j);

                    AddIndexTriangle(meshData, 
                                     i * ringVertexCount + j,
                                     i * ringVertexCount + j + 1,
                                     (i + 1) * ringVertexCount + j + 1);
                }
                else
                {
                    AddIndexTriangle(meshData, 
                                     i * ringVertexCount + j,
                                     (i + 1) * ringVertexCount + j,
                                     (i + 1) * ringVertexCount + j + 1);

                    AddIndexTriangle(meshData, 
                                     i * ringVertexCount + j,
                                     (i + 1) * ringVertexCount + j + 1,
                                     i * ringVertexCount + j + 1);
                }
            }
        }

        s_BuildCylinderTopCap(meshData, 
                              radiusBottom, 
                              radiusTop, 
                              height, 
                              heightOffset,
                              sliceCount, 
                              stackCount, 
                              flipV, 
                              rightHand);
        s_BuildCylinderBottomCap(meshData, 
                                 radiusBottom, 
                                 radiusTop, 
                                 height, 
                                 heightOffset,
                                 sliceCount, 
                                 stackCount, 
                                 flipV, 
                                 rightHand);
    }

    //EntityCapsule
    void FMeshGeometry::CreateEntityCapsule(FMeshData& meshData,
                                            float radius,
                                            float height,
                                            float heightOffset,
                                            uint32 numRings,
                                            uint32 numSegments,
                                            uint32 numSegHeight,
                                            bool flipV,
                                            bool rightHand)
    {
        //      *** 
        //    *  *  *
        //    *  *  *
        //    *     *
        //    *     *
        //    *     *
        //    *  *  *
        //    *  *  *
        //      ***  

        //Vertex
        uint32 nVertexCount = (2 * numRings + 2) * (numSegments + 1) + (numSegHeight - 1) * (numSegments + 1);
        uint32 nIndexCount = (2 * numRings + 1) * (numSegments + 1) * 6 + (numSegHeight - 1) * (numSegments + 1) * 6;
        ReserveVertexCount(meshData, nVertexCount);
        ReserveIndexCount(meshData, nIndexCount);

        float fDeltaRingAngle = (FMath::ms_fPI_Half / numRings);
        float fDeltaSegAngle = (FMath::ms_fPI_Two / numSegments);

        float sphereRatio = radius / (2 * radius + height);
        float cylinderRatio = height / (2 * radius + height);
        int offset = 0;

        //1> Top half sphere
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf(ring * fDeltaRingAngle);
            float y0 = radius * cosf(ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 pos(x0, heightOffset + 0.5f * height + y0, z0);
                FVector3 normal = FMath::Normalize(FVector3(x0, y0, z0));
                FVector3 tangent = FMath::Normalize(FVector3(-z0, y0, x0));
                float u = (float)seg / (float)numSegments;
                float v = flipV ? (1.0f - (float)ring / (float)numRings * sphereRatio) : (float)ring / (float)numRings * sphereRatio; 

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                if (rightHand)
                {
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + numSegments);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + 1, offset);
                }
                else
                {
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + numSegments, offset);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + 1);
                }

                offset ++;
            }
        }

        //2> Cylinder part
        float deltaAngle = (FMath::ms_fPI_Two / numSegments);
        float deltamHeight = height/(float)numSegHeight;
        for (uint32 i = 1; i < numSegHeight; i++)
        {
            for (uint32 j = 0; j<=numSegments; j++)
            {
                float x0 = radius * cosf(j * deltaAngle);
                float z0 = radius * sinf(j * deltaAngle);
                FVector3 pos(x0, heightOffset + 0.5f * height - i * deltamHeight, z0);
                FVector3 normal = FMath::Normalize(FVector3(x0, 0, z0));
                FVector3 tangent = FMath::Normalize(FVector3(-z0, 0, x0));
                float u = j / (float)numSegments;
                float v = flipV ? (1.0f - i / (float)numSegHeight * cylinderRatio + sphereRatio) : (i / (float)numSegHeight * cylinderRatio + sphereRatio); 

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                if (rightHand)
                {
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + numSegments);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + 1, offset);
                }
                else
                {
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset + numSegments, offset);
                    AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + 1);
                }

                offset ++;
            }
        }

        //3> Bottom half sphere
        for (uint32 ring = 0; ring <= numRings; ring++)
        {
            float r0 = radius * sinf (FMath::ms_fPI_Half + ring * fDeltaRingAngle);
            float y0 =  radius * cosf (FMath::ms_fPI_Half + ring * fDeltaRingAngle);

            for (uint32 seg = 0; seg <= numSegments; seg++)
            {
                float x0 = r0 * cosf(seg * fDeltaSegAngle);
                float z0 = r0 * sinf(seg * fDeltaSegAngle);
                FVector3 pos(x0, heightOffset - 0.5f * height + y0, z0);
                FVector3 normal = FMath::Normalize(FVector3(x0, y0, z0));
                FVector3 tangent = FMath::Normalize(FVector3(-z0, y0, x0));
                float u = (float)seg / (float)numSegments;
                float v = flipV ? (1.0f - ((float)ring / (float)numRings * sphereRatio + cylinderRatio + sphereRatio)) : ((float)ring / (float)numRings * sphereRatio + cylinderRatio + sphereRatio); 

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                if (ring != numRings)
                {
                    if (rightHand)
                    {
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + numSegments);
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset + 1, offset);
                    }
                    else
                    {
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset + numSegments, offset);
                        AddIndexTriangle(meshData, offset + numSegments + 1, offset, offset + 1);
                    }
                }

                offset ++;
            } 
        } 
    }

    //EntityCone
    void FMeshGeometry::CreateEntityCone(FMeshData& meshData,
                                         float radius,
                                         float height,
                                         float heightOffset,
                                         uint32 numSegBase,
                                         uint32 numSegHeight,
                                         bool flipV,
                                         bool rightHand)
    {
        //               *
        //             *   *
        //           *       *
        //         *           *
        //       *       +       *
        //     *   +           +   *
        //    *          *           *
        //      +                  +
        //          +          +
        //               +

        //Vertex
        uint32 nVertexCount = (numSegHeight + 1) * (numSegBase + 1) + numSegBase + 2;
        uint32 nIndexCount = numSegHeight * numSegBase * 6 + 3 * numSegBase;
        ReserveVertexCount(meshData, nVertexCount);
        ReserveIndexCount(meshData, nIndexCount);

        float deltaAngle = (FMath::ms_fPI_Two / numSegBase);
        float deltaHeight = height / (float)numSegHeight;
        int offset = 0;

        FVector3 refNormal = FMath::Normalize(FVector3(radius, height, 0.f));
        
        //1> Cone
        for (uint32 i = 0; i <= numSegHeight; i++)
        {
            float r0 = radius * (1 - i / (float)numSegHeight);
            for (uint32 j = 0; j <= numSegBase; j++)
            {
                float x0 = r0 * cosf(j * deltaAngle);
                float z0 = r0 * sinf(j * deltaAngle);
                FVector3 pos(x0, heightOffset + i * deltaHeight, z0);
                FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(-deltaAngle * j, FMath::ms_v3UnitY);
                FVector3 normal = FMath::Transform(qRot, refNormal);
                FVector3 tangent = FMath::Normalize(FVector3(-z0, i * deltaHeight, x0));
                float u = j / (float)numSegBase;
                float v = flipV ? (1.0f - i / (float)numSegHeight) : i / (float)numSegHeight;

                AddVertex(meshData, 
                          pos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          normal,
                          tangent,
                          FVector2(u, v));

                if (i != numSegHeight && j != numSegBase)
                {
                    if (rightHand)
                    {
                        AddIndexTriangle(meshData, offset + numSegBase + 2, offset + numSegBase + 1, offset);
                        AddIndexTriangle(meshData, offset + numSegBase + 2, offset, offset + 1);
                    }
                    else
                    {
                        AddIndexTriangle(meshData, offset + numSegBase + 2, offset, offset + numSegBase + 1);
                        AddIndexTriangle(meshData, offset + numSegBase + 2, offset + 1, offset);
                    }
                }

                offset ++;
            }
        }

        //2> Low Cap
        int centerIndex = offset;
        AddVertex(meshData, 
                  heightOffset + FMath::ms_v3Zero,
                  FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                  FMath::ms_v3UnitNegY,
                  FMath::ms_v3UnitX,
                  flipV ? FMath::ms_v2UnitX : FMath::ms_v2One);
        offset++;
        for (uint32 j = 0; j <= numSegBase; j++)
        {
            float x0 = radius * cosf(j * deltaAngle);
            float z0 = radius * sinf(j * deltaAngle);
            FVector3 pos(x0, heightOffset, z0);
            float u = j / (float)numSegBase;
            float v = flipV ? 1.0f : 0.0f;

            AddVertex(meshData, 
                      pos,
                      FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                      FMath::ms_v3UnitNegY,
                      FMath::ms_v3UnitX,
                      FVector2(u, v));

            if (j != numSegBase)
            {
                if (rightHand)
                {
                    AddIndexTriangle(meshData, centerIndex, offset + 1, offset);
                }
                else
                {
                    AddIndexTriangle(meshData, centerIndex, offset, offset + 1);
                }
            }
            offset++;
        }
    }

    //EntityTorus
    void FMeshGeometry::CreateEntityTorus(FMeshData& meshData,
                                          float radius,
                                          float sectionRadius,
                                          uint32 numSegSection,
                                          uint32 numSegCircle,
                                          bool flipV,
                                          bool rightHand)
    {
        //         + +
        //       + * *  +  
        //     + * - -  *  +
        //    + * -    - * +
        //     + * - -  *  + 
        //       + * *  +
        //         +  +

        //Vertex
        uint32 nVertexCount = (numSegCircle + 1) * (numSegSection + 1);
        uint32 nIndexCount = (numSegCircle) * (numSegSection + 1) * 6;
        ReserveVertexCount(meshData, nVertexCount);
        ReserveIndexCount(meshData, nIndexCount);

        float deltaSection = (FMath::ms_fPI_Two / numSegSection);
        float deltaCircle = (FMath::ms_fPI_Two / numSegCircle);
        int offset = 0;

        for (uint32 i = 0; i <= numSegCircle; i++)
        {   
            for (uint32 j = 0; j<= numSegSection; j++)
            {
                FVector3 c0(radius, 0.0, 0.0);
                FVector3 v0(radius + sectionRadius * cosf(j * deltaSection), sectionRadius * sinf(j * deltaSection), 0.0);
                FQuaternion qRot = FMath::ToQuaternionFromRadianAxis(i * deltaCircle, FMath::ms_v3UnitY);
                FVector3 vPos = FMath::Transform(qRot, v0);
                FVector3 c = FMath::Transform(qRot, c0);
                float u = i / (float)numSegCircle;
                float v = flipV ? (1.0f - j / (float)numSegSection) : (j / (float)numSegSection);

                AddVertex(meshData, 
                          vPos,
                          FVector4(1.0f, 1.0f, 1.0f, 1.0f),
                          FMath::Normalize(vPos - c),
                          FMath::ms_v3UnitX,
                          FVector2(u, v));

                if (i != numSegCircle)
                {
                    if (rightHand)
                    {
                        AddIndexTriangle(meshData, offset + numSegSection + 1, offset + numSegSection, offset);
                        AddIndexTriangle(meshData, offset + numSegSection + 1, offset, offset + 1);
                    }
                    else
                    {
                        AddIndexTriangle(meshData, offset + numSegSection + 1, offset, offset + numSegSection);
                        AddIndexTriangle(meshData, offset + numSegSection + 1, offset + 1, offset);
                    }
                }
                offset ++;
            }
        }       
    }

    //EntitySkyBox
    void FMeshGeometry::CreateEntitySkyBox(FMeshData& meshData,
                                           bool flipV,
                                           bool rightHand)
    {

    }

    //EntitySkyDome
    void FMeshGeometry::CreateEntitySkyDome(FMeshData& meshData,
                                            bool flipV,
                                            bool rightHand)
    {

    }

    //EntityTerrain
    void FMeshGeometry::CreateEntityTerrain(FMeshData& meshData,
                                            float offsetX,
                                            float offsetZ,
                                            float width,
                                            float height,
                                            uint32 vertexX,
                                            uint32 vertexZ,
                                            float* pHeight,
                                            uint32 heightDataGap,
                                            bool flipV,
                                            bool rightHand)
    {
        uint32 vertexCount = vertexX * vertexZ;
        uint32 faceCount = (vertexX - 1) * (vertexZ - 1) * 2;

        //Vertex
        float halfW = 0.5f * width;
        float halfH = 0.5f * height;

        float dx = width / (vertexX - 1);
        float dz = height / (vertexZ - 1);

        float du = 1.0f / (vertexX - 1);
        float dv = 1.0f / (vertexZ - 1);

        ResizeVertexCount(meshData, vertexCount);
        for (uint32 i = 0; i < vertexZ; ++i)
        {
            float z = halfH - i * dz + offsetZ;
            for (uint32 j = 0; j < vertexX; ++j)
            {
                float height = 0.0f;
                if (pHeight != nullptr)
                {
                    height = pHeight[i * vertexX * heightDataGap + j * heightDataGap];
                }
                
                float x = -halfW + j * dx + offsetX;
                FMeshVertex vertex = FMeshVertex(x, height, z,
                                                 0.0f, 1.0f, .0f,
                                                 1.0f, 0.0f, 0.0f,
                                                 j * du, flipV ? (1.0f - i * dv) : (i * dv));
                SetVertex(meshData, i * vertexZ + j, vertex);
            }
        }
        meshData.RefreshSphereAndAABB(FVector3(offsetX, 0, offsetZ));

        //Index
        ResizeIndexCount(meshData, faceCount * 3);
        uint32 k = 0;
        for (uint32 i = 0; i < vertexZ - 1; ++i)
        {
            for (uint32 j = 0; j < vertexX - 1; ++j)
            {
                if (rightHand)
                {
                    SetIndexTriangle(meshData, k + 0,
                                     i * vertexZ + j,
                                     (i + 1) * vertexZ + j,
                                     (i + 1) * vertexZ + j + 1);
                    
                    SetIndexTriangle(meshData, k + 3,
                                     (i + 1) * vertexZ + j + 1,
                                     i * vertexZ + j + 1,
                                     i * vertexZ + j);
                }
                else
                {
                    SetIndexTriangle(meshData, k + 0,
                                     i * vertexZ + j,
                                     i * vertexZ + j + 1,
                                     (i + 1) * vertexZ + j + 1);
                    
                    SetIndexTriangle(meshData, k + 3,
                                     (i + 1) * vertexZ + j + 1,
                                     (i + 1) * vertexZ + j,
                                     i * vertexZ + j);
                }

                k += 6;
            }
        }
    }

    void FMeshGeometry::MidPoint(const FMeshVertex& v0, const FMeshVertex& v1, FMeshVertex& m)
    {
        m.pos = 0.5f * (v0.pos + v1.pos);
        m.color = 0.5f * (v0.color + v1.color);
        m.normal = FMath::Normalize(0.5f * (v0.normal + v1.normal));
        m.tangent = FMath::Normalize(0.5f * (v0.tangent + v1.tangent));
        m.texCoord = 0.5f * (v0.texCoord + v1.texCoord);
    }

    void FMeshGeometry::SubDivide(FMeshData& meshData, bool rightHand)
    {
        //      (0)
        //       v0
        //       *
        //      / \
        //  (3)/   \(5)
        //  m0*-----*m2
        //   / \   / \
        //  /   \ /   \
        // *-----*-----*
        // v1    m1    v2
        //(1)    (4)   (2)

        FMeshData inputCopy = meshData;
        ResizeVertexCount(meshData, 0);
        ResizeIndexCount(meshData, 0);
        uint32 numTri = GetIndexCount(inputCopy) / 3;
        for (uint32 i = 0; i < numTri; ++i)
        {
            const FMeshVertex& v0 = inputCopy.GetVertex(inputCopy.GetIndex(i * 3 + 0));
            const FMeshVertex& v1 = inputCopy.GetVertex(inputCopy.GetIndex(i * 3 + 1));
            const FMeshVertex& v2 = inputCopy.GetVertex(inputCopy.GetIndex(i * 3 + 2));

            //mid
            FMeshVertex m0,m1,m2;
            MidPoint(v0, v1, m0);
            MidPoint(v1, v2, m1);
            MidPoint(v0, v2, m2);

            //new
            AddVertex(meshData, v0); // 0
            AddVertex(meshData, v1); // 1
            AddVertex(meshData, v2); // 2
            AddVertex(meshData, m0); // 3
            AddVertex(meshData, m1); // 4
            AddVertex(meshData, m2); // 5

            if (rightHand)
            {
                AddIndexTriangle(meshData, 
                                 i * 6 + 0,
                                 i * 6 + 3,
                                 i * 6 + 5);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 4,
                                 i * 6 + 5);

                AddIndexTriangle(meshData, 
                                 i * 6 + 5,
                                 i * 6 + 4,
                                 i * 6 + 2);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 1,
                                 i * 6 + 4);
            }
            else
            {
                AddIndexTriangle(meshData, 
                                 i * 6 + 0,
                                 i * 6 + 5,
                                 i * 6 + 3);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 5,
                                 i * 6 + 4);

                AddIndexTriangle(meshData, 
                                 i * 6 + 5,
                                 i * 6 + 2,
                                 i * 6 + 4);

                AddIndexTriangle(meshData, 
                                 i * 6 + 3,
                                 i * 6 + 4,
                                 i * 6 + 1);
            }
        }
    }


    //FMeshDataPC
    uint32 FMeshGeometry::GetVertexCount(FMeshDataPC& meshDataPC)
    {
        return meshDataPC.GetVertexCount();
    }
    void FMeshGeometry::ReserveVertexCount(FMeshDataPC& meshDataPC, uint32 count)
    {
        meshDataPC.ReserveVertexCount(count);
    }
    void FMeshGeometry::ResizeVertexCount(FMeshDataPC& meshDataPC, uint32 count)
    {
        meshDataPC.ResizeVertexCount(count);
    }
    FMeshVertexPC& FMeshGeometry::GetVertex(FMeshDataPC& meshDataPC, uint32 index)
    {
        return meshDataPC.GetVertex(index);
    }
    uint32 FMeshGeometry::AddVertex(FMeshDataPC& meshDataPC, const FMeshVertexPC& vertex)
    {
        meshDataPC.AddVertex(vertex);
        return GetVertexCount(meshDataPC);
    }
    uint32 FMeshGeometry::AddVertex(FMeshDataPC& meshDataPC, 
                                    const FVector3& vPos,
                                    const FVector4& color)
    {
        return AddVertex(meshDataPC, FMeshVertexPC(vPos, color));
    }
    void FMeshGeometry::SetVertex(FMeshDataPC& meshDataPC, int index, const FMeshVertexPC& vertex)
    {
        meshDataPC.SetVertex(index, vertex);
    }

    uint32 FMeshGeometry::GetIndexCount(FMeshDataPC& meshDataPC)
    {
        return meshDataPC.GetIndexCount();
    }
    void FMeshGeometry::ReserveIndexCount(FMeshDataPC& meshDataPC, uint32 count)
    {
        meshDataPC.ReserveIndexCount(count);
    }
    void FMeshGeometry::ResizeIndexCount(FMeshDataPC& meshDataPC, uint32 count)
    {
        meshDataPC.ResizeIndexCount(count);
    }
    uint32 FMeshGeometry::GetIndex(FMeshDataPC& meshDataPC, uint32 index)
    {
        return meshDataPC.GetIndex(index);
    }   
    uint32 FMeshGeometry::AddIndex(FMeshDataPC& meshDataPC, uint32 value)
    {
        meshDataPC.AddIndex(value);
        return GetIndexCount(meshDataPC);
    }
    void FMeshGeometry::AddIndices(FMeshDataPC& meshDataPC, uint32 count, uint32* pIndex)
    {
        meshDataPC.AddIndices(count, pIndex);
    }
    void FMeshGeometry::SetIndex(FMeshDataPC& meshDataPC, uint32 index, uint32 value)
    {
        meshDataPC.SetIndex(index, value);
    }
    void FMeshGeometry::AddIndexLine(FMeshDataPC& meshDataPC, uint32 index1, uint32 index2)
    {
        meshDataPC.AddIndexLine(index1, index2);
    }
    void FMeshGeometry::AddIndexTriangle(FMeshDataPC& meshDataPC, uint32 index1, uint32 index2, uint32 index3)
    {
        meshDataPC.AddIndexTriangle(index1, index2, index3);
    }
    void FMeshGeometry::SetIndexTriangle(FMeshDataPC& meshDataPC, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
    {
        meshDataPC.SetIndexTriangle(indexStart, index1, index2, index3);
    }

    
    //FMeshData
    uint32 FMeshGeometry::GetVertexCount(FMeshData& meshData)
    {
        return meshData.GetVertexCount();
    }
    void FMeshGeometry::ReserveVertexCount(FMeshData& meshData, uint32 count)
    {
        meshData.ReserveVertexCount(count);
    }
    void FMeshGeometry::ResizeVertexCount(FMeshData& meshData, uint32 count)
    {
        meshData.ResizeVertexCount(count);
    }
    FMeshVertex& FMeshGeometry::GetVertex(FMeshData& meshData, uint32 index)
    {
        return meshData.GetVertex(index);
    }
    uint32 FMeshGeometry::AddVertex(FMeshData& meshData, const FMeshVertex& vertex)
    {
        meshData.AddVertex(vertex);
        return GetVertexCount(meshData);
    }
    uint32 FMeshGeometry::AddVertex(FMeshData& meshData,
                                    const FVector3& vPos,
                                    const FVector4& color, 
                                    const FVector3& normal, 
                                    const FVector3& tangent, 
                                    const FVector2& texCoord)
    {
        return AddVertex(meshData, FMeshVertex(vPos, color, normal, tangent, texCoord));
    }
    void FMeshGeometry::SetVertex(FMeshData& meshData, int index, const FMeshVertex& vertex)
    {
        meshData.SetVertex(index, vertex);
    }

    uint32 FMeshGeometry::GetIndexCount(FMeshData& meshData)
    {
        return meshData.GetIndexCount();
    }
    void FMeshGeometry::ReserveIndexCount(FMeshData& meshData, uint32 count)
    {
        meshData.ReserveIndexCount(count);
    }
    void FMeshGeometry::ResizeIndexCount(FMeshData& meshData, uint32 count)
    {
        meshData.ResizeIndexCount(count);
    }
    uint32 FMeshGeometry::GetIndex(FMeshData& meshData, uint32 index)
    {
        return meshData.GetIndex(index);
    }
    uint32 FMeshGeometry::AddIndex(FMeshData& meshData, uint32 value)
    {
        meshData.AddIndex(value);
        return GetIndexCount(meshData);
    }
    void FMeshGeometry::AddIndices(FMeshData& meshData, uint32 count, uint32* pIndex)
    {
        meshData.AddIndices(count, pIndex);
    }
    void FMeshGeometry::SetIndex(FMeshData& meshData, uint32 index, uint32 value)
    {
        meshData.SetIndex(index, value);
    }
    void FMeshGeometry::AddIndexLine(FMeshData& meshData, uint32 index1, uint32 index2)
    {
        meshData.AddIndexLine(index1, index2);
    }
    void FMeshGeometry::AddIndexTriangle(FMeshData& meshData, uint32 index1, uint32 index2, uint32 index3)
    {
        meshData.AddIndexTriangle(index1, index2, index3);
    }
    void FMeshGeometry::SetIndexTriangle(FMeshData& meshData, uint32 indexStart, uint32 index1, uint32 index2, uint32 index3)
    {
        meshData.SetIndexTriangle(indexStart, index1, index2, index3);
    }

}; //LostPeterFoundation
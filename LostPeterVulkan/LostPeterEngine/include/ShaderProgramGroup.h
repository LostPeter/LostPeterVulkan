/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SHADER_PROGRAM_GROUP_H_
#define _SHADER_PROGRAM_GROUP_H_

#include "Resource.h"

namespace LostPeterEngine
{
    class engineExport ShaderProgramGroup
    {
    public:
        ShaderProgramGroup();
        virtual ~ShaderProgramGroup();

    public:
    protected:


    public:
        void WriteConstant(uint32 nRegisterIndex, float fValue);
		void WriteConstant(uint32 nRegisterIndex, const FVector2& v2);
		void WriteConstant(uint32 nRegisterIndex, const FVector3& v3);
		void WriteConstant(uint32 nRegisterIndex, const FVector4& v4);
		void WriteConstant(uint32 nRegisterIndex, const FQuaternion& q);
		void WriteConstant(uint32 nRegisterIndex, const FMatrix4& m4);
		void WriteConstant(uint32 nRegisterIndex, const FMatrix4* pMatrix, uint32 nCount);
		void WriteConstant(uint32 nRegisterIndex, const float* pVal, uint32 nCount);

    };

}; //LostPeterEngine

#endif
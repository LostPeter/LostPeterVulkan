/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-10
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SHADER_PROGRAM_FACTORY_H_
#define _SHADER_PROGRAM_FACTORY_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport ShaderProgramFactory
    {
    public:
        ShaderProgramFactory();
        virtual ~ShaderProgramFactory();

    public:
        virtual const String& GetShaderLanguage() const = 0;

    public:
        virtual ShaderProgram* Create(ResourceManager* pResourceManager, 
                                      uint32 nGroup, 
                                      const String& strName, 
                                      const String& strGroupName, 
                                      ResourceHandle nHandle,
                                      bool bIsManualLoad,
                                      ResourceManualLoader* pResourceManualLoader) = 0;
        virtual void Destroy(ShaderProgram* pShaderProgram);
    };

}; //LostPeterEngine

#endif
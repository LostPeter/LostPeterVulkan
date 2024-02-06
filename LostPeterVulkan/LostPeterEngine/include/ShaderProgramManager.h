/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SHADER_PROGRAM_MANAGER_H_
#define _SHADER_PROGRAM_MANAGER_H_

#include "ResourceManager.h"

namespace LostPeterEngine
{
    class engineExport ShaderProgramManager : public FSingleton<ShaderProgramManager>
                                            , public ResourceManager
    {
    public:
        ShaderProgramManager();
        virtual ~ShaderProgramManager();

    public:
        static const String ms_strShaderProgramConfigName;

    protected:
        ShaderProgramSerializer* m_pShaderProgramSerializer;

    public:
        F_FORCEINLINE ShaderProgramSerializer* GetShaderProgramSerializer() const { return m_pShaderProgramSerializer; }

    public:
        static ShaderProgramManager& GetSingleton();
		static ShaderProgramManager* GetSingletonPtr();

    public:
        virtual void Destroy();
        bool Init(uint nGroup, const String& strNameCfg);

    public:
        ShaderProgram* LoadShaderProgram(uint nGroup, const String& strName, const String& strGroupName = ResourceGroupManager::ms_strNameResourceGroup_AutoDetect);

        bool HasShaderProgram(const String& strName);
        bool HasShaderProgram(const String& strName, const String& strGroupName);
        ShaderProgram* GetShaderProgram(const String& strName);
        ShaderProgram* GetShaderProgram(const String& strName, const String& strGroupName);

    private:
        ShaderProgram* loadShaderProgram(ShaderProgramInfo* pSPI);

    public:
		
    };

}; //LostPeterEngine

#endif
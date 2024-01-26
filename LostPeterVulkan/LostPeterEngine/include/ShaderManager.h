/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "ResourceManager.h"

namespace LostPeterEngine
{
    class engineExport ShaderManager : public FSingleton<ShaderManager>
                                     , public ResourceManager
    {
    public:
        ShaderManager();
        virtual ~ShaderManager();

    public:
        static const String ms_strShaderConfigName;

    protected:
        ShaderSerializer* m_pShaderSerializer;

    public:
        F_FORCEINLINE ShaderSerializer* GetShaderSerializer() const { return m_pShaderSerializer; }

    public:
        static ShaderManager& GetSingleton();
		static ShaderManager* GetSingletonPtr();

    public:
        virtual void Destroy();
        bool Init(uint nGroup, const String& strNameCfg);

    public:
        Shader* LoadShader(uint nGroup, const String& strName, const String& strGroupName = ResourceGroupManager::ms_strNameResourceGroup_AutoDetect);

        bool HasShader(const String& strName);
        bool HasShader(const String& strName, const String& strGroupName);
        Shader* GetShader(const String& strName);
        Shader* GetShader(const String& strName, const String& strGroupName);

    private:
        Shader* loadShader(ShaderInfo* pSI);

    public:
        
    };

}; //LostPeterEngine

#endif
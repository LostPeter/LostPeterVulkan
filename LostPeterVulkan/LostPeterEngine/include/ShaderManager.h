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

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ShaderManager : public FSingleton<ShaderManager>
                                     , public Base
    {
    public:
        ShaderManager();
        virtual ~ShaderManager();

    public:
        static const String ms_strShaderConfigName;

    protected:
        ShaderSerializer* m_pShaderSerializer;
        ShaderPtrVector m_aShader;
        ShaderGroupPtrMap m_mapShaderGroup;

    public:
        F_FORCEINLINE ShaderSerializer* GetShaderSerializer() const { return m_pShaderSerializer; }
        F_FORCEINLINE const ShaderPtrVector& GetShaderPtrVector() const { return m_aShader; }
        F_FORCEINLINE ShaderPtrVector& GetShaderPtrVector() { return m_aShader; }
        F_FORCEINLINE const ShaderGroupPtrMap& GetShaderGroupPtrMap() const { return m_mapShaderGroup; }
        F_FORCEINLINE ShaderGroupPtrMap& GetShaderGroupPtrMap() { return m_mapShaderGroup; }

    public:
        static ShaderManager& GetSingleton();
		static ShaderManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroup, const String& strNameCfg);

    public:
        bool LoadShaderAll();
        Shader* LoadShader(uint nGroup, const String& strName);
        void UnloadShader(Shader* pShader);

        bool HasShader(uint nGroup, const String& strName);
        Shader* GetShader(uint nGroup, const String& strName);
        bool AddShader(uint nGroup, Shader* pShader);
        void DeleteShader(uint nGroup, const String& strName);
        void DeleteShaderAll();

    private:
        Shader* loadShader(ShaderInfo* pSI);
    };

}; //LostPeterEngine

#endif
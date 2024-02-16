/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SHADER_PARAM_PASS_MANAGER_H_
#define _SHADER_PARAM_PASS_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ShaderParamPassManager : public FSingleton<ShaderParamPassManager>
                                              , public Base
    {
    public:
        ShaderParamPassManager();
        virtual ~ShaderParamPassManager();

    public: 
    protected:
        ShaderParamSourceAuto* m_pShaderParamSourceAutoGlobal;
        ShaderParamPassPtrVector m_aShaderParamPass;
        ShaderParamPassPtrMap m_mapShaderParamPass;

    public:
        static ShaderParamPassManager& GetSingleton();
		static ShaderParamPassManager* GetSingletonPtr();

    public:
        F_FORCEINLINE ShaderParamSourceAuto* GetShaderParamSourceAutoGlobal() const { return m_pShaderParamSourceAutoGlobal; }
        F_FORCEINLINE const ShaderParamPassPtrVector& GetShaderParamPassPtrVector() const { return m_aShaderParamPass; }
        F_FORCEINLINE ShaderParamPassPtrVector& GetShaderParamPassPtrVector() { return m_aShaderParamPass; }
        F_FORCEINLINE const ShaderParamPassPtrMap& GetPShaderParamPassPtrMap() const { return m_mapShaderParamPass; }
        F_FORCEINLINE ShaderParamPassPtrMap& GetPShaderParamPassPtrMap() { return m_mapShaderParamPass; }

    public:
        virtual void Destroy();
        virtual bool Init();

    protected:
        virtual bool createShaderParamSourceAutoGlobal();

    public:
        bool HasShaderParamPass(const String& strName);
        ShaderParamPass* GetShaderParamPass(const String& strName);
        bool AddShaderParamPass(ShaderParamPass* pShaderParamPass);
        ShaderParamPass* CreateShaderParamPass(FShaderPassType eShaderPass);
        void DeleteShaderParamPass(const String& strName);
        void DeleteShaderParamPass(ShaderParamPass* pShaderParamPass);
        void DeleteShaderParamPassAll();

    public:
        virtual bool GenerateShaderProgramParams(ShaderProgram* pShaderProgram) = 0;

    public:
        virtual void UpdateShaderParamGlobal();
        virtual void UpdateShaderProgramParams(ShaderProgram* pShaderProgram);
    };

}; //LostPeterEngine

#endif
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

#include "../include/ShaderParamPass.h"
#include "../include/ShaderParamPassManager.h"
#include "../include/ShaderParamSourceAuto.h"
#include "../include/ShaderParameter.h"

namespace LostPeterEngine
{
    ShaderParamPass::ShaderParamPass(const String& strNameShaderParamPass)
        : Base(strNameShaderParamPass)
        , m_pShaderParamSourceAuto(nullptr)
        , m_bIsUseShaderParamSourceAutoGlobal(true)
        , m_pShaderParameter(nullptr)
    {   

    }

    ShaderParamPass::~ShaderParamPass()
    {

    }

    void ShaderParamPass::Destroy()
    {
        if (!m_bIsUseShaderParamSourceAutoGlobal)
        {
            F_DELETE(m_pShaderParamSourceAuto)
        }
        m_pShaderParamSourceAuto = nullptr;

        F_DELETE(m_pShaderParameter)
    }

    bool ShaderParamPass::Init()
    {
        //1> createParamSourceAuto
        if (!createParamSourceAuto())
        {
            F_LogError("*********************** ShaderParamPass::Init: Failed to createParamSourceAuto, name: [%s] !", GetName().c_str());
            return false;
        }

        //2> createParamPass
        if (!createParamPass())
        {
            F_LogError("*********************** ShaderParamPass::Init: Failed to createParamPass, name: [%s] !", GetName().c_str());
            return false;
        }

        return true;
    }
        bool ShaderParamPass::createParamSourceAuto()
        {
            if (m_bIsUseShaderParamSourceAutoGlobal)
                m_pShaderParamSourceAuto = ShaderParamPassManager::GetSingleton().GetShaderParamSourceAutoGlobal();
            else
                m_pShaderParamSourceAuto = new ShaderParamSourceAuto("ParamSourceAuto_" + GetName());
            return true;
        }

}; //LostPeterEngine
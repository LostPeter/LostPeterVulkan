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

#include "../include/ShaderParamPassManager.h"
#include "../include/ShaderParamPass.h"
#include "../include/ShaderParamPassMain.h"
#include "../include/ShaderParamPassDepth.h"
#include "../include/ShaderParamPassCustom.h"
#include "../include/ShaderParamSourceAuto.h"

template<> LostPeterEngine::ShaderParamPassManager* LostPeterFoundation::FSingleton<LostPeterEngine::ShaderParamPassManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    ShaderParamPassManager* ShaderParamPassManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	ShaderParamPassManager& ShaderParamPassManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "ShaderParamPassManager::GetSingleton")
		return (*ms_Singleton);     
	}


    ShaderParamPassManager::ShaderParamPassManager()
        : Base("ShaderParamPassManager")
        , m_pShaderParamSourceAutoGlobal(nullptr)
    {

    }
    
    ShaderParamPassManager::~ShaderParamPassManager()
    {

    }

    void ShaderParamPassManager::Destroy()
    {
        F_DELETE(m_pShaderParamSourceAutoGlobal)
        DeleteShaderParamPassAll();
    }

    bool ShaderParamPassManager::Init()
    {
        //1> createShaderParamSourceAutoGlobal
        if (!createShaderParamSourceAutoGlobal())
        {
            return false;
        }



        return true;
    }
        bool ShaderParamPassManager::createShaderParamSourceAutoGlobal()
        {
            m_pShaderParamSourceAutoGlobal = new ShaderParamSourceAuto("ParamSourceAuto_Global");
            return true;
        }

    bool ShaderParamPassManager::HasShaderParamPass(const String& strName)
    {
        return GetShaderParamPass(strName) != nullptr;
    }
    ShaderParamPass* ShaderParamPassManager::GetShaderParamPass(const String& strName)
    {
        ShaderParamPassPtrMap::iterator itFind = m_mapShaderParamPass.find(strName);
        if (itFind == m_mapShaderParamPass.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool ShaderParamPassManager::AddShaderParamPass(ShaderParamPass* pShaderParamPass)
    {
        const String& strName = pShaderParamPass->GetName();
        ShaderParamPassPtrMap::iterator itFind = m_mapShaderParamPass.find(strName);
        if (itFind != m_mapShaderParamPass.end())
        {
            F_LogError("*********************** ShaderParamPassManager::AddShaderParamPass: ShaderParamPass name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapShaderParamPass.insert(ShaderParamPassPtrMap::value_type(strName, pShaderParamPass));
        m_aShaderParamPass.push_back(pShaderParamPass);
        return true;
    }
    ShaderParamPass* ShaderParamPassManager::CreateShaderParamPass(FShaderPassType eShaderPass)
    {
        const String& strName = F_GetShaderPassTypeName(eShaderPass);
        ShaderParamPass* pShaderParamPass = GetShaderParamPass(strName);
        if (pShaderParamPass != nullptr)
        {
            pShaderParamPass->AddRef();
            return pShaderParamPass;
        }

        switch ((uint32)eShaderPass)
        {
        case F_ShaderPass_Main:
            pShaderParamPass = new ShaderParamPassMain;
            break;
        case F_ShaderPass_Depth:
            pShaderParamPass = new ShaderParamPassDepth;
            break;
        case F_ShaderPass_Custom:
            pShaderParamPass = new ShaderParamPassCustom;
            break;
        
        default:
            F_Assert(false && "Wrong FShaderPassType !")
            return nullptr;
        }
        return pShaderParamPass;
    }

    void ShaderParamPassManager::DeleteShaderParamPass(const String& strName)
    {
        ShaderParamPassPtrMap::iterator itFind = m_mapShaderParamPass.find(strName);
        if (itFind == m_mapShaderParamPass.end())
        {
            return;
        }

        itFind->second->DelRef();
        if (itFind->second->CanDel())
        {
            ShaderParamPassPtrVector::iterator itFindA = std::find(m_aShaderParamPass.begin(), m_aShaderParamPass.end(), itFind->second);
            if (itFindA != m_aShaderParamPass.end())
                m_aShaderParamPass.erase(itFindA);
            F_DELETE(itFind->second)
            m_mapShaderParamPass.erase(itFind);
        }
    }
    void ShaderParamPassManager::DeleteShaderParamPass(ShaderParamPass* pShaderParamPass)
    {
        if (!pShaderParamPass)
            return;
        DeleteShaderParamPass(pShaderParamPass->GetName());
    }
    void ShaderParamPassManager::DeleteShaderParamPassAll()
    {
        for (ShaderParamPassPtrVector::iterator it = m_aShaderParamPass.begin();
             it != m_aShaderParamPass.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aShaderParamPass.clear();
        m_mapShaderParamPass.clear();
    }


    void ShaderParamPassManager::UpdateShaderParamGlobal()
    {
        
    }
    void ShaderParamPassManager::UpdateShaderProgramParams(ShaderProgram* pShaderProgram)
    {

    }

}; //LostPeterEngine
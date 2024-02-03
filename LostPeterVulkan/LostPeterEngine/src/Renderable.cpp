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

#include "../include/Renderable.h"
#include "../include/Material.h"
#include "../include/MaterialInstance.h"
#include "../include/MaterialManager.h"
#include "../include/Shader.h"

namespace LostPeterEngine
{
    Renderable::Renderable(const String& nameRenderable)
        : Base(nameRenderable)
        , m_eObject(E_Object_Mesh)
		, m_eRenderQueue(F_RenderQueue_Opaque)
        , m_eRenderPass(F_RenderPass_ForwardLit)
		, m_nRendOrderID(-1)
		, m_bIsPolygonModeOverrideable(true)
		, m_bIsUseIdentityView(false)
		, m_bIsUseIdentityProjection(false)
        , m_nGroup(FPathManager::PathGroup_Material)
        , m_strMaterialName(MaterialManager::GetMaterialName_DefaultOpaque())
		
    {
        m_pMaterialManager = MaterialManager::GetSingletonPtr();
		F_Assert(m_pMaterialManager && "Renderable::Renderable")
		m_pMaterialInstance = m_pMaterialManager->GetMaterialInstance_DefaultOpaque();
		F_Assert(m_pMaterialInstance && "Renderable::Renderable")
    }

    Renderable::~Renderable()
    {
        if (m_pMaterialInstance)
		{
			m_pMaterialManager->DestroyMaterialInstance(m_pMaterialInstance);
		}
        m_pMaterialInstance = nullptr;
    }

    MaterialInstance* Renderable::GetMaterialInstance()
	{
		return m_pMaterialInstance;
	}
	const MaterialInstance* Renderable::GetMaterialInstance() const
	{ 
		return m_pMaterialInstance; 
	}

	bool Renderable::SetMaterialInstance(MaterialInstance* pMaterialInstance)
	{
		if (!pMaterialInstance)
		{
			F_Assert(false && "Renderable::SetMaterialInstance");
			return false;
		}
		if (m_pMaterialInstance)
			m_pMaterialManager->DestroyMaterialInstance(m_pMaterialInstance);
		m_pMaterialInstance = pMaterialInstance;
		return true;
	}

	bool Renderable::SetMaterialName(const String& strMaterialName, bool bIsUnique /*= true*/)
	{
		if (strMaterialName != m_strMaterialName)
		{
			Material* pMaterial = m_pMaterialManager->GetMaterial(FPathManager::PathGroup_Material, strMaterialName);
			if (!pMaterial)
			{
				F_LogError("*********************** Renderable::SetMaterialName: Material: [%s] is not loaded !", strMaterialName.c_str());
				if (m_nGroup > 0)
					return SetMaterial(strMaterialName, m_nGroup, bIsUnique);
				else
					return false;
			}
			MaterialInstance* pMaterialInstance = m_pMaterialManager->CreateMaterialInstance(pMaterial, bIsUnique);
			if (pMaterialInstance)
			{
				if (m_pMaterialInstance)
					m_pMaterialManager->DestroyMaterialInstance(m_pMaterialInstance);
				m_pMaterialInstance = pMaterialInstance;
				m_strMaterialName = strMaterialName;
				return true;
			}
			else
			{
				F_LogError("*********************** Renderable::SetMaterialName: Create Material: [%s]'s instance failed !", strMaterialName.c_str());
				return false;
			}
		}
		return true;
	}

	bool Renderable::SetMaterial(const String& strMaterialName, uint32 nGroup, bool bIsUnique /*= true*/)
	{
		if (strMaterialName != m_strMaterialName)
		{
			MaterialInstance* pMaterialInstance = m_pMaterialManager->CreateMaterialInstance(nGroup, strMaterialName, bIsUnique);
			if (pMaterialInstance)
			{
				if (m_pMaterialInstance)
					m_pMaterialManager->DestroyMaterialInstance(m_pMaterialInstance);
				m_pMaterialInstance = pMaterialInstance;
				m_strMaterialName = strMaterialName;
				return true;
			}
			else
			{
				F_LogError("*********************** Renderable::SetMaterial: Create Material: [%s]'s instance failed !", strMaterialName.c_str());
				return false;
			}
		}
		return true;
	}

	bool Renderable::SetMaterial(Material* pMaterial)
	{
		const String& strMaterialName = pMaterial->GetName();
		if (strMaterialName != m_strMaterialName ||
			(m_pMaterialInstance && pMaterial != m_pMaterialInstance->GetMaterial()))
		{
			m_strMaterialName = strMaterialName;
			MaterialInstance* pMaterialInstance = m_pMaterialManager->CreateMaterialInstance(pMaterial, true);
			SetMaterialInstance(pMaterialInstance);
		}
		return true;
	}

    void Renderable::UpdateShaderParam(Shader* pShader)
    {

    }
    void Renderable::UpdateShaderParam(int nIndex, Shader* pShader)
    {
        
    }
    
}; //LostPeterEngine
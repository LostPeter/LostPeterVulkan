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

#include "../include/MaterialData.h"

namespace LostPeterEngine
{
    MaterialData::MaterialData(const String& nameMaterialData)
        : Base(nameMaterialData)
        , m_strPath("")
    {
        
    }
    MaterialData::~MaterialData()
    {
        Destroy();
    }

    void MaterialData::Destroy()
	{
		DeleteRenderStateAll();
	}

	bool MaterialData::HasRenderState(const String& strName)
	{
		return GetRenderState(strName) != nullptr;
	}
	RenderState* MaterialData::GetRenderState(const String& strName)
	{
		RenderStatePtrMap::iterator itFind = m_mapRenderState.find(strName);
        if (itFind == m_mapRenderState.end())
        {
            F_LogError("*********************** MaterialData::GetRenderState: Can not find, name: [%s]", strName.c_str());
            return nullptr;
        }
        return itFind->second;
	}
	bool MaterialData::AddRenderState(RenderState* pRenderState)
	{
		const String& strName = pRenderState->GetName();
        RenderStatePtrMap::iterator itFind = m_mapRenderState.find(strName);
        if (itFind != m_mapRenderState.end())
        {
            F_LogError("*********************** MaterialData::AddRenderState: RenderState name already exist: [%s] !", strName.c_str());
            F_DELETE(pRenderState)
            return false;
        }

        m_aRenderState.push_back(pRenderState);
        m_mapRenderState.insert(RenderStatePtrMap::value_type(strName, pRenderState));
        return true;
	}
	void MaterialData::DeleteRenderState(const String& strName)
	{
		RenderStatePtrMap::iterator itFind = m_mapRenderState.find(strName);
        if (itFind == m_mapRenderState.end())
        {
            return;
        }

        RenderStatePtrVector::iterator itFindA = std::find(m_aRenderState.begin(), m_aRenderState.end(), itFind->second);
        if (itFindA != m_aRenderState.end())
            m_aRenderState.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapRenderState.erase(itFind);
	}
	void MaterialData::DeleteRenderStateAll()
	{
		for (RenderStatePtrMap::iterator it = m_mapRenderState.begin();
             it != m_mapRenderState.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_aRenderState.clear();
        m_mapRenderState.clear();
	}

    void MaterialData::SerializerFrom(const MaterialData* pMaterialData)
    {

    }

}; //LostPeterEngine
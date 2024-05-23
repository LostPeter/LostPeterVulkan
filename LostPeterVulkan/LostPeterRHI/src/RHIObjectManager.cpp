/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIObjectManager.h"
#include "../include/RHIDevice.h"
#include "../include/RHIObject.h"

template<> LostPeterRHI::RHIObjectManager* LostPeterFoundation::FSingleton<LostPeterRHI::RHIObjectManager>::ms_Singleton = nullptr;

namespace LostPeterRHI
{
    String RHIObjectManager::GetAutoName(RHIObjectType eObject, uint32 nID)
    {
        return RHI_GetObjectTypeName(eObject) + "-" + FUtilString::SaveUInt(nID);
    }

    uint32 RHIObjectManager::ms_nID = 0;
    RHIObjectManager* RHIObjectManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RHIObjectManager& RHIObjectManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RHIObjectManager::GetSingleton")
		return (*ms_Singleton);     
	}

    
    RHIObjectManager::RHIObjectManager(RHIDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "RHIObjectManager::RHIObjectManager")
    }

    RHIObjectManager::~RHIObjectManager()
    {
        
    }

    bool RHIObjectManager::HasObject(uint32 nID)
    {
        return GetObject(nID) != nullptr;
    }
    bool RHIObjectManager::HasObject(const String& strName)
    {
        return GetObject(strName) != nullptr;
    }
    RHIObject* RHIObjectManager::GetObject(uint32 nID)
    {
        RHIObjectPtrIDMap::iterator itFind = m_mapID2Object.find(nID);
        if (itFind == m_mapID2Object.end())
            return nullptr;
        return itFind->second;
    }
    RHIObject* RHIObjectManager::GetObject(const String& strName)
    {
        RHIObjectPtrNameMap::iterator itFind = m_mapName2Object.find(strName);
        if (itFind == m_mapName2Object.end())
            return nullptr;
        return itFind->second;
    }
    void RHIObjectManager::AddObject(RHIObject* pObject)
    {
        F_Assert(!HasObject(pObject->GetID() && "RHIObjectManager::AddObject"))

        m_aObject.push_back(pObject);
        m_mapID2Object[pObject->GetID()] = pObject;
        m_mapName2Object[pObject->GetName()] = pObject;
        RHIObjectPtrType2VectorMap::iterator itFind = m_mapType2Vector.find(pObject->GetObjectType());
        if (itFind == m_mapType2Vector.end())
        {
            RHIObjectPtrVector aObject;
            m_mapType2Vector[pObject->GetObjectType()] = aObject;
            itFind = m_mapType2Vector.find(pObject->GetObjectType());
        }
        itFind->second.push_back(pObject);
    }
    void RHIObjectManager::DestroyObject(RHIObject* pObject)
    {
        destroyFromVector(pObject, m_aObject);
        destroyFromMap(pObject, m_mapID2Object);
        destroyFromMap(pObject, m_mapName2Object);
        RHIObjectPtrType2VectorMap::iterator itFind = m_mapType2Vector.find(pObject->GetObjectType());
        if (itFind != m_mapType2Vector.end())
        {
            destroyFromVector(pObject, itFind->second);
        }
        F_DELETE(pObject)
    }
    void RHIObjectManager::DestroyObject(uint32 nID)
    {
        RHIObject* pObject = GetObject(nID);
        if (pObject != nullptr)
        {
            DestroyObject(pObject);
        }
    }
    void RHIObjectManager::DestroyObject(const String& strName)
    {
        RHIObject* pObject = GetObject(strName);
        if (pObject != nullptr)
        {
            DestroyObject(pObject);
        }
    }
    void RHIObjectManager::DestroyObjectAll()
    {
        size_t count = m_aObject.size();
        for (size_t i = 0; i < count; i++)
        {
            F_DELETE(m_aObject[i])
        }
        m_aObject.clear();
        m_mapID2Object.clear();
        m_mapName2Object.clear();
    }

    void RHIObjectManager::destroyFromVector(RHIObject* pObject, RHIObjectPtrVector& aObject)
    {
        RHIObjectPtrVector::iterator itFind = std::find(aObject.begin(), aObject.end(), pObject);
        if (itFind != aObject.end())
        {
            aObject.erase(itFind);
        }
    }
    void RHIObjectManager::destroyFromMap(RHIObject* pObject, RHIObjectPtrIDMap& mapObject)
    {
        RHIObjectPtrIDMap::iterator itFind = mapObject.find(pObject->GetID());
        if (itFind != mapObject.end())
        {
            mapObject.erase(itFind);
        }
    }
    void RHIObjectManager::destroyFromMap(RHIObject* pObject, RHIObjectPtrNameMap& mapObject)
    {
        RHIObjectPtrNameMap::iterator itFind = mapObject.find(pObject->GetName());
        if (itFind != mapObject.end())
        {
            mapObject.erase(itFind);
        }
    }

}; //LostPeterRHI
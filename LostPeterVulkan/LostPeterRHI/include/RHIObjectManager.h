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

#ifndef _RHI_OBJECT_MANAGER_H_
#define _RHI_OBJECT_MANAGER_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIObjectManager : public FSingleton<RHIObjectManager>
    {
    public:
        RHIObjectManager(RHIDevice* pDevice);
        virtual ~RHIObjectManager();

    public:
        static uint32 ms_nID;
        static uint32 GetNextID()
        {
            return ++ms_nID;
        }
        static String GetAutoName(RHIObjectType eObject, uint32 nID);

    protected:
        RHIDevice* m_pDevice;

        RHIObjectPtrVector m_aObject;
        RHIObjectPtrIDMap m_mapID2Object;
        RHIObjectPtrNameMap m_mapName2Object;
        RHIObjectPtrType2VectorMap m_mapType2Vector;

    public:
        static RHIObjectManager& GetSingleton();
		static RHIObjectManager* GetSingletonPtr();

    public:
        F_FORCEINLINE RHIDevice* GetDevice() { return m_pDevice; }

        F_FORCEINLINE const RHIObjectPtrVector& GetObjectPtrVector() const { return m_aObject; }
        F_FORCEINLINE RHIObjectPtrVector& GetObjectPtrVector() { return m_aObject; }
        F_FORCEINLINE const RHIObjectPtrIDMap& GetObjectPtrIDMap() const { return m_mapID2Object; }
        F_FORCEINLINE RHIObjectPtrIDMap& GetObjectPtrIDMap() { return m_mapID2Object; }
        F_FORCEINLINE const RHIObjectPtrNameMap& GetObjectPtrNameMap() const { return m_mapName2Object; }
        F_FORCEINLINE RHIObjectPtrNameMap& GetObjectPtrNameMap() { return m_mapName2Object; }
        F_FORCEINLINE const RHIObjectPtrType2VectorMap& GetObjectPtrType2VectorMap() const { return m_mapType2Vector; }
        F_FORCEINLINE RHIObjectPtrType2VectorMap& GetObjectPtrType2VectorMap() { return m_mapType2Vector; }
        
    public:
        virtual void Destroy() = 0;
        
        virtual bool HasObject(uint32 nID);
        virtual bool HasObject(const String& strName);
        virtual RHIObject* GetObject(uint32 nID);
        virtual RHIObject* GetObject(const String& strName);
        virtual void AddObject(RHIObject* pObject);
        virtual void DestroyObject(RHIObject* pObject);
        virtual void DestroyObject(uint32 nID);
        virtual void DestroyObject(const String& strName);
        virtual void DestroyObjectAll();

    protected:
        void destroyFromVector(RHIObject* pObject, RHIObjectPtrVector& aObject);
        void destroyFromMap(RHIObject* pObject, RHIObjectPtrIDMap& mapObject);
        void destroyFromMap(RHIObject* pObject, RHIObjectPtrNameMap& mapObject);
    };

}; //LostPeterRHI

#endif
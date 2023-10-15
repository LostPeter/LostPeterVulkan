/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_DESCRIPTOR_SET_SERIALIZER_H_
#define _VK_DESCRIPTOR_SET_SERIALIZER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKDescriptorSetSerializer : public FSerializer
                                               , public Base
    {
    public:
        VKDescriptorSetSerializer();
        virtual ~VKDescriptorSetSerializer();

    public:
    protected:
        DescriptorSetInfoPtrVector m_aDescriptorSetInfo;
        DescriptorSetInfoPtrMap m_mapDescriptorSetInfo;

    public:
        LP_FORCEINLINE const DescriptorSetInfoPtrVector& GetDescriptorSetInfoPtrVector() const { return m_aDescriptorSetInfo; }
        LP_FORCEINLINE DescriptorSetInfoPtrVector& GetDescriptorSetInfoPtrVector() { return m_aDescriptorSetInfo; }
        LP_FORCEINLINE const DescriptorSetInfoPtrMap& GetDescriptorSetInfoPtrMap() const { return m_mapDescriptorSetInfo; }
        LP_FORCEINLINE DescriptorSetInfoPtrMap& GetDescriptorSetInfoPtrMap() { return m_mapDescriptorSetInfo; }  

    public:
        bool HasDescriptorSetInfo(const String& strName);
        DescriptorSetInfo* GetDescriptorSetInfo(const String& strName);
        bool AddDescriptorSetInfo(DescriptorSetInfo* pDescriptorSetInfo);
        void DeleteDescriptorSetInfo(const String& strName);
        void DeleteDescriptorSetInfoAll();

    protected:
        //File Content XML
        virtual bool serializeXML();
        virtual bool deserializeXML();

        //File Content Binary
        virtual bool serializeBinary();
        virtual bool deserializeBinary();
    };

}; //LostPeter

#endif
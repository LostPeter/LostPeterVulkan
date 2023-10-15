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

#ifndef _VK_DESCRIPTOR_SET_LAYOUT_SERIALIZER_H_
#define _VK_DESCRIPTOR_SET_LAYOUT_SERIALIZER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKDescriptorSetLayoutSerializer : public FSerializer
                                                     , public Base
    {
    public:
        VKDescriptorSetLayoutSerializer();
        virtual ~VKDescriptorSetLayoutSerializer();

    public:
    protected:
        DescriptorSetLayoutInfoPtrVector m_aDescriptorSetLayoutInfo;
        DescriptorSetLayoutInfoPtrMap m_mapDescriptorSetLayoutInfo;

    public:
        LP_FORCEINLINE const DescriptorSetLayoutInfoPtrVector& GetDescriptorSetLayoutInfoPtrVector() const { return m_aDescriptorSetLayoutInfo; }
        LP_FORCEINLINE DescriptorSetLayoutInfoPtrVector& GetDescriptorSetLayoutInfoPtrVector() { return m_aDescriptorSetLayoutInfo; }
        LP_FORCEINLINE const DescriptorSetLayoutInfoPtrMap& GetDescriptorSetLayoutInfoPtrMap() const { return m_mapDescriptorSetLayoutInfo; }
        LP_FORCEINLINE DescriptorSetLayoutInfoPtrMap& GetDescriptorSetLayoutInfoPtrMap() { return m_mapDescriptorSetLayoutInfo; }  

    public:
        bool HasDescriptorSetLayoutInfo(const String& strName);
        DescriptorSetLayoutInfo* GetDescriptorSetLayoutInfo(const String& strName);
        bool AddDescriptorSetLayoutInfo(DescriptorSetLayoutInfo* pDescriptorSetLayoutInfo);
        void DeleteDescriptorSetLayoutInfo(const String& strName);
        void DeleteDescriptorSetLayoutInfoAll();

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
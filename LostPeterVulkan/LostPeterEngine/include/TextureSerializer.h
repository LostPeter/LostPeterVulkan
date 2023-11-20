/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _TEXTURE_SERIALIZER_H_
#define _TEXTURE_SERIALIZER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class utilExport TextureSerializer : public FSerializer
                                       , public Base
    {
    public:
        TextureSerializer();
        virtual ~TextureSerializer();

    public:
    protected:
        TextureInfoPtrVector m_aTextureInfo;
        TextureInfoGroupPtrMap m_mapTextureInfoGroup;

    public:
        LP_FORCEINLINE const TextureInfoPtrVector& GetTextureInfoPtrVector() const { return m_aTextureInfo; }
        LP_FORCEINLINE TextureInfoPtrVector& GetTextureInfoPtrVector() { return m_aTextureInfo; }
        LP_FORCEINLINE const TextureInfoGroupPtrMap& GetTextureInfoGroupPtrMap() const { return m_mapTextureInfoGroup; }
        LP_FORCEINLINE TextureInfoGroupPtrMap& GetTextureInfoGroupPtrMap() { return m_mapTextureInfoGroup; }


    public:
        bool HasTextureInfo(uint nGroup, const String& strName);
        TextureInfo* GetTextureInfo(uint nGroup, const String& strName);
        bool AddTextureInfo(TextureInfo* pTextureInfo);
        void DeleteTextureInfo(uint nGroup, const String& strName);
        void DeleteTextureInfoAll();

    protected:
        //File Content XML
        virtual bool serializeXML();
        virtual bool deserializeXML();

        //File Content Binary
        virtual bool serializeBinary();
        virtual bool deserializeBinary();
    };

}; //LostPeterEngine

#endif
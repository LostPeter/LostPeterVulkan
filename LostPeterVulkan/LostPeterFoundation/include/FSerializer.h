/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_SERIALIZER_H_
#define _F_SERIALIZER_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class foundationExport FSerializer
    {
    public:
        FSerializer();
        virtual ~FSerializer();

    public:
    protected:
        bool m_bIsUseFileXML;

        FFileXML* m_pFileXML;
        FFileMemory* m_pFileMemory;

    public:
        F_FORCEINLINE bool IsUseFileXML() const { return m_bIsUseFileXML; }
        F_FORCEINLINE bool IsUseFileBinary() const { return !m_bIsUseFileXML; }
        F_FORCEINLINE void SetIsUseFileXML() { m_bIsUseFileXML = true; }
        F_FORCEINLINE void SetIsUseFileBinary() { m_bIsUseFileXML = false; }

    public:
        void Destroy();

    ////File
    public:   
        virtual void CreateFile();
        virtual bool CreateFile(const String& strContent);
        virtual bool CreateFile(const char* szContent);
        virtual bool CreateFile(uint8* pContent, int32 size);
        virtual bool LoadFile(const String& strPath);
        virtual bool SaveFile(const String& strPath);

    protected:
        //File XML
        virtual void createFileXML();
        virtual bool createFileXML(const String& strContent);
        virtual bool createFileXML(const char* szContent);
        virtual bool loadFileXML(const String& strPath);
        virtual bool saveFileXML(const String& strPath);

        //File Binary
        virtual void createFileBinary();
        virtual bool createFileBinary(uint8* pContent, int32 size);
        virtual bool loadFileBinary(const String& strPath);
        virtual bool saveFileBinary(const String& strPath);


    ////File Content
    public:    
        virtual bool Serialize();
        virtual bool Deserialize();

    protected:
        //File Content XML
        virtual bool serializeXML() = 0;
        virtual bool deserializeXML() = 0;

        //File Content Binary
        virtual bool serializeBinary() = 0;
        virtual bool deserializeBinary() = 0;
    };

}; //LostPeterFoundation

#endif
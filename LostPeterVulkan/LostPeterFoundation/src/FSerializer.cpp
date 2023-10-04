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

#include "../include/FSerializer.h"
#include "../include/FFileXML.h"
#include "../include/FFileMemory.h"
#include "../include/FUtil.h"

namespace LostPeterFoundation
{
    FSerializer::FSerializer()
        : m_bIsUseFileXML(true)
        , m_pFileXML(nullptr)
        , m_pFileMemory(nullptr)
    {

    }
    
    FSerializer::~FSerializer()
    {
        Destroy();
    }

    void FSerializer::Destroy()
    {
        F_DELETE(m_pFileXML)
        F_DELETE(m_pFileMemory)
    }

    void FSerializer::CreateFile()
    {
        if (IsUseFileXML())
        {
            createFileXML();
            return;
        }
        createFileBinary();
    }
    bool FSerializer::CreateFile(const String& strContent)
    {
        if (IsUseFileXML())
            return createFileXML(strContent);
        return false;
    }
    bool FSerializer::CreateFile(const char* szContent)
    {
        if (IsUseFileXML())
            return createFileXML(szContent);
        return false;
    }
    bool FSerializer::CreateFile(uint8* pContent, int32 size)
    {
        if (IsUseFileXML())
            return false;
        return createFileBinary(pContent, size);
    }
    bool FSerializer::LoadFile(const String& strPath)
    {
        if (IsUseFileXML())
            return loadFileXML(strPath);
        return loadFileBinary(strPath);
    }
    bool FSerializer::SaveFile(const String& strPath)
    {
        if (IsUseFileXML())
            return saveFileXML(strPath);
        return saveFileBinary(strPath);
    }

    //File XML
    void FSerializer::createFileXML()
    {
        if (m_pFileXML == nullptr)
        {
            m_pFileXML = new FFileXML();
        }
        m_pFileXML->CreateXML();
    }
    bool FSerializer::createFileXML(const String& strContent)
    {
        if (m_pFileXML == nullptr)
        {
            m_pFileXML = new FFileXML();
        }
        return m_pFileXML->CreateXML(strContent);
    }
    bool FSerializer::createFileXML(const char* szContent)
    {
        if (m_pFileXML == nullptr)
        {
            m_pFileXML = new FFileXML();
        }
        return m_pFileXML->CreateXML(szContent);
    }
    bool FSerializer::loadFileXML(const String& strPath)
    {
        CharVector aConfig;
        if (!FUtil::LoadAssetFileContent(strPath.c_str(), aConfig, true))
        {
            F_LogError("*********************** FSerializer::loadFileXML: LoadAssetFileContent failed, path: [%s]", strPath.c_str());
            return false;
        }
        if (!createFileXML(aConfig.data()))
        {
            F_LogError("*********************** FSerializer::loadFileXML: createFileXML failed, path: [%s]", strPath.c_str());
            return false;
        }
        
        if (!Deserialize())
        {
            F_LogError("*********************** FSerializer::loadFileXML: Deserialize() failed, path: [%s]", strPath.c_str());
            return false;
        }

        return true;
    }
    bool FSerializer::saveFileXML(const String& strPath)
    {
        
        return false;
    }

    //File Binary
    void FSerializer::createFileBinary()
    {
        if (m_pFileMemory == nullptr)
        {
            m_pFileMemory = new FFileMemory();
        }
    }
    bool FSerializer::createFileBinary(uint8* pContent, int32 size)
    {
        createFileBinary();

        return false;
    }
    bool FSerializer::loadFileBinary(const String& strPath)
    {
        uint8* pData;
        int32 sizeData = 0;
        if (!FUtil::LoadAssetFileToBuffer(strPath.c_str(), &pData, sizeData, true))
        {
            F_LogError("*********************** FSerializer::loadFileBinary: LoadAssetFileToBuffer failed, path: [%s]", strPath.c_str());
            return false;
        }
        if (!createFileBinary(pData, sizeData))
        {
            F_LogError("*********************** FSerializer::loadFileBinary: createFileBinary failed, path: [%s]", strPath.c_str());
            return false;
        }
        
        if (!Deserialize())
        {
            F_LogError("*********************** FSerializer::loadFileBinary: Deserialize() failed, path: [%s]", strPath.c_str());
            return false;
        }

        return true;
    }
    bool FSerializer::saveFileBinary(const String& strPath)
    {

        return false;
    }


    bool FSerializer::Serialize()
    {
        if (IsUseFileXML())
            return serializeXML();
        return serializeBinary();
    }
    bool FSerializer::Deserialize()
    {
        if (IsUseFileXML())
            return deserializeXML();
        return deserializeBinary();
    }
    
}; //LostPeterFoundation
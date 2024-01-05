/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FArchiveFileSystemFactory.h"
#include "../include/FArchiveFileSystem.h"

namespace LostPeterFoundation
{
    String FArchiveFileSystemFactory::ms_strFactoryType = "ArchiveFileSystem";
    FArchiveFileSystemFactory::FArchiveFileSystemFactory()
    {

    }

    FArchiveFileSystemFactory::~FArchiveFileSystemFactory() 
    {

    }

    const String& FArchiveFileSystemFactory::GetType() const
    {
        return ms_strFactoryType;
    }

    FArchive* FArchiveFileSystemFactory::CreateInstance(const String& strName) 
    {
        return new FArchiveFileSystem(strName, GetType());
    }

    void FArchiveFileSystemFactory::DestroyInstance(FArchive* pInstance)
    { 
        F_DELETE(pInstance)
    }
    
}; //LostPeterFoundation
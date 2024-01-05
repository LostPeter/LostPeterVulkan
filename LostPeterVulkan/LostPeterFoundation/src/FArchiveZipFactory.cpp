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

#include "../include/FArchiveZipFactory.h"
#include "../include/FArchiveZip.h"

namespace LostPeterFoundation
{
    String FArchiveZipFactory::ms_strFactoryType = "ArchiveZip";
    FArchiveZipFactory::FArchiveZipFactory()
    {

    }

    FArchiveZipFactory::~FArchiveZipFactory() 
    {

    }

    const String& FArchiveZipFactory::GetType() const
    {
        return ms_strFactoryType;
    }

    FArchive* FArchiveZipFactory::CreateInstance(const String& strName) 
    {
        return new FArchiveZip(strName, GetType());
    }

    void FArchiveZipFactory::DestroyInstance(FArchive* pInstance)
    { 
        F_DELETE(pInstance)
    }
    
}; //LostPeterFoundation
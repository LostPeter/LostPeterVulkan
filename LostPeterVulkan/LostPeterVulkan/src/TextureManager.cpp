/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/TextureManager.h"
#include "../include/VulkanWindow.h"
#include "../include/TextureSerializer.h"

template<> LostPeter::TextureManager* LostPeterFoundation::FSingleton<LostPeter::TextureManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    TextureManager* TextureManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	TextureManager& TextureManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "TextureManager::GetSingleton")
		return (*ms_Singleton);  
	}

    TextureManager::TextureManager()
        : Base("TextureManager")
        , m_pTextureSerializer(nullptr)
    {

    }
    TextureManager::~TextureManager()
    {
        
    }

    void TextureManager::Destroy()
    {
        F_DELETE(m_pTextureSerializer)
    }
    bool TextureManager::Init(uint nGroup, const String& strNameCfgTexture)
    {
        

        return true;
    }

}; //LostPeter
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

#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport TextureManager : public FSingleton<TextureManager>
                                    , public Base
    {
    public:
        TextureManager();
        virtual ~TextureManager();

    public:
    protected:
        TextureSerializer* m_pTextureSerializer;
        TexturePtrVector m_aTexture;
        TextureGroupPtrMap m_mapTextureGroup;

    public:
        static TextureManager& GetSingleton();
		static TextureManager* GetSingletonPtr();

    public:
        void Destroy();
        bool Init(uint nGroup, const String& strNameCfgTexture);

    public:
        bool LoadTextureAll();
        Texture* LoadTexture(uint nGroup, const String& strName);

        bool HasTexture(uint nGroup, const String& strName);
        Texture* GetTexture(uint nGroup, const String& strName);
        bool AddTexture(uint nGroup, Texture* pTexture);
        void DeleteTexture(uint nGroup, const String& strName);
        void DeleteTextureAll();

    private:
        Texture* loadTexture(TextureInfo* pTI);
    };

}; //LostPeter

#endif
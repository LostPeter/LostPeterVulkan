/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _CULL_TERRAIN_STATIC_H_
#define _CULL_TERRAIN_STATIC_H_

#include "Base.h"
#include "CullUnitTerrain.h"

namespace LostPeterVulkan
{
    ////////////////////// CullTerrainStatic /////////////////////////
    class vulkanExport CullTerrainStatic : public FSingleton<CullTerrainStatic>
                                         , public Base
    {
    public:
        CullTerrainStatic();
        virtual ~CullTerrainStatic();

    public:
        ////////////////////// CullUnitTerrainStatic /////////////////////
        class vulkanExport CullUnitTerrainStatic : public CullUnitTerrain
        {
        public:
            CullUnitTerrainStatic(const String& nameUnit, CullTerrainStatic* pCullTS);
            virtual ~CullUnitTerrainStatic();

        public:
            CullTerrainStatic* pCullTerrainStatic;


        public:
            virtual void Destroy();
            virtual void Init();

        public:
            virtual bool IsCulling();
            virtual int GetRenderCount();  
            virtual ComputeBuffer* GetRenderArgsCB();
        };
        typedef std::vector<CullUnitTerrainStatic*> CullUnitTerrainStaticPtrVector;
        typedef std::map<String, CullUnitTerrainStatic*> CullUnitTerrainStaticPtrMap;


    public:
        static const String s_nameCullTerrainStatic;
        static const String s_nameCullUnitTerrainStatic;
        

    public:
        CullUnitTerrainStatic* pCullUnitTerrainStatic;

    public:
        static CullTerrainStatic& GetSingleton();
		static CullTerrainStatic* GetSingletonPtr();

    public:
        void Destroy();
        void Init();
    };

}; //LostPeterVulkan

#endif
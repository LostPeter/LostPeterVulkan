/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VERTEX_DECLARATION_MANAGER_H_
#define _VERTEX_DECLARATION_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport VertexDeclarationManager : public FSingleton<VertexDeclarationManager>
                                                , public Base
    {
    public:
        VertexDeclarationManager();
        ~VertexDeclarationManager();

    public:
		static size_t ms_nVDIncrementNum;

	protected:
		uint32 m_nCount;
		VertexDeclarationPtrVector m_aVDPool;
		VertexDeclarationPtrList m_listVDFree;
		VertexDeclarationPtrMap m_mapVD;
        //ENGINE_MUTEX(m_mutexVertexDecl)

    public:
        static VertexDeclarationManager& GetSingleton();
        static VertexDeclarationManager* GetSingletonPtr();

    public:
		E_FORCEINLINE uint32 GetVertexDeclarationCount() { return m_nCount; }
		VertexDeclaration* GetSameVertexDeclaration(VertexDeclaration* pVertexDeclaration);

	public:
		virtual VertexDeclaration* CreateVertexDeclaration();
		virtual void DestroyVertexDeclaration(VertexDeclaration* pVertexDeclaration);
		virtual void DestroyVertexDeclarationAll();

	protected:
		virtual VertexDeclaration* createVertexDeclarationImpl();
		virtual void destroyVertexDeclarationImpl(VertexDeclaration* pVertexDeclaration);
    };

}; //LostPeterEngine

#endif
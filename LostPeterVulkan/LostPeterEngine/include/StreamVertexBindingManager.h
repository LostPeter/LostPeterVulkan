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

#ifndef _STREAM_VERTEX_BINDING_MANAGER_H_
#define _STREAM_VERTEX_BINDING_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport StreamVertexBindingManager : public FSingleton<StreamVertexBindingManager>
                                                  , public Base
    {
    public:
        StreamVertexBindingManager();
        ~StreamVertexBindingManager();

    public:
	protected:
		uint32 m_nCount;
		VertexStreamBindingPtrListNode m_vsbActiveHeadNode;
		VertexStreamBindingPtrListNode m_vsbActiveTailNode;
		VertexStreamBindingPtrListNode m_vsbFreeHeadNode;
		VertexStreamBindingPtrListNode m_vsbFreeTailNode;
		//ENGINE_MUTEX(m_mutexVertexBindings)

    public:
        static StreamVertexBindingManager& GetSingleton();
        static StreamVertexBindingManager* GetSingletonPtr();

    public:
		virtual StreamVertexBinding* CreateVertexStreamBinding();
		virtual void DestroyVertexStreamBinding(StreamVertexBinding* pVSBinding);
		virtual void DestroyVertexStreamBindingAll();

	protected:
		virtual StreamVertexBinding* createVertexStreamBindingImpl();
		virtual void destroyVertexStreamBindingImpl(StreamVertexBinding* pVSBinding);
    };

}; //LostPeterEngine

#endif
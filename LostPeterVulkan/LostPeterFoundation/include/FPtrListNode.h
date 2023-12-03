/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_PTR_LIST_NODE_H_
#define _F_PTR_LIST_NODE_H_

#include "FTPoint.h"

namespace LostPeterFoundation
{
    template<class T>
	class FPtrListNode
	{
	public:
		FPtrListNode()
			: m_pPointer(0)
			, m_pNext(0)
			, m_pPrev(0)
		  {

		  }

		  ~FPtrListNode()
		  {

		  }

	public:
		T* m_pPointer;
		FPtrListNode* m_pNext;
		FPtrListNode* m_pPrev;
	};

}; //LostPeterFoundation

#endif
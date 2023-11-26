/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _MOVABLE_FACTORY_H_
#define _MOVABLE_FACTORY_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class utilExport MovableFactory
    {
    public:
        MovableFactory();
        virtual ~MovableFactory();

    public:
    protected:
		uint32 m_nTypeFlag;
		bool m_bDelExternal;

	public:
		LP_FORCEINLINE uint32 GetTypeFlags() const { return m_nTypeFlag;  }
		LP_FORCEINLINE void	SetTypeFlags(uint32 nTypeFlag) { m_nTypeFlag = nTypeFlag; }
		LP_FORCEINLINE bool GetIsDelExternal() const { return m_bDelExternal; }
		LP_FORCEINLINE void SetIsDelExternal(bool b) { m_bDelExternal = b; }

	public:			
		virtual const String& GetMovableType() const = 0;

		virtual Movable* CreateInstance(const String& strName, const String2StringMap* pParams = nullptr);
		virtual void DestroyInstance(Movable* pMovable) = 0;

		virtual bool RequestTypeFlags() const { return false;  }

	protected:
		virtual Movable* createInstanceImpl(const String& strName, const String2StringMap* pParams = nullptr) = 0;
    };

}; //LostPeterEngine

#endif
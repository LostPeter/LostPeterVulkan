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

#ifndef _VERTEX_ELEMENT_H_
#define _VERTEX_ELEMENT_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport VertexElement
	{
	public:	
		VertexElement();
		VertexElement(uint16 nSourceIndex,    
                      size_t nOffset, 
                      EVertexElementSemanticType typeSemantic,
					  EVertexElementDataType typeData,
                      uint16 nIndex = 0);
		~VertexElement();
		
	public:
		uint16 m_nSourceIndex;
		size_t m_nOffset;
		EVertexElementSemanticType m_eVertexElementSemantic;
		EVertexElementDataType m_eVertexElementData;
		uint16 m_nIndex;	

	public:
		E_FORCEINLINE uint16 GetSourceIndex() const { return m_nSourceIndex; }
		E_FORCEINLINE size_t GetOffset() const { return m_nOffset; }
		E_FORCEINLINE EVertexElementSemanticType GetVertexElementSemanticType() const { return m_eVertexElementSemantic; }
		E_FORCEINLINE EVertexElementDataType GetVertexElementDataType() const { return m_eVertexElementData; }
		E_FORCEINLINE uint16 GetIndex() const { return m_nIndex; }
	
	public:
		void GetElementKey(String& strKey);
			
	public:
		size_t GetSize() const;
		static size_t GetTypeSize(EVertexElementDataType typeData);
		static uint16 GetTypeCount(EVertexElementDataType typeData);
		static EVertexElementDataType MultiplyTypeCount(EVertexElementDataType typeData, uint16 nCount);
		static EVertexElementDataType GetBaseType(EVertexElementDataType typeData);
		static void	ConvertColorValue(EVertexElementDataType typeDataSrc, EVertexElementDataType typeDataDst, uint32* ptr);
		static uint32 ConvertColorValue(const FColor& srcColor, EVertexElementDataType typeData);
		static EVertexElementDataType GetBestColorVertexElementType();

		E_FORCEINLINE void BaseVertexPointerToElement(void* pBase, void** pElem) const
		{
			*pElem = static_cast<void*>(static_cast<uint8*>(pBase) + m_nOffset);
		}

		E_FORCEINLINE void BaseVertexPointerToElement(void* pBase, float** pElem) const
		{
			*pElem = static_cast<float*>(static_cast<void*>(static_cast<uint8*>(pBase) + m_nOffset));
		}

		E_FORCEINLINE void BaseVertexPointerToElement(void* pBase, RGBA** pElem) const
		{
			*pElem = static_cast<RGBA*>(static_cast<void*>(static_cast<uint8*>(pBase) + m_nOffset));
		}

		E_FORCEINLINE void BaseVertexPointerToElement(void* pBase, uint8** pElem) const
		{
			*pElem = static_cast<uint8*>(pBase) + m_nOffset;
		}

		E_FORCEINLINE void BaseVertexPointerToElement(void* pBase, uint16** pElem) const
		{
			*pElem = static_cast<uint16*>(pBase) + m_nOffset;
		}
	};

}; //LostPeterEngine

#endif
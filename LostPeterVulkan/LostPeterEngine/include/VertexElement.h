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
                      FVertexElementSemanticType typeSemantic,
					  FVertexElementDataType typeData,
                      uint16 nIndex = 0);
		~VertexElement();
		
	public:
		uint16 m_nSourceIndex;
		size_t m_nOffset;
		FVertexElementSemanticType m_eVertexElementSemantic;
		FVertexElementDataType m_eVertexElementData;
		uint16 m_nIndex;	

	public:
		F_FORCEINLINE uint16 GetSourceIndex() const { return m_nSourceIndex; }
		F_FORCEINLINE size_t GetOffset() const { return m_nOffset; }
		F_FORCEINLINE FVertexElementSemanticType GetVertexElementSemanticType() const { return m_eVertexElementSemantic; }
		F_FORCEINLINE FVertexElementDataType GetVertexElementDataType() const { return m_eVertexElementData; }
		F_FORCEINLINE uint16 GetIndex() const { return m_nIndex; }
	
	public:
		void GetElementKey(String& strKey);
			
	public:
		size_t GetSize() const;
		static size_t GetTypeSize(FVertexElementDataType typeData);
		static uint16 GetTypeCount(FVertexElementDataType typeData);
		static FVertexElementDataType MultiplyTypeCount(FVertexElementDataType typeData, uint16 nCount);
		static FVertexElementDataType GetBaseType(FVertexElementDataType typeData);
		static void	ConvertColorValue(FVertexElementDataType typeDataSrc, FVertexElementDataType typeDataDst, uint32* ptr);
		static uint32 ConvertColorValue(const FColor& srcColor, FVertexElementDataType typeData);
		static FVertexElementDataType GetBestColorVertexElementType();

		F_FORCEINLINE void BaseVertexPointerToElement(void* pBase, void** pElem) const
		{
			*pElem = static_cast<void*>(static_cast<uint8*>(pBase) + m_nOffset);
		}

		F_FORCEINLINE void BaseVertexPointerToElement(void* pBase, float** pElem) const
		{
			*pElem = static_cast<float*>(static_cast<void*>(static_cast<uint8*>(pBase) + m_nOffset));
		}

		F_FORCEINLINE void BaseVertexPointerToElement(void* pBase, RGBA** pElem) const
		{
			*pElem = static_cast<RGBA*>(static_cast<void*>(static_cast<uint8*>(pBase) + m_nOffset));
		}

		F_FORCEINLINE void BaseVertexPointerToElement(void* pBase, uint8** pElem) const
		{
			*pElem = static_cast<uint8*>(pBase) + m_nOffset;
		}

		F_FORCEINLINE void BaseVertexPointerToElement(void* pBase, uint16** pElem) const
		{
			*pElem = static_cast<uint16*>(pBase) + m_nOffset;
		}
	};

}; //LostPeterEngine

#endif
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

#include "../include/VertexElement.h"
#include "../include/RenderEngine.h"
#include "../include/Renderer.h"

namespace LostPeterEngine
{
    VertexElement::VertexElement()
		: m_nSourceIndex(0)
		, m_nOffset(0)
		, m_eVertexElementSemantic(E_VertexElementSemantic_Position)
		, m_eVertexElementData(E_VertexElementData_Float3)
		, m_nIndex(0)
	{

	}

	VertexElement::VertexElement(uint16 nSourceIndex,
                                 size_t nOffset,
                                 EVertexElementSemanticType typeSemantic,
		                         EVertexElementDataType typeData,
                                 uint16 nIndex /*= 0*/)
		: m_nSourceIndex(nSourceIndex)
		, m_nOffset(nOffset)
		, m_eVertexElementSemantic(typeSemantic)
		, m_eVertexElementData(typeData)
		, m_nIndex(nIndex)
	{

	}

	VertexElement::~VertexElement()
	{

	}

	void VertexElement::GetElementKey(String& strKey)
	{
		/*/////////////////////////////
		32 bit
			                     | SourceIndex(4b)
		******** ******** **** **** **** ****
		  Offset(16b)		|		  |	   | Semantic(4b)
                         Index(4b) DataType(4b)
		/////////////////////////////*/
		uint32 nSemantic = (uint32)m_eVertexElementSemantic; 
		uint32 nDataType = (uint32)m_eVertexElementData; 
		uint32 nSourceIndex = (uint32)m_nSourceIndex; 
		uint32 nIndex = (uint32)m_nIndex; 
		uint32 nOffset = (uint32)m_nOffset; 
		
		uint32 nKey = 0x00000000;
		nKey |= nSemantic;
		nKey |= nDataType<<4;
		nKey |= nSourceIndex<<8;
		nKey |= nIndex<<12;
		nKey |= nOffset<<16;
		strKey = FUtilString::SaveUInt(nKey);
	}

	size_t VertexElement::GetSize(void) const
	{
		return GetTypeSize(m_eVertexElementData);
	}

	size_t VertexElement::GetTypeSize(EVertexElementDataType typeData)
	{
		switch ((int32)typeData)
		{
		case E_VertexElementData_Color:
		case E_VertexElementData_ColorARGB:
		case E_VertexElementData_ColorABGR:
			return sizeof(RGBA);
		case E_VertexElementData_Float1:
			return sizeof(float);
		case E_VertexElementData_Float2:
			return sizeof(float) * 2;
		case E_VertexElementData_Float3:
			return sizeof(float) * 3;
		case E_VertexElementData_Float4:
			return sizeof(float) * 4;
		case E_VertexElementData_Short1:
			return sizeof(short);
		case E_VertexElementData_Short2:
			return sizeof(short) * 2;
		case E_VertexElementData_Short3:
			return sizeof(short) * 3;
		case E_VertexElementData_Short4:
			return sizeof(short) * 4;
		case E_VertexElementData_UByte4:
			return sizeof(uint8) * 4;
		default:
			F_Assert(false && "VertexElement::GetTypeSize: Invalid type !")
		}
		return 0;
	}

	uint16 VertexElement::GetTypeCount(EVertexElementDataType typeData)
	{
		switch ((int32)typeData)
		{
		case E_VertexElementData_Color:
		case E_VertexElementData_ColorARGB:
		case E_VertexElementData_ColorABGR:
			return 1;
		case E_VertexElementData_Float1:
			return 1;
		case E_VertexElementData_Float2:
			return 2;
		case E_VertexElementData_Float3:
			return 3;
		case E_VertexElementData_Float4:
			return 4;
		case E_VertexElementData_Short1:
			return 1;
		case E_VertexElementData_Short2:
			return 2;
		case E_VertexElementData_Short3:
			return 3;
		case E_VertexElementData_Short4:
			return 4;
		case E_VertexElementData_UByte4:
			return 4;
		default:
			F_Assert(false && "VertexElement::GetTypeCount: Invalid type !")
		}
		return 0;
	}

	EVertexElementDataType VertexElement::MultiplyTypeCount(EVertexElementDataType typeData, uint16 nCount)
	{
		switch (typeData)
		{
		case E_VertexElementData_Float1:
			switch (nCount)
			{
			case 1:
				return E_VertexElementData_Float1;
			case 2:
				return E_VertexElementData_Float2;
			case 3:
				return E_VertexElementData_Float3;
			case 4:
				return E_VertexElementData_Float4;
			default:
				break;
			}
			break;
		case E_VertexElementData_Short1:
			switch (nCount)
			{
			case 1:
				return E_VertexElementData_Short1;
			case 2:
				return E_VertexElementData_Short2;
			case 3:
				return E_VertexElementData_Short3;
			case 4:
				return E_VertexElementData_Short4;
			default:
				break;
			}
			break;
		default:
			F_Assert(false && "VertexElement::MultiplyTypeCount: Invalid base type !")
		}

		return E_VertexElementData_Float1;
	}

	EVertexElementDataType VertexElement::GetBaseType(EVertexElementDataType typeData)
	{
		switch ((int32)typeData)
		{
		case E_VertexElementData_Float1:
		case E_VertexElementData_Float2:
		case E_VertexElementData_Float3:
		case E_VertexElementData_Float4:
			return E_VertexElementData_Float1;
		case E_VertexElementData_Color:
			return E_VertexElementData_Color;
		case E_VertexElementData_ColorARGB:
			return E_VertexElementData_ColorARGB;
		case E_VertexElementData_ColorABGR:
			return E_VertexElementData_ColorABGR;
		case E_VertexElementData_Short1:
		case E_VertexElementData_Short2:
		case E_VertexElementData_Short3:
		case E_VertexElementData_Short4:
			return E_VertexElementData_Short1;
		case E_VertexElementData_UByte4:
			return E_VertexElementData_UByte4;
		};
		return E_VertexElementData_Float1;
	}

	void VertexElement::ConvertColorValue(EVertexElementDataType typeDataSrc, EVertexElementDataType typeDataDst, uint32* ptr)
	{
		if (typeDataSrc == typeDataDst)
			return;

		//ARGB -> ABGR
		*ptr = ((*ptr & 0x00FF0000) >> 16) | ((*ptr & 0x000000FF) << 16) | (*ptr & 0xFF00FF00);	
	}

	uint32 VertexElement::ConvertColorValue(const FColor& srcColor, EVertexElementDataType typeData)
	{
		switch ((int32)typeData)
		{
		#if F_PLATFORM == F_PLATFORM_WINDOW
			default:
		#endif
		case E_VertexElementData_ColorARGB:
			return FMath::GetAsARGB(srcColor);
		#if F_PLATFORM != F_PLATFORM_WINDOW
			default:
		#endif
		case E_VertexElementData_ColorABGR: 
			return FMath::GetAsABGR(srcColor);
		};
	}

	EVertexElementDataType VertexElement::GetBestColorVertexElementType(void)
	{
		RenderEngine* pRenderEngine = RenderEngine::GetSingletonPtr();
		if (pRenderEngine && pRenderEngine->GetRendererCurrent())
		{
			return pRenderEngine->GetRendererCurrent()->GetColorVertexElementType();
		}
		else
		{
		#if F_PLATFORM == F_PLATFORM_WINDOW
			return E_VertexElementData_ColorARGB; // prefer D3D format on windows
		#else
			return E_VertexElementData_ColorABGR; // prefer GL format on everything else
		#endif
		}
	}

}; //LostPeterEngine
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

#ifndef _VERTEX_DECLARATION_H_
#define _VERTEX_DECLARATION_H_

#include "VertexElement.h"

namespace LostPeterEngine
{
    class engineExport VertexDeclaration 
	{
	public:
		VertexDeclaration();
		virtual ~VertexDeclaration();
		
	public:
	protected:
		VertexElementList m_listElement;
		VertexSizeVector m_aVertexSize;			
		int32 m_nPoolIndex;
		bool m_bSharedVD;

	public:
		void GetVertexDeclKey(String& strKey);

	public:
		F_FORCEINLINE size_t GetVertexElementCount() { return m_listElement.size(); }
		F_FORCEINLINE const VertexElementList& GetVertexElementList() const { return m_listElement; }
		F_FORCEINLINE int32 GetPoolIndex() { return m_nPoolIndex; }
		F_FORCEINLINE void SetPoolIndex(int32 nIndex) { m_nPoolIndex = nIndex; }
		F_FORCEINLINE bool GetSharedVD() const { return m_bSharedVD; }
		F_FORCEINLINE void SetSharedVD(bool b) { m_bSharedVD = b; }
		
		const VertexElement* GetVertexElement(uint16 nIndex);

		static bool	VertexElementLess(const VertexElement& e1,const VertexElement& e2);
		void Sort();
		void Clear();
		void CloseGapsInSource();

		VertexDeclaration* GetAutoOrganizedDeclaration(bool bSkeletalAnimation, bool bVertexAnimation);

		uint16 GetMaxSource() const;

	public:	
		virtual const VertexElement& AddVertexElement(uint16 nSourceIndex, size_t nOffset, FVertexElementSemanticType typeSemantic, FVertexElementDataType typeData, uint16 nIndex = 0);
		virtual const VertexElement& InsertVertexElement(uint16 atPosition, uint16 nSourceIndex, size_t nOffset, FVertexElementSemanticType typeSemantic, FVertexElementDataType typeData, uint16 nIndex = 0);

		virtual void RemoveVertexElement(uint16 nIndex);
		virtual void RemoveVertexElement(FVertexElementSemanticType typeSemantic, uint16 nIndex = 0);
		virtual void RemoveVertexElementAll();

		virtual void ModifyVertexElement(uint16 nElementIndex, uint16 nSourceIndex, size_t nOffset, FVertexElementSemanticType typeSemantic, FVertexElementDataType typeData, uint16 nIndex = 0);

		virtual const VertexElement* FindVertexElementBySemantic(FVertexElementSemanticType typeSemantic, uint16 nIndex = 0);
		virtual VertexElementList FindVertexElementsBySource(uint16 nSourceIndex);

		virtual size_t GetVertexSize(uint16 nSourceIndex);

		virtual VertexDeclaration* Clone();
	};

}; //LostPeterEngine

#endif
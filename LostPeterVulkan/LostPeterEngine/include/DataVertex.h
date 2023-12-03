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

#ifndef _DATA_VERTEX_H_
#define _DATA_VERTEX_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport DataVertex : public FNonCopyable
    {
    public:
        DataVertex();
		DataVertex(VertexDeclaration* pDecl, bool bDelVD, StreamVertexBinding* pBinding, bool bDelVB);
		virtual ~DataVertex();

	public:
	private:
		VertexDeclaration* m_pVertexDeclaration;
		StreamVertexBinding* m_pVertexStreamBinding;
		bool m_bDel_VertexDeclaration;
		bool m_bDel_VertexStreamBinding;

		size_t m_nVertexStart;
		size_t m_nVertexCount;

	////Animation Data
		// AnimationDataVector m_aAnimationData;
		// size_t m_nAnimDataItemsUsed;
	
	public:
		E_FORCEINLINE const VertexDeclaration* GetVertexDeclaration() const { return m_pVertexDeclaration; }
		E_FORCEINLINE VertexDeclaration* GetVertexDeclaration() { return m_pVertexDeclaration; }
		E_FORCEINLINE void SetVertexDeclaration(VertexDeclaration* pDecl) { m_pVertexDeclaration = pDecl; }
		E_FORCEINLINE StreamVertexBinding*	GetVertexStreamBinding() { return m_pVertexStreamBinding; }
		E_FORCEINLINE size_t GetVertexStart() const { return m_nVertexStart; }
		E_FORCEINLINE void SetVertexStart(size_t n) { m_nVertexStart = n; }
		E_FORCEINLINE size_t GetVertexCount() const { return m_nVertexCount; }
		E_FORCEINLINE void SetVertexCount(size_t n) { m_nVertexCount = n; }
		E_FORCEINLINE size_t IncreaseVertexCount() { return ++m_nVertexCount; }
		E_FORCEINLINE void IncreaseVertexCount(size_t n) { m_nVertexCount += n; }
		E_FORCEINLINE void DecreaseVertexCount(size_t n) { m_nVertexCount -= n; }

		// E_FORCEINLINE AnimationDataVector& GetAnimationData() { return m_aAnimationData; }
		// E_FORCEINLINE size_t GetAnimDataUsed() const { return m_nAnimDataItemsUsed; }
		// E_FORCEINLINE void	SetAnimDataUsed(size_t n) { m_nAnimDataItemsUsed = n; }
		// E_FORCEINLINE size_t IncreaseAnimDataUsed() { return ++m_nAnimDataItemsUsed; }
		// E_FORCEINLINE void	IncreaseAnimDataUsed(size_t n) { m_nAnimDataItemsUsed += n; }
		
	public:
		void Destroy();

    public:
		DataVertex* Clone(bool bCopyData = true, bool bSharedVD = false) const;
		bool UpdateVertexDeclarationPointer();
		
		void CloseGapsInBindings();

		//void AllocateAnimationElements(uint16 nCount);
    };

}; //LostPeterEngine

#endif
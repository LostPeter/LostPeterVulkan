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
		DataVertex(VertexDeclaration* pVertexDeclaration, bool bDelVD, 
				   StreamVertexBinding* pBinding, bool bDelVB);
		virtual ~DataVertex();

	public:
	protected:
	////VertexDeclaration/StreamVertexBinding
		VertexDeclaration* m_pVertexDeclaration;
		StreamVertexBinding* m_pVertexStreamBinding;
		bool m_bDel_VertexDeclaration;
		bool m_bDel_VertexStreamBinding;

	////Vertex Data
		uint32 m_nVertexStart;
		uint32 m_nVertexCount;

	////Animation Data
		// AnimationDataVector m_aAnimationData;
		// uint32 m_nAnimDataItemsUsed;
	
	public:
		F_FORCEINLINE const VertexDeclaration* GetVertexDeclaration() const { return m_pVertexDeclaration; }
		F_FORCEINLINE VertexDeclaration* GetVertexDeclaration() { return m_pVertexDeclaration; }
		F_FORCEINLINE void SetVertexDeclaration(VertexDeclaration* pVertexDeclaration) { m_pVertexDeclaration = pVertexDeclaration; }
		F_FORCEINLINE StreamVertexBinding* GetVertexStreamBinding() { return m_pVertexStreamBinding; }
		F_FORCEINLINE uint32 GetVertexStart() const { return m_nVertexStart; }
		F_FORCEINLINE void SetVertexStart(uint32 nVertexStart) { m_nVertexStart = nVertexStart; }
		F_FORCEINLINE uint32 GetVertexCount() const { return m_nVertexCount; }
		F_FORCEINLINE void SetVertexCount(uint32 nVertexCount) { m_nVertexCount = nVertexCount; }
		F_FORCEINLINE uint32 IncreaseVertexCount() { return ++m_nVertexCount; }
		F_FORCEINLINE void IncreaseVertexCount(uint32 nVertexCount) { m_nVertexCount += nVertexCount; }
		F_FORCEINLINE void DecreaseVertexCount(uint32 nVertexCount) { m_nVertexCount -= nVertexCount; }

		// F_FORCEINLINE AnimationDataVector& GetAnimationData() { return m_aAnimationData; }
		// F_FORCEINLINE uint32 GetAnimDataUsed() const { return m_nAnimDataItemsUsed; }
		// F_FORCEINLINE void	SetAnimDataUsed(uint32 n) { m_nAnimDataItemsUsed = n; }
		// F_FORCEINLINE uint32 IncreaseAnimDataUsed() { return ++m_nAnimDataItemsUsed; }
		// F_FORCEINLINE void	IncreaseAnimDataUsed(uint32 n) { m_nAnimDataItemsUsed += n; }
		
	public:
		void Destroy();

    public:
		DataVertex* Clone(bool bIsCopyData = true, bool bIsSharedVD = false) const;
		bool UpdateVertexDeclaration();
		
		void CloseGapsInBindings();

		//void AllocateAnimationElements(uint16 nCount);
    };

}; //LostPeterEngine

#endif
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

#ifndef _STREAM_VERTEX_BINDING_H_
#define _STREAM_VERTEX_BINDING_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport StreamVertexBinding
	{
		friend class StreamVertexBindingManager;
	
	private:
		StreamVertexBinding();
	public:
		virtual ~StreamVertexBinding();

	public:
		typedef std::map<uint16, uint16> BindingIndexMap;

	protected:
		StreamVertexPtrMap m_mapBinding;			
		mutable uint16 m_nMaxIndex;
		void* m_pCreator;

	public:
		F_FORCEINLINE uint32 GetStreamCount() const { return (uint32)m_mapBinding.size(); }
		F_FORCEINLINE const StreamVertexPtrMap& GetBindings() const { return m_mapBinding; }
		F_FORCEINLINE uint16 GetNextIndex() const { return m_nMaxIndex++; }
		F_FORCEINLINE void* GetCreator() { return m_pCreator; }
		F_FORCEINLINE void SetCreator(void* pCreator) { m_pCreator = pCreator; }
		
	public:
		void Destroy();
	
    public:
		virtual void SetBinding(uint16 nIndex, StreamVertex* pVertexStream);
		virtual StreamVertex* UnsetBinding(uint16 nIndex);
		virtual StreamVertexPtrVector UnsetAllBindings();

		virtual StreamVertex* GetStreamVertex(uint16 nIndex) const;
		virtual bool IsStreamBound(uint16 nIndex) const;
		virtual uint16 GetLastBoundIndex() const;
		virtual bool HasGaps() const;
		virtual void CloseGaps(BindingIndexMap& bindingIndexMap);
	};

}; //LostPeterEngine

#endif
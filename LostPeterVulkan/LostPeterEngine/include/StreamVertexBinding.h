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
	public:
		StreamVertexBinding();
		virtual ~StreamVertexBinding();

	public:
		typedef std::map<uint16, uint16> BindingIndexMap;

	protected:
		StreamVertexPtrMap	m_mapBinding;			
		mutable uint16 m_nMaxIndex;
		void* m_pCreator;

	public:
		E_FORCEINLINE size_t GetStreamCount() const { return m_mapBinding.size(); }
		E_FORCEINLINE const StreamVertexPtrMap& GetBindings() const { return m_mapBinding; }
		E_FORCEINLINE uint16 GetNextIndex() const { return m_nMaxIndex++; }
		E_FORCEINLINE void* GetCreator() { return m_pCreator; }
		E_FORCEINLINE void SetCreator(void* pCreator) { m_pCreator = pCreator; }
		
	public:
		void Destroy();
	
    public:
		virtual void SetBinding(uint16 nIndex, StreamVertex* pVertexStream);
		virtual StreamVertex* UnsetBinding(uint16 nIndex);
		virtual StreamVertexPtrVector UnsetAllBindings();

		virtual StreamVertex* GetVertexStream(uint16 nIndex) const;
		virtual bool IsStreamBound(uint16 nIndex) const;
		virtual uint16 GetLastBoundIndex() const;
		virtual bool HasGaps() const;
		virtual void CloseGaps(BindingIndexMap& bindingIndexMap);
	};

}; //LostPeterEngine

#endif
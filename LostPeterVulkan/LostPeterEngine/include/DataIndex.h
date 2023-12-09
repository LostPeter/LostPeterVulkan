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

#ifndef _DATA_INDEX_H_
#define _DATA_INDEX_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport DataIndex : public FNonCopyable
	{
	public:
		DataIndex();
		DataIndex(StreamIndex* pStreamIndex, size_t nStart, size_t nCount, bool bDel);
		virtual ~DataIndex();
	
	public:
	private:
		StreamIndex* m_pStreamIndex;
		bool m_bDelete;

		size_t m_nIndexStart;		
		size_t m_nIndexCount;		

		uint32 m_nBatchCount;
		//BatchInstance* m_pBatchInstance;

	public:
		F_FORCEINLINE StreamIndex* GetStreamIndex() { return m_pStreamIndex; }
		F_FORCEINLINE void SetStreamIndex(StreamIndex* pStreamIndex) { m_pStreamIndex = pStreamIndex; }
		F_FORCEINLINE size_t GetIndexStart() const	{ return m_nIndexStart; }
		F_FORCEINLINE void	SetIndexStart(size_t n)	{ m_nIndexStart = n; }
		F_FORCEINLINE size_t GetIndexCount() const	{ return m_nIndexCount; }
		F_FORCEINLINE void	SetIndexCount(size_t n)	{ m_nIndexCount = n; }
		F_FORCEINLINE size_t IncreaseIndexCount() { return ++m_nIndexCount; }
		F_FORCEINLINE void	IncreaseIndexCount(size_t n) { m_nIndexCount += n; }
		F_FORCEINLINE uint32 GetBatchCount() const { return m_nBatchCount; }
		F_FORCEINLINE void	SetBatchCount(uint32 n)	{ m_nBatchCount = n; }
		// F_FORCEINLINE BatchInstance* GetBatchInstance() const { return m_pBatchInstance; }
		// F_FORCEINLINE void	SetBatchInstance(BatchInstance* p) { m_pBatchInstance = p; }

	public:
		void Destroy();

    public:
		DataIndex* Clone(bool bCopyData = true) const;
	};

}; //LostPeterEngine

#endif
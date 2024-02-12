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

#include "../include/StreamManager.h"
#include "../include/StreamVertex.h"
#include "../include/StreamIndex.h"
#include "../include/StreamTemp.h"

template<> LostPeterEngine::StreamManager* LostPeterFoundation::FSingleton<LostPeterEngine::StreamManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    StreamManager* StreamManager::GetSingletonPtr()
    {
        return ms_Singleton;
    }
    StreamManager& StreamManager::GetSingleton()
    {  
        F_Assert(ms_Singleton && "StreamManager::GetSingleton")
        return (*ms_Singleton);  
    }

    const size_t StreamManager::ms_nUnderUsedFrameThreshold = 30000;
	const size_t StreamManager::ms_nExpiredDelayFrameThreshold = 5;
    StreamManager::StreamManager()
        : Base("StreamManager")
        , m_nUnderUsedFrameCount(0)
    {

    }

    StreamManager::~StreamManager()
    {
        Destroy();
    }

    void StreamManager::Destroy()
	{	
		//vertex
		{
			StreamVertexPtrSet::iterator it,itEnd;
			itEnd = m_setStreamVertex.end();
			for (it = m_setStreamVertex.begin(); it != itEnd; ++it)
			{
				delete (*it);
			}
			m_setStreamVertex.clear();
		}
		//index
		{
			StreamIndexPtrSet::iterator it,itEnd;
			itEnd = m_setStreamIndex.end();
			for (it = m_setStreamIndex.begin(); it != itEnd; ++it)
			{
				delete (*it);
			}
			m_setStreamIndex.clear();
		}
	}

	void StreamManager::DestroyStreamVertex(StreamVertex* pStreamVertex)
	{
		//ENGINE_LOCK_MUTEX(m_mutexVertexStreams)

		StreamVertexPtrSet::iterator itFind = m_setStreamVertex.find(pStreamVertex);
		if (itFind != m_setStreamVertex.end())
			m_setStreamVertex.erase(itFind);
		F_DELETE(pStreamVertex)
	}

	void StreamManager::DestroyStreamIndex(StreamIndex* pStreamIndex)
	{
		//ENGINE_LOCK_MUTEX(m_mutexIndexStreams)

		StreamIndexPtrSet::iterator itFind = m_setStreamIndex.find(pStreamIndex);
		if (itFind != m_setStreamIndex.end())
			m_setStreamIndex.erase(itFind);
		F_DELETE(pStreamIndex)
	}

	StreamVertex* StreamManager::AllocateStreamVertexCopy(StreamVertex* pStreamVertexSrc, EStreamReleaseType eStreamRelease,
														  StreamTemp* pStreamTemp, bool bIsCopyData /*= false*/)
	{
		//ENGINE_LOCK_MUTEX(m_mutexVertexStreams)
		{
			//ENGINE_LOCK_MUTEX(m_mutexTempStreams)
			StreamVertex* pStreamVertex = 0;
			FreeTemporaryVertexStreamMap::iterator itFind = m_mapFreeTempVertexStream.find(pStreamVertexSrc);
			if (itFind == m_mapFreeTempVertexStream.end())
			{
				pStreamVertex = makeStreamCopy(pStreamVertexSrc, E_StreamUsage_DynamicWriteOnlyDiscardable, true);
			}
			else
			{
				pStreamVertex = itFind->second;
				m_mapFreeTempVertexStream.erase(itFind);
			}

			if (bIsCopyData)
			{
				pStreamVertex->CopyData(*pStreamVertexSrc, 0, 0, pStreamVertexSrc->GetStreamSizeInBytes(), true);
			}
			m_mapTempVertexStreamInfos.insert(TemporaryVertexStreamInfoMap::value_type(pStreamVertex, VertexStreamInfo(pStreamVertexSrc, eStreamRelease, ms_nExpiredDelayFrameThreshold, pStreamVertex, pStreamTemp)));
			return pStreamVertex;
		}
	}

	void StreamManager::ReleaseStreamVertexCopy(StreamVertex* pStreamVertex)
	{
		//ENGINE_LOCK_MUTEX(m_mutexTempStreams)

		TemporaryVertexStreamInfoMap::iterator itFind = m_mapTempVertexStreamInfos.find(pStreamVertex);
		if (itFind != m_mapTempVertexStreamInfos.end())
		{
			const VertexStreamInfo& vsi = itFind->second;
			vsi.m_pStreamTemp->DeleteStream(vsi.m_pStreamVertex);
		
			m_mapFreeTempVertexStream.insert(FreeTemporaryVertexStreamMap::value_type(vsi.m_pStreamVertexOriginal, vsi.m_pStreamVertex));
			m_mapTempVertexStreamInfos.erase(itFind);
		}
	}

	void StreamManager::TouchStreamVertexCopy(StreamVertex* pStreamVertex)
	{
		//ENGINE_LOCK_MUTEX(m_mutexTempStreams)

		TemporaryVertexStreamInfoMap::iterator itFind = m_mapTempVertexStreamInfos.find(pStreamVertex);
		if (itFind != m_mapTempVertexStreamInfos.end())
		{
			VertexStreamInfo& vsi = itFind->second;
			F_Assert(vsi.m_eStreamRelease == E_StreamRelease_Auto && "StreamManager::TouchStreamVertexCopy")
			vsi.m_nExpiredDelay = ms_nExpiredDelayFrameThreshold;
		}
	}

	void StreamManager::FreeUnusedStreamCopies()
	{
		//ENGINE_LOCK_MUTEX(m_mutexTempStreams)

		size_t nNumFreed = 0;
		FreeTemporaryVertexStreamMap::iterator it,itEnd;
		itEnd = m_mapFreeTempVertexStream.end();
		for (it = m_mapFreeTempVertexStream.begin(); it != itEnd; ++it)
		{
			StreamVertex* pStreamVertex = it->second;
			//if (pStreamVertex->GetRef() <= 1)
			{
				DestroyStreamVertex(pStreamVertex);
				m_mapFreeTempVertexStream.erase(it);

				++nNumFreed;
			}
		}
		F_LogInfo("StreamManager::FreeUnusedStreamCopies: Destroy temp stream num: [%d] !", nNumFreed);
	}

	void StreamManager::ReleaseStreamCopies(bool bForceFreeUnused /*= false*/)
	{
		//ENGINE_LOCK_MUTEX(m_mutexTempStreams)

		size_t nNumUnused = m_mapFreeTempVertexStream.size();
		size_t nNumUsed = m_mapTempVertexStreamInfos.size();

		TemporaryVertexStreamInfoMap::iterator it,itEnd;
		itEnd = m_mapTempVertexStreamInfos.end();
		for (it = m_mapTempVertexStreamInfos.begin(); it != itEnd; ++it)
		{
			VertexStreamInfo& vsi = it->second;
			if (vsi.m_eStreamRelease == E_StreamRelease_Auto &&
				(bForceFreeUnused || --vsi.m_nExpiredDelay <= 0))
			{
				vsi.m_pStreamTemp->DeleteStream(vsi.m_pStreamVertex);

				m_mapFreeTempVertexStream.insert(FreeTemporaryVertexStreamMap::value_type(vsi.m_pStreamVertexOriginal, vsi.m_pStreamVertex));
				m_mapTempVertexStreamInfos.erase(it);
			}
		}
		
		if (bForceFreeUnused)
		{
			FreeUnusedStreamCopies();
			m_nUnderUsedFrameCount = 0;
		}
		else
		{
			if (nNumUsed < nNumUnused)
			{
				++m_nUnderUsedFrameCount;
				if (m_nUnderUsedFrameCount >= ms_nUnderUsedFrameThreshold)
				{
					FreeUnusedStreamCopies();
					m_nUnderUsedFrameCount = 0;
				}
			}
			else
			{
				m_nUnderUsedFrameCount = 0;
			}
		}
	}

	void StreamManager::ForceReleaseStreamCopies(StreamVertex* pStreamVertex)
	{
		//ENGINE_LOCK_MUTEX(m_mutexTempStreams)

		TemporaryVertexStreamInfoMap::iterator it,itEnd;
		itEnd = m_mapTempVertexStreamInfos.end();
		for (it = m_mapTempVertexStreamInfos.begin(); it != itEnd; ++it)
		{
			const VertexStreamInfo& vsi = it->second;
			if (vsi.m_pStreamVertexOriginal == pStreamVertex)
			{
				vsi.m_pStreamTemp->DeleteStream(vsi.m_pStreamVertex);
				m_mapTempVertexStreamInfos.erase(it);
			}
		}

		typedef FreeTemporaryVertexStreamMap::iterator _Iter;
		std::pair<_Iter, _Iter> range = m_mapFreeTempVertexStream.equal_range(pStreamVertex);
		if (range.first != range.second)
		{
			std::list<StreamVertex*> holdForDelayDestroy;
			for (_Iter it = range.first; it != range.second; ++it)
			{
				StreamVertex* pStreamVertex = it->second;
				//if (pStreamVertex->GetRef() <= 1)
				{
					DestroyStreamVertex(pStreamVertex);
					holdForDelayDestroy.push_back(pStreamVertex);
				}
			}
			m_mapFreeTempVertexStream.erase(range.first, range.second);
		}
	}

	StreamVertex* StreamManager::makeStreamCopy(StreamVertex* pStreamVertexSrc, EStreamUsageType eStreamUsage, bool bUseShadowStream)
	{
		StreamVertex* pStreamVertex = this->CreateStreamVertex(pStreamVertexSrc->GetStreamVertexSize(), pStreamVertexSrc->GetStreamVertexNum(), eStreamUsage, bUseShadowStream);
		//pStreamVertex->AddRef();
		return pStreamVertex;
	}

}; //LostPeterEngine
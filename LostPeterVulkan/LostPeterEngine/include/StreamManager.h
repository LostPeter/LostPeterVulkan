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

#ifndef _STREAM_MANAGER_H_
#define _STREAM_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport StreamManager : public FSingleton<StreamManager>
                                     , public Base
    {
    public:
        StreamManager();
        virtual ~StreamManager();

    public:
        class engineExport VertexStreamInfo
		{
		public:
			VertexStreamInfo(StreamVertex* pStreamVertexOriginal, EStreamReleaseType eStreamRelease, size_t nDelay,
							 StreamVertex* pStreamVertex, StreamTemp* pStreamTemp) 
				: m_pStreamVertexOriginal(pStreamVertexOriginal)
				, m_eStreamRelease(eStreamRelease)
				, m_nExpiredDelay(nDelay)
				, m_pStreamVertex(pStreamVertex)
				, m_pStreamTemp(pStreamTemp)
			{

			}

		public:
			StreamVertex* m_pStreamVertexOriginal;
			EStreamReleaseType m_eStreamRelease;
			size_t m_nExpiredDelay;
			StreamVertex* m_pStreamVertex;
			StreamTemp* m_pStreamTemp;
		};
		typedef std::multimap<StreamVertex*, StreamVertex*> FreeTemporaryVertexStreamMap;
		typedef std::map<StreamVertex*, VertexStreamInfo> TemporaryVertexStreamInfoMap;

    protected:		
        static const size_t ms_nUnderUsedFrameThreshold;
		static const size_t ms_nExpiredDelayFrameThreshold;

		StreamVertexPtrSet m_setStreamVertex;
		StreamIndexPtrSet m_setStreamIndex;

		FreeTemporaryVertexStreamMap m_mapFreeTempVertexStream;
		TemporaryVertexStreamInfoMap m_mapTempVertexStreamInfos;

		size_t m_nUnderUsedFrameCount;

		//ENGINE_MUTEX(m_mutexVertexStreams)
		//ENGINE_MUTEX(m_mutexIndexStreams)

		//ENGINE_MUTEX(m_mutexTempStreams)

    public:
        static StreamManager& GetSingleton();
        static StreamManager* GetSingletonPtr();

    public:
		void Destroy();

	public:
		virtual StreamVertex* CreateStreamVertex(size_t nVertexSize, size_t nVertexNum, EStreamUsageType eStreamUsage, bool bUseShadowStream = false) = 0;
		virtual StreamVertex* CreateStreamVertex(size_t nSizeInBytes, EStreamUsageType eStreamUsage, bool bUseShadowStream = false) = 0;
		virtual void DestroyStreamVertex(StreamVertex* pStreamVertex);

		virtual StreamIndex* CreateStreamIndex(EStreamIndexType eStreamIndex, size_t nIndexNum, EStreamUsageType eStreamUsage, bool bUseShadowStream = false) = 0;
		virtual void DestroyStreamIndex(StreamIndex* pStreamIndex);

		virtual StreamVertex* AllocateStreamVertexCopy(StreamVertex* pStreamVertexSrc, EStreamReleaseType eStreamRelease,
													   StreamTemp* pStreamTemp, bool bIsCopyData = false);
		virtual void ReleaseStreamVertexCopy(StreamVertex* pStreamVertex); 
		virtual void TouchStreamVertexCopy(StreamVertex* pStreamVertex);

		virtual void FreeUnusedStreamCopies();
		virtual void ReleaseStreamCopies(bool bForceFreeUnused = false);
		
		virtual void ForceReleaseStreamCopies(StreamVertex* pStreamVertex);

	protected:
		StreamVertex* makeStreamCopy(StreamVertex* pStreamVertexSrc, EStreamUsageType eStreamUsage, bool bUseShadowStream);
    };

}; //LostPeterEngine

#endif
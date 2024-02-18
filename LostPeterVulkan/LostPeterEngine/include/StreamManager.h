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
			VertexStreamInfo(StreamVertex* pStreamVertexOriginal, 
							 EStreamReleaseType eStreamRelease, 
							 uint32 nDelay,
							 StreamVertex* pStreamVertex, 
							 StreamTemp* pStreamTemp) 
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
			uint32 m_nExpiredDelay;
			StreamVertex* m_pStreamVertex;
			StreamTemp* m_pStreamTemp;
		};
		typedef std::multimap<StreamVertex*, StreamVertex*> FreeTemporaryVertexStreamMap;
		typedef std::map<StreamVertex*, VertexStreamInfo> TemporaryVertexStreamInfoMap;

    protected:		
        static const uint32 ms_nUnderUsedFrameThreshold;
		static const uint32 ms_nExpiredDelayFrameThreshold;

		StreamVertexPtrSet m_setStreamVertex;
		StreamIndexPtrSet m_setStreamIndex;
		StreamUniformPtrSet m_setStreamUniform;

		FreeTemporaryVertexStreamMap m_mapFreeTempVertexStream;
		TemporaryVertexStreamInfoMap m_mapTempVertexStreamInfos;

		uint32 m_nUnderUsedFrameCount;

		//ENGINE_MUTEX(m_mutexVertexStreams)
		//ENGINE_MUTEX(m_mutexIndexStreams)
		//ENGINE_MUTEX(m_mutexUniformStreams)

		//ENGINE_MUTEX(m_mutexTempStreams)

    public:
        static StreamManager& GetSingleton();
        static StreamManager* GetSingletonPtr();

    public:
		void Destroy();

	public:
		//StreamVertex
		virtual StreamVertex* CreateStreamVertex(uint32 nVertexSize, uint32 nVertexCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false) = 0;
		virtual StreamVertex* CreateStreamVertex(uint32 nSizeInBytes, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false) = 0;
		virtual void DestroyStreamVertex(StreamVertex* pStreamVertex);

		//StreamIndex
		virtual StreamIndex* CreateStreamIndex(EStreamIndexType eStreamIndex, uint32 nIndexCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false) = 0;
		virtual void DestroyStreamIndex(StreamIndex* pStreamIndex);

		//StreamUniform
		virtual StreamUniform* CreateStreamUniform(uint32 nUniformSize, uint32 nUniformCount, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false) = 0;
		virtual StreamUniform* CreateStreamUniform(uint32 nSizeInBytes, EStreamUsageType eStreamUsage, bool bIsUseShadowStream = false) = 0;
		virtual void DestroyStreamUniform(StreamUniform* pStreamUniform);

	public:
		virtual StreamVertex* AllocateStreamVertexCopy(StreamVertex* pStreamVertexSrc, EStreamReleaseType eStreamRelease,
													   StreamTemp* pStreamTemp, bool bIsCopyData = false);
		virtual void ReleaseStreamVertexCopy(StreamVertex* pStreamVertex); 
		virtual void TouchStreamVertexCopy(StreamVertex* pStreamVertex);

		virtual void FreeUnusedStreamCopies();
		virtual void ReleaseStreamCopies(bool bForceFreeUnused = false);
		
		virtual void ForceReleaseStreamCopies(StreamVertex* pStreamVertex);

	protected:
		StreamVertex* makeStreamCopy(StreamVertex* pStreamVertexSrc, EStreamUsageType eStreamUsage, bool bIsUseShadowStream);
    };

}; //LostPeterEngine

#endif
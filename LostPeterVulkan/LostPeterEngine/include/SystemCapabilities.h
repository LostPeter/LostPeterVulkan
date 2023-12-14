/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SYSTEM_CAPABILITIES_H_
#define _SYSTEM_CAPABILITIES_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport SystemCapabilities : public FSingleton<SystemCapabilities>
                                          , public Base
    {
    public:
        SystemCapabilities();
        virtual ~SystemCapabilities();

    public:
    protected:
    ////CPU
		StringVector m_aCPUKeys;
		String2StringMap m_mapCPUInfo;
		Int2IntMap m_mapCPUCapabilities;
		bool m_bCPUInfoInit;
	////Memory

    public:
		static const String& GetCpuIdentifier();
		static uint32 GetCpuFeatures();
		static bool HasCpuFeature(ECpuFeatureType eCPUFeature);

    public:
        static SystemCapabilities& GetSingleton();
		static SystemCapabilities* GetSingletonPtr();

     public:
		F_FORCEINLINE const StringVector& GetCPUKeys() const { return m_aCPUKeys; }
		F_FORCEINLINE const String2StringMap& GetCPUInfos() const { return m_mapCPUInfo; }
		F_FORCEINLINE const Int2IntMap& GetCPUCapabilities() const { return m_mapCPUCapabilities; }
	
	public:
		bool HasCPUCapability(int32 nKey);
		bool HasCPUCapability(ECpuInfoType eType);

    public:
        void Destroy();
        void Init();

    private:
		void initCPUInfos();
		void initMemoryInfos();

	private:
		void logInfos();
    };

}; //LostPeterEngine

#endif
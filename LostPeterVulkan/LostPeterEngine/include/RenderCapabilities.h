/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-14
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_CAPABILITIES_H_
#define _RENDER_CAPABILITIES_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport GpuDriverVersion
	{
	public:
		GpuDriverVersion();

	public:
		int32 m_nMajor;
		int32 m_nMinor;
		int32 m_nRelease;
		int32 m_nBuild;

	public:
		String ToString() const;

		void FromString(const String& strVersion);
	};


	class engineExport RenderCapabilities : public FSingleton<RenderCapabilities>
                                          , public Base
	{
	public:
		RenderCapabilities();
		virtual ~RenderCapabilities();

	public:
	protected:
		GpuDriverVersion m_gpuDriverVersion;
		EGpuVendorType m_eGpuVendor;
		String m_strNameDevice;
		String m_strNameRenderer;

	protected:
		uint32 m_nCapabilities[E_RenderCapabilityCategory_Count];
		bool m_bCapabilities[E_RenderCapabilityCategory_Count];

		uint16 m_nStencilBufferBitDepth;
		uint16 m_nNumVertexAttributes;
		uint16 m_nNumVertexTextureUnits;
		uint16 m_nNumTextureUnits;
		uint16 m_nNumMultiRenderTargets;
		float m_fMaxPointSize;
		float m_fMaxSupportedAnisotropy;
		
		String m_strMaxVertexProgramVersion;
		String m_strMaxFragmentProgramVersion;
		StringSet m_setShaderProfiles;

		uint16 m_nVertexProgramConstantFloatCount;   
		uint16 m_nGeometryProgramConstantFloatCount;
		uint16 m_nTessellationHullProgramConstantFloatCount;
		uint16 m_nTessellationDomainProgramConstantFloatCount;
		uint16 m_nComputeProgramConstantFloatCount;
		uint16 m_nFragmentProgramConstantFloatCount; 

		bool m_bNonPOW2TexturesLimited;
		bool m_bVertexTextureUnitsShared;
		bool m_bRenderTargetFormatNull;
		bool m_bDSTFormatD16;
		bool m_bDSTFormatD24S8;
		bool m_bDSTFormatD24X8;

    public:
        static RenderCapabilities& GetSingleton();
		static RenderCapabilities* GetSingletonPtr();

	public:
		static String GetGpuVendorName(EGpuVendorType eGpuVendor)
		{
			return E_GetGpuVendorTypeName(eGpuVendor);
		}
	
	public:
		F_FORCEINLINE const GpuDriverVersion& GetGpuDriverVersion() const { return m_gpuDriverVersion; }
		F_FORCEINLINE void SetGpuDriverVersion(const GpuDriverVersion& gpuVersion) { m_gpuDriverVersion = gpuVersion; }
		F_FORCEINLINE EGpuVendorType GetGpuVendorType() const { return m_eGpuVendor; }
		F_FORCEINLINE void SetGpuVendorType(EGpuVendorType eGpuVendor) { m_eGpuVendor = eGpuVendor; }

		F_FORCEINLINE const String& GetNameDevice() const { return m_strNameDevice; }
		F_FORCEINLINE void SetNameDevice(const String& nameDevice) { m_strNameDevice = nameDevice; }
		F_FORCEINLINE const String& GetNameRenderer() const { return m_strNameRenderer; }
        F_FORCEINLINE void SetNameRenderer(const String& nameRenderer) { m_strNameRenderer = nameRenderer; }

	public:
		F_FORCEINLINE bool IsCategory(ERenderCapabilityCategoryType eRenderCapabilityCategory)
        {
            return m_bCapabilities[eRenderCapabilityCategory];
        }
		F_FORCEINLINE void SetCategory(ERenderCapabilityCategoryType eRenderCapabilityCategory, bool enable)
		{
			m_bCapabilities[eRenderCapabilityCategory] = enable;
		}

		F_FORCEINLINE bool	HasCapabilityRendererSpecific(const ERenderCapabilityType eRenderCapability) const
		{
			int32 cat = eRenderCapability >> E_RC_CAPS_BITSHIFT;
            if (cat == E_RenderCapabilityCategory_DirectX || 
				cat == E_RenderCapabilityCategory_Vulkan ||
				cat == E_RenderCapabilityCategory_Metal ||
				cat == E_RenderCapabilityCategory_OpenGL)
                return true;
            return false;
		}

		F_FORCEINLINE bool HasCapability(const ERenderCapabilityType eRenderCapability) const
        {
            int32 index = (E_RC_CAPS_CATEGORY_MASK & eRenderCapability) >> E_RC_CAPS_BITSHIFT;
            if (m_nCapabilities[index] & (eRenderCapability & ~E_RC_CAPS_CATEGORY_MASK))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        F_FORCEINLINE void SetCapability(const ERenderCapabilityType eRenderCapability) 
        { 
			int32 index = (E_RC_CAPS_CATEGORY_MASK & eRenderCapability) >> E_RC_CAPS_BITSHIFT;
            m_nCapabilities[index] |= (eRenderCapability & ~E_RC_CAPS_CATEGORY_MASK);
        }

		F_FORCEINLINE void UnsetCapability(const ERenderCapabilityType eRenderCapability) 
        { 
            int32 index = (E_RC_CAPS_CATEGORY_MASK & eRenderCapability) >> E_RC_CAPS_BITSHIFT;
            m_nCapabilities[index] &= (~eRenderCapability | E_RC_CAPS_CATEGORY_MASK);
        }

	public:
		F_FORCEINLINE uint16 GetStencilBufferBitDepth() const { return m_nStencilBufferBitDepth; }
		F_FORCEINLINE void SetStencilBufferBitDepth(uint16 n) { m_nStencilBufferBitDepth = n; }
		F_FORCEINLINE uint16 GetNumVertexAttributes() const { return m_nNumVertexAttributes; }
		F_FORCEINLINE void SetNumVertexAttributes(uint16 n) { m_nNumVertexAttributes = n; }
		F_FORCEINLINE uint16 GetNumVertexTextureUnits() const { return m_nNumVertexTextureUnits; }
		F_FORCEINLINE void SetNumVertexTextureUnits(uint16 n) { m_nNumVertexTextureUnits = n; }
		F_FORCEINLINE uint16 GetNumTextureUnits() const { return m_nNumTextureUnits; }
		F_FORCEINLINE void SetNumTextureUnits(uint16 n) { m_nNumTextureUnits = n; }
		F_FORCEINLINE uint16 GetNumRenderMultiTargets() const { return m_nNumMultiRenderTargets; }
		F_FORCEINLINE void SetNumRenderMultiTargets(uint16 n) { m_nNumMultiRenderTargets = n; }
		F_FORCEINLINE float	GetMaxPointSize() const	{ return m_fMaxPointSize; }
		F_FORCEINLINE void SetMaxPointSize(float f)	{ m_fMaxPointSize = f; }
		F_FORCEINLINE float	GetMaxSupportedAnisotropy() const { return m_fMaxSupportedAnisotropy; }
		F_FORCEINLINE void SetMaxSupportedAnisotropy(float f) { m_fMaxSupportedAnisotropy = f; }

		F_FORCEINLINE const String& GetMaxVertexProgramVersion() const	{ return m_strMaxVertexProgramVersion; }
		F_FORCEINLINE void SetMaxVertexProgramVersion(const String& str) { m_strMaxVertexProgramVersion = str; }
		F_FORCEINLINE const String& GetMaxFragmentProgramVersion() const { return m_strMaxFragmentProgramVersion; }
		F_FORCEINLINE void SetMaxFragmentProgramVersion(const String& str) { m_strMaxFragmentProgramVersion = str; }
		
		F_FORCEINLINE const StringSet& GetSupportedShaderProfiles() const
        {
            return m_setShaderProfiles;
        }

		F_FORCEINLINE uint16 GetVertexProgramConstantFloatCount() const { return m_nVertexProgramConstantFloatCount; }
		F_FORCEINLINE void SetVertexProgramConstantFloatCount(uint16 n) { m_nVertexProgramConstantFloatCount = n; }
		F_FORCEINLINE uint16 GetGeometryProgramConstantFloatCount() const { return m_nGeometryProgramConstantFloatCount; }
		F_FORCEINLINE void SetGeometryProgramConstantFloatCount(uint16 n) { m_nGeometryProgramConstantFloatCount = n; }
		F_FORCEINLINE uint16 GetTessellationHullProgramConstantFloatCount() const { return m_nTessellationHullProgramConstantFloatCount; }
		F_FORCEINLINE void SetTessellationHullProgramConstantFloatCount(uint16 n) { m_nTessellationHullProgramConstantFloatCount = n; }
		F_FORCEINLINE uint16 GetTessellationDomainProgramConstantFloatCount() const { return m_nTessellationDomainProgramConstantFloatCount; }
		F_FORCEINLINE void SetTessellationDomainProgramConstantFloatCount(uint16 n) { m_nTessellationDomainProgramConstantFloatCount = n; }
		F_FORCEINLINE uint16 GetComputeProgramConstantFloatCount() const { return m_nComputeProgramConstantFloatCount; }
		F_FORCEINLINE void SetComputeProgramConstantFloatCount(uint16 n) { m_nComputeProgramConstantFloatCount = n; }
		F_FORCEINLINE uint16 GetFragmentProgramConstantFloatCount() const { return m_nFragmentProgramConstantFloatCount; }
		F_FORCEINLINE void SetFragmentProgramConstantFloatCount(uint16 n) { m_nFragmentProgramConstantFloatCount = n; }
	
		F_FORCEINLINE bool GetNonPOW2TexturesLimited() const { return m_bNonPOW2TexturesLimited; }
		F_FORCEINLINE void SetNonPOW2TexturesLimited(bool b) { m_bNonPOW2TexturesLimited = b; }
		F_FORCEINLINE bool GetVertexTextureUnitsShared() const { return m_bVertexTextureUnitsShared; }
		F_FORCEINLINE void SetVertexTextureUnitsShared(bool b) { m_bVertexTextureUnitsShared = b; }
		F_FORCEINLINE bool GetRenderTargetFormatNull() const { return m_bRenderTargetFormatNull; }
		F_FORCEINLINE void SetRenderTargetFormatNull(bool b) { m_bRenderTargetFormatNull = b; }
		
		F_FORCEINLINE bool GetDepthStencilTextureD16() const { return m_bDSTFormatD16; }
		F_FORCEINLINE void SetDepthStencilTextureD16(bool b) { m_bDSTFormatD16 = b; }
		F_FORCEINLINE bool GetDepthStencilTextureD24S8() const { return m_bDSTFormatD24S8; }
		F_FORCEINLINE void SetDepthStencilTextureD24S8(bool b) { m_bDSTFormatD24S8 = b; }
		F_FORCEINLINE bool GetDepthStencilTextureD24X8() const { return m_bDSTFormatD24X8; }
		F_FORCEINLINE void SetDepthStencilTextureD24X8(bool b) { m_bDSTFormatD24X8 = b; }

	public:
		void AddShaderProfile(const String& profile);
        void RemoveShaderProfile(const String& profile);
        bool IsShaderProfileSupported(const String& profile) const;

	public:
		void LogInfo();
	};

}; //LostPeterEngine

#endif
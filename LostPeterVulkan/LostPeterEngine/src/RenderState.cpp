/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderState.h"
#include "../include/TextureManager.h"
#include "../include/Texture.h"
#include "../include/ShaderManager.h"
#include "../include/Shader.h"

namespace LostPeterEngine
{
    //////////////////////////////////// RenderStateCommon //////////////////////////////
    RenderStateCommon::RenderStateCommon()
		: m_ePolygon(F_Polygon_Solid)
		, m_eCulling(F_Culling_ClockWise)

		, m_fPointSize(64.0f)
		, m_bPointSpriteEnabled(false)
		, m_bPointAttenuEnabled(false)
		, m_fPointAttenuConstant(1.0f)
		, m_fPointAttenuLinear(0.0f)
		, m_fPointAttenuQuadratic(0.0f)
		, m_fPointMinSize(1.0f)
		, m_fPointMaxSize(64.0f)

		, m_bDepthTestEnabled(true)
		, m_bDepthWriteEnabled(true)
		, m_eDepthCompareFunc(F_CompareFunc_LessEqual)
		, m_fDepthBiasConstant(0.0f)
		, m_fDepthBiasSlopeScale(0.0f)

		, m_bStencilEnabled(false)
		, m_eStencilCompareFunc(F_CompareFunc_AlwaysPass)
		, m_nStencilRefValue(0)
		, m_nStencilMask(0xFFFFFFFF)
		, m_eStencilFailOP(F_StencilOP_Keep)
		, m_eStencilDepthFailOP(F_StencilOP_Keep)
		, m_eStencilPassOP(F_StencilOP_Keep)
		, m_bStencilTwoSidedEnabled(false)

		, m_bScissorTestEnabled(false)
		, m_nScissorTest_Left(0)
		, m_nScissorTest_Top(0)
		, m_nScissorTest_Right(800)
		, m_nScissorTest_Bottom(600)

		, m_bAlphaTestEnabled(false)
		, m_eAlphaRejectCompareFunc(F_CompareFunc_AlwaysPass)
		, m_nAlphaRejectValue(0)

		, m_bSceneBlendingEnabled(false)
		, m_eSceneBlending(F_SceneBlending_Alpha)
		, m_eSceneBlendingOP(F_SceneBlendingOP_Add)
		, m_eSceneBlendingFactorSrc(F_SceneBlendingFactor_One)
		, m_eSceneBlendingFactorDst(F_SceneBlendingFactor_Zero)
		, m_bSceneBlendingSeperateEnabled(false)
		, m_eSceneBlendingOP2(F_SceneBlendingOP_Add)
		, m_eSceneBlendingFactorSrc2(F_SceneBlendingFactor_One)
		, m_eSceneBlendingFactorDst2(F_SceneBlendingFactor_Zero)

		, m_bColorRWriteEnabled(true)
		, m_bColorGWriteEnabled(true)
		, m_bColorBWriteEnabled(true)
		, m_bColorAWriteEnabled(true)
	{

	}

	RenderStateCommon::~RenderStateCommon()
	{

	}

	void RenderStateCommon::CopyFrom(const RenderStateCommon* pStateCommon)
	{
		memcpy(this, pStateCommon, sizeof(RenderStateCommon));
	}

	void RenderStateCommon::CopyTo(RenderStateCommon* pStateCommon)
	{
		memcpy(pStateCommon, this,sizeof(RenderStateCommon));
	}
	

	//////////////////////////////////// RenderStateLighting ////////////////////////////
	RenderStateLighting::RenderStateLighting()
		: m_bLightingEnabled(true)
		, m_eLighting(F_Lighting_Gouraud) 
		, m_nMaxEffectLightNum(C_CONFIG_MAX_LIGHT_COUNT)

	////Flat/Gouraud/Phong
		, m_clAmbient(FMath::ms_clWhite)
		, m_clDiffuse(FMath::ms_clWhite)
		, m_clSpecular(FMath::ms_clBlack)
		, m_clEmissive(FMath::ms_clBlack)
		, m_fShininess(0.0f)
		, m_nColorFromVertexFlag(0)

	////Pbr

	{

	}
	RenderStateLighting::~RenderStateLighting()
	{

	}

	void RenderStateLighting::CopyFrom(const RenderStateLighting* pStateLighting)
	{
		memcpy(this, pStateLighting, sizeof(RenderStateLighting));
	}

	void RenderStateLighting::CopyTo(RenderStateLighting* pStateLighting)
	{
		memcpy(pStateLighting, this,sizeof(RenderStateLighting));
	}
    
	//////////////////////////////////// RenderStateParam ///////////////////////////////
	RenderStateParam::RenderStateParam(const String& _nameParam)
		: Base(_nameParam)
	{

	}
	RenderStateParam::~RenderStateParam()
	{	
		Destroy();
	}
	void RenderStateParam::Destroy()
	{

	}


	//////////////////////////////////// RenderStateTexture /////////////////////////////
	RenderStateTexture::RenderStateTexture(uint32 _group, const String& _nameTexture)
		: Base(_group, _nameTexture)
		, m_pTexture(nullptr)
	{

	}
	RenderStateTexture::~RenderStateTexture()
	{
		Destroy();
	}
	void RenderStateTexture::Destroy()
	{
		UnloadTexture();
	}
	bool RenderStateTexture::LoadTexture()
	{
		m_pTexture = TextureManager::GetSingleton().LoadTexture(GetGroup(), GetName());
		if (m_pTexture == nullptr)
		{
			F_LogError("*********************** RenderStateTexture::LoadTexture: Load texture, group: [%u], name: [%s] failed !", GetGroup(), GetName().c_str());
			return false;
		}

		return true;
	}
	void RenderStateTexture::UnloadTexture()
	{
		if (m_pTexture != nullptr)
		{
			TextureManager::GetSingleton().Delete(m_pTexture);
		}
		m_pTexture = nullptr;
	}


    //////////////////////////////////// RenderStateShader //////////////////////////////
	RenderStateShaderItem::RenderStateShaderItem(const String& nameShader, FShaderType type)
		: Base(nameShader)
		, m_eShader(type)
		, m_pShader(nullptr)
	{

	}
	RenderStateShaderItem::~RenderStateShaderItem()
	{
		Destroy();
	}
	void RenderStateShaderItem::Destroy()
	{
		m_eShader = F_Shader_Vertex;
		DeleteStateParamAll();
		DeleteStateTextureAll();
		UnloadShader();
	}

	bool RenderStateShaderItem::LoadShader()
	{
		uint32 nGroup = FPathManager::PathGroup_Shader;
		m_pShader = ShaderManager::GetSingleton().LoadShader(nGroup, GetName());
		if (m_pShader == nullptr)
		{
			F_LogError("*********************** RenderStateShaderItem::LoadShader: Load shader, group: [%u], name: [%s] failed !", nGroup, GetName().c_str());
			return false;
		}

		return true;
	}
	void RenderStateShaderItem::UnloadShader()
	{
		if (m_pShader != nullptr)
		{
			ShaderManager::GetSingleton().Delete(m_pShader);
		}
		m_pShader = nullptr;
	}

////Param
	int RenderStateShaderItem::GetStateParamCount() const
	{
		return (int)m_aRenderStateParam.size();
	}
	RenderStateParam* RenderStateShaderItem::GetStateParam(int index) const
	{
		if (index < 0 || index >= (int)m_aRenderStateParam.size())
			return nullptr;
		return m_aRenderStateParam[index];
	}
	RenderStateParam* RenderStateShaderItem::GetStateParamByName(const String& name)
	{
		RenderStateParamPtrMap::iterator itFind = m_mapRenderStateParam.find(name);
		if (itFind == m_mapRenderStateParam.end())
			return nullptr;
		return itFind->second;
	}
	void RenderStateShaderItem::AddStateParam(RenderStateParam* pStateParam)
	{
		const String& nameSP = pStateParam->GetName();
		RenderStateParam* pSP = GetStateParamByName(nameSP);
		if (pSP != nullptr)
			return;

		m_aRenderStateParam.push_back(pStateParam);
		m_mapRenderStateParam[nameSP] = pStateParam;
	}
	void RenderStateShaderItem::DeleteStateParam(RenderStateParam* pStateParam)
	{
		if (pStateParam == nullptr)
			return;

		const String& nameSP = pStateParam->GetName();
		RenderStateParamPtrMap::iterator itFind = m_mapRenderStateParam.find(nameSP);
		if (itFind != m_mapRenderStateParam.end())
		{
			m_mapRenderStateParam.erase(itFind);
		}
		RenderStateParamPtrVector::iterator itFindA = std::find(m_aRenderStateParam.begin(), m_aRenderStateParam.end(), pStateParam);
		if (itFindA != m_aRenderStateParam.end())
			m_aRenderStateParam.erase(itFindA);
		F_DELETE(pStateParam)
	}
	void RenderStateShaderItem::DeleteStateParam(int index)
	{
		RenderStateParam* pStateParam = GetStateParam(index);
		DeleteStateParam(pStateParam);
	}
	void RenderStateShaderItem::DeleteStateParamAll()
	{
		int count = (int)m_aRenderStateParam.size();
		for (int i = 0; i < count; i++)
		{
			F_DELETE(m_aRenderStateParam[i])
		}
		m_aRenderStateParam.clear();
		m_mapRenderStateParam.clear();
	}


////Texture
	int RenderStateShaderItem::GetStateTextureCount() const
	{
		return (int)m_aRenderStateTexture.size();
	}
	RenderStateTexture* RenderStateShaderItem::GetStateTexture(int index) const
	{
		if (index < 0 || index >= (int)m_aRenderStateTexture.size())
			return nullptr;
		return m_aRenderStateTexture[index];
	}
	RenderStateTexture* RenderStateShaderItem::GetStateTextureByName(const String& name)
	{
		RenderStateTexturePtrMap::iterator itFind = m_mapRenderStateTexture.find(name);
		if (itFind == m_mapRenderStateTexture.end())
			return nullptr;
		return itFind->second;
	}
	void RenderStateShaderItem::AddStateTexture(RenderStateTexture* pStateTexture)
	{
		const String& nameST = pStateTexture->GetName();
		RenderStateTexture* pST = GetStateTextureByName(nameST);
		if (pST != nullptr)
			return;

		m_aRenderStateTexture.push_back(pStateTexture);
		m_mapRenderStateTexture[nameST] = pStateTexture;
	}
	void RenderStateShaderItem::DeleteStateTexture(RenderStateTexture* pStateTexture)
	{
		if (pStateTexture == nullptr)
			return;

		const String& nameST = pStateTexture->GetName();
		RenderStateTexturePtrMap::iterator itFind = m_mapRenderStateTexture.find(nameST);
		if (itFind != m_mapRenderStateTexture.end())
		{
			m_mapRenderStateTexture.erase(itFind);
		}
		RenderStateTexturePtrVector::iterator itFindA = std::find(m_aRenderStateTexture.begin(), m_aRenderStateTexture.end(), pStateTexture);
		if (itFindA != m_aRenderStateTexture.end())
			m_aRenderStateTexture.erase(itFindA);
		F_DELETE(pStateTexture)
	}
	void RenderStateShaderItem::DeleteStateTexture(int index)
	{
		RenderStateTexture* pStateTexture = GetStateTexture(index);
		DeleteStateTexture(pStateTexture);
	}
	void RenderStateShaderItem::DeleteStateTextureAll()
	{
		int count = (int)m_aRenderStateTexture.size();
		for (int i = 0; i < count; i++)
		{
			F_DELETE(m_aRenderStateTexture[i])
		}
		m_aRenderStateTexture.clear();
		m_mapRenderStateTexture.clear();
	}



    RenderStateShader::RenderStateShader()
		: m_strNameDescriptorSetLayout("")
    {

    }
	RenderStateShader::~RenderStateShader()
    {
		Destroy();
    }
	void RenderStateShader::Destroy()
	{
		DeleteRenderStateShaderItemAll();
	}

	bool RenderStateShader::HasRenderStateShaderItem(const String& nameShader)
	{
		return GetRenderStateShaderItem(nameShader) != nullptr;
	}
	RenderStateShaderItem* RenderStateShader::GetRenderStateShaderItem(const String& nameShader)
	{
		RenderStateShaderItemPtrMap::iterator itFind = m_mapRenderStateShaderItem.find(nameShader);
		if (itFind != m_mapRenderStateShaderItem.end())
			return itFind->second;
		return nullptr;
	}
	void RenderStateShader::AddRenderStateShaderItem(RenderStateShaderItem* pItem)
	{
		const String& nameShader = pItem->GetName();
		RenderStateShaderItemPtrMap::iterator itFind = m_mapRenderStateShaderItem.find(nameShader);
		if (itFind != m_mapRenderStateShaderItem.end())
			return;

		m_aRenderStateShaderItem.push_back(pItem);
		m_mapRenderStateShaderItem[nameShader] = pItem;
	}
	void RenderStateShader::DeleteRenderStateShaderItem(const String& nameShader)
	{
		RenderStateShaderItemPtrMap::iterator itFind = m_mapRenderStateShaderItem.find(nameShader);
		if (itFind == m_mapRenderStateShaderItem.end())
			return;
		
		RenderStateShaderItemPtrVector::iterator itFindA = std::find(m_aRenderStateShaderItem.begin(), m_aRenderStateShaderItem.end(), itFind->second);
		if (itFindA != m_aRenderStateShaderItem.end())
			m_aRenderStateShaderItem.erase(itFindA);
		F_DELETE(itFind->second)
		m_mapRenderStateShaderItem.erase(itFind);
	}
	void RenderStateShader::DeleteRenderStateShaderItem(RenderStateShaderItem* pItem)
	{
		DeleteRenderStateShaderItem(pItem->GetName());
	}
	void RenderStateShader::DeleteRenderStateShaderItemAll()
	{
		for (RenderStateShaderItemPtrVector::iterator it = m_aRenderStateShaderItem.begin();
			 it != m_aRenderStateShaderItem.end(); ++it)
		{
			F_DELETE(*it)
		}
		m_aRenderStateShaderItem.clear();
		m_mapRenderStateShaderItem.clear();
	}


    //////////////////////////////////// RenderState ////////////////////////////////////
    RenderState::RenderState(const String& namePass,
							 FRenderPassType typeRenderPass)
        : Base(namePass)
		, m_typeRenderPass(typeRenderPass)
		, m_pStateCommon(new RenderStateCommon)
		, m_pStateLighting(new RenderStateLighting)
		, m_pStateShader(new RenderStateShader)
    {

    }
    RenderState::~RenderState()
    {
        Destroy();
    }

    void RenderState::Destroy()
    {
        F_DELETE(m_pStateCommon)
		F_DELETE(m_pStateLighting)
		F_DELETE(m_pStateShader)
    }

    void RenderState::SetStateCommonFrom(const RenderStateCommon* pStateCommon)
    {
        F_Assert(pStateCommon && "RenderState::SetStateCommonFrom")
		m_pStateCommon->CopyFrom(pStateCommon);
    }
    void RenderState::SetStateCommonTo(RenderStateCommon* pStateCommon)
    {
        F_Assert(pStateCommon && "RenderState::SetStateCommonTo")
		m_pStateCommon->CopyTo(pStateCommon);
    }

	void RenderState::SetStateLightingFrom(const RenderStateLighting* pStateLighting)
	{
		F_Assert(pStateLighting && "RenderState::SetStateLightingFrom")
		m_pStateLighting->CopyFrom(pStateLighting);
	}
	void RenderState::SetStateLightingTo(RenderStateLighting* pStateLighting)
	{
		F_Assert(pStateLighting && "RenderState::SetStateLightingTo")
		m_pStateLighting->CopyTo(pStateLighting);
	}

    void RenderState::SetStateShaderFrom(const RenderStateShader* pStateShader)
    {
        F_Assert(pStateShader && "RenderState::SetStateShaderFrom")

    }
    void RenderState::SetStateShaderTo(RenderStateShader* pStateShader)
    {
        F_Assert(pStateShader && "RenderState::SetStateShaderTo")

    }

}; //LostPeterEngine
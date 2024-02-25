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
#include "../include/ShaderProgramManager.h"
#include "../include/ShaderProgram.h"

namespace LostPeterEngine
{
    //////////////////////////////////// RenderStateCommon //////////////////////////////
    RenderStateCommon::RenderStateCommon()
		: m_ePolygon(F_Polygon_Solid)
		, m_eFrontFace(F_FrontFace_ClockWise)
		, m_eCulling(F_Culling_CounterClockWise)

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
		, m_bStencilTwoSidedEnabled(false)
		, m_eStencilCompareFunc_Front(F_CompareFunc_AlwaysPass)
		, m_nStencilRefValue_Front(0)
		, m_nStencilCompareMask_Front(0xFFFFFFFF)
		, m_nStencilWriteMask_Front(0xFFFFFFFF)
		, m_eStencilFailOP_Front(F_StencilOP_Keep)
		, m_eStencilDepthFailOP_Front(F_StencilOP_Keep)
		, m_eStencilPassOP_Front(F_StencilOP_Keep)
		, m_eStencilCompareFunc_Back(F_CompareFunc_AlwaysPass)
		, m_nStencilRefValue_Back(0)
		, m_nStencilCompareMask_Back(0xFFFFFFFF)
		, m_nStencilWriteMask_Back(0xFFFFFFFF)
		, m_eStencilFailOP_Back(F_StencilOP_Keep)
		, m_eStencilDepthFailOP_Back(F_StencilOP_Keep)
		, m_eStencilPassOP_Back(F_StencilOP_Keep)

		, m_bScissorTestEnabled(false)
		, m_nScissorTest_Left(0)
		, m_nScissorTest_Top(0)
		, m_nScissorTest_Right(800)
		, m_nScissorTest_Bottom(600)

		, m_bAlphaTestEnabled(false)
		, m_eAlphaRejectCompareFunc(F_CompareFunc_AlwaysPass)
		, m_nAlphaRejectValue(0)

		, m_bSceneBlendingEnabled(false)
		, m_eSceneBlendingOP_Color(F_SceneBlendingOP_Add)
		, m_eSceneBlendingFactorSrc_Color(F_SceneBlendingFactor_One)
		, m_eSceneBlendingFactorDst_Color(F_SceneBlendingFactor_Zero)
		, m_eSceneBlendingOP_Alpha(F_SceneBlendingOP_Add)
		, m_eSceneBlendingFactorSrc_Alpha(F_SceneBlendingFactor_One)
		, m_eSceneBlendingFactorDst_Alpha(F_SceneBlendingFactor_Zero)

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
	RenderStateTexture::RenderStateTexture(uint32 nGroup, const String& strNameTexture)
		: Base(nGroup, strNameTexture)
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
	RenderStateShaderProgramItem::RenderStateShaderProgramItem(const String& nameShaderProgram, FShaderType type)
		: Base(nameShaderProgram)
		, m_eShader(type)
		, m_pShaderProgram(nullptr)
	{

	}
	RenderStateShaderProgramItem::~RenderStateShaderProgramItem()
	{
		Destroy();
	}
	void RenderStateShaderProgramItem::Destroy()
	{
		m_eShader = F_Shader_Vertex;
		DeleteStateParamAll();
		DeleteStateTextureAll();
		UnloadShader();
	}

	bool RenderStateShaderProgramItem::LoadShaderProgram()
	{
		uint32 nGroup = FPathManager::PathGroup_Shader;
		m_pShaderProgram = ShaderProgramManager::GetSingleton().LoadShaderProgram(nGroup, GetName());
		if (m_pShaderProgram == nullptr)
		{
			F_LogError("*********************** RenderStateShaderProgramItem::LoadShaderProgram: Load shader, group: [%u], name: [%s] failed !", nGroup, GetName().c_str());
			return false;
		}

		return true;
	}
	void RenderStateShaderProgramItem::UnloadShader()
	{
		if (m_pShaderProgram != nullptr)
		{
			ShaderProgramManager::GetSingleton().Delete(m_pShaderProgram);
		}
		m_pShaderProgram = nullptr;
	}

////Param
	int RenderStateShaderProgramItem::GetStateParamCount() const
	{
		return (int)m_aRenderStateParam.size();
	}
	RenderStateParam* RenderStateShaderProgramItem::GetStateParam(int index) const
	{
		if (index < 0 || index >= (int)m_aRenderStateParam.size())
			return nullptr;
		return m_aRenderStateParam[index];
	}
	RenderStateParam* RenderStateShaderProgramItem::GetStateParamByName(const String& name)
	{
		RenderStateParamPtrMap::iterator itFind = m_mapRenderStateParam.find(name);
		if (itFind == m_mapRenderStateParam.end())
			return nullptr;
		return itFind->second;
	}
	void RenderStateShaderProgramItem::AddStateParam(RenderStateParam* pStateParam)
	{
		const String& nameSP = pStateParam->GetName();
		RenderStateParam* pSP = GetStateParamByName(nameSP);
		if (pSP != nullptr)
			return;

		m_aRenderStateParam.push_back(pStateParam);
		m_mapRenderStateParam[nameSP] = pStateParam;
	}
	void RenderStateShaderProgramItem::DeleteStateParam(RenderStateParam* pStateParam)
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
	void RenderStateShaderProgramItem::DeleteStateParam(int index)
	{
		RenderStateParam* pStateParam = GetStateParam(index);
		DeleteStateParam(pStateParam);
	}
	void RenderStateShaderProgramItem::DeleteStateParamAll()
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
	int RenderStateShaderProgramItem::GetStateTextureCount() const
	{
		return (int)m_aRenderStateTexture.size();
	}
	RenderStateTexture* RenderStateShaderProgramItem::GetStateTexture(int index) const
	{
		if (index < 0 || index >= (int)m_aRenderStateTexture.size())
			return nullptr;
		return m_aRenderStateTexture[index];
	}
	RenderStateTexture* RenderStateShaderProgramItem::GetStateTextureByName(const String& name)
	{
		RenderStateTexturePtrMap::iterator itFind = m_mapRenderStateTexture.find(name);
		if (itFind == m_mapRenderStateTexture.end())
			return nullptr;
		return itFind->second;
	}
	void RenderStateShaderProgramItem::AddStateTexture(RenderStateTexture* pStateTexture)
	{
		const String& nameST = pStateTexture->GetName();
		RenderStateTexture* pST = GetStateTextureByName(nameST);
		if (pST != nullptr)
			return;

		m_aRenderStateTexture.push_back(pStateTexture);
		m_mapRenderStateTexture[nameST] = pStateTexture;
	}
	void RenderStateShaderProgramItem::DeleteStateTexture(RenderStateTexture* pStateTexture)
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
	void RenderStateShaderProgramItem::DeleteStateTexture(int index)
	{
		RenderStateTexture* pStateTexture = GetStateTexture(index);
		DeleteStateTexture(pStateTexture);
	}
	void RenderStateShaderProgramItem::DeleteStateTextureAll()
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
		DeleteRenderStateShaderProgramItemAll();
	}

	bool RenderStateShader::HasRenderStateShaderProgramItem(const String& nameShaderProgram)
	{
		return GetRenderStateShaderProgramItem(nameShaderProgram) != nullptr;
	}
	RenderStateShaderProgramItem* RenderStateShader::GetRenderStateShaderProgramItem(const String& nameShaderProgram)
	{
		RenderStateShaderProgramItemPtrMap::iterator itFind = m_mapRenderStateShaderProgramItem.find(nameShaderProgram);
		if (itFind != m_mapRenderStateShaderProgramItem.end())
			return itFind->second;
		return nullptr;
	}
	void RenderStateShader::AddRenderStateShaderProgramItem(RenderStateShaderProgramItem* pItem)
	{
		const String& nameShaderProgram = pItem->GetName();
		RenderStateShaderProgramItemPtrMap::iterator itFind = m_mapRenderStateShaderProgramItem.find(nameShaderProgram);
		if (itFind != m_mapRenderStateShaderProgramItem.end())
			return;

		m_aRenderStateShaderProgramItem.push_back(pItem);
		m_mapRenderStateShaderProgramItem[nameShaderProgram] = pItem;
	}
	void RenderStateShader::DeleteRenderStateShaderProgramItem(const String& nameShaderProgram)
	{
		RenderStateShaderProgramItemPtrMap::iterator itFind = m_mapRenderStateShaderProgramItem.find(nameShaderProgram);
		if (itFind == m_mapRenderStateShaderProgramItem.end())
			return;
		
		RenderStateShaderProgramItemPtrVector::iterator itFindA = std::find(m_aRenderStateShaderProgramItem.begin(), m_aRenderStateShaderProgramItem.end(), itFind->second);
		if (itFindA != m_aRenderStateShaderProgramItem.end())
			m_aRenderStateShaderProgramItem.erase(itFindA);
		F_DELETE(itFind->second)
		m_mapRenderStateShaderProgramItem.erase(itFind);
	}
	void RenderStateShader::DeleteRenderStateShaderProgramItem(RenderStateShaderProgramItem* pItem)
	{
		DeleteRenderStateShaderProgramItem(pItem->GetName());
	}
	void RenderStateShader::DeleteRenderStateShaderProgramItemAll()
	{
		for (RenderStateShaderProgramItemPtrVector::iterator it = m_aRenderStateShaderProgramItem.begin();
			 it != m_aRenderStateShaderProgramItem.end(); ++it)
		{
			F_DELETE(*it)
		}
		m_aRenderStateShaderProgramItem.clear();
		m_mapRenderStateShaderProgramItem.clear();
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
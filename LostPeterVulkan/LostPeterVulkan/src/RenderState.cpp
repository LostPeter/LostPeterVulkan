/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderState.h"
#include "../include/VulkanWindow.h"
#include "../include/TextureManager.h"
#include "../include/Texture.h"
#include "../include/ShaderManager.h"
#include "../include/Shader.h"
#include "../include/VKDescriptorSetLayoutManager.h"
#include "../include/VKDescriptorSetLayout.h"

namespace LostPeter
{
    //////////////////////////////////// RenderStateCommon //////////////////////////////
    RenderStateCommon::RenderStateCommon()
		: typePolygon(F_Polygon_Solid)
		, typeCulling(F_Culling_ClockWise)
		, fPointSize(64.0f)
		, bPointSpriteEnabled(false)
		, bPointAttenuEnabled(false)
		, fPointAttenuConstant(1.0f)
		, fPointAttenuLinear(0.0f)
		, fPointAttenuQuadratic(0.0f)
		, fPointMinSize(1.0f)
		, fPointMaxSize(64.0f)
		, bDepthTestEnabled(true)
		, bDepthWriteEnabled(true)
		, typeDepthFunc(F_CompareFunc_LessEqual)
		, fDepthBiasConstant(0.0f)
		, fDepthBiasSlopeScale(0.0f)
		, bStencilEnabled(false)
		, typeStencilFunc(F_CompareFunc_AlwaysPass)
		, nStencilRefValue(0)
		, nStencilMask(0xFFFFFFFF)
		, typeStencilFailOP(F_StencilOP_Keep)
		, typeStencilDepthFailOP(F_StencilOP_Keep)
		, typeStencilPassOP(F_StencilOP_Keep)
		, bStencilTwoSidedEnabled(false)
		, bScissorTestEnabled(false)
		, nScissorTest_Left(0)
		, nScissorTest_Top(0)
		, nScissorTest_Right(800)
		, nScissorTest_Bottom(600)
		, bAlphaTestEnabled(false)
		, typeAlphaRejectFunc(F_CompareFunc_AlwaysPass)
		, nAlphaRejectValue(0)
		, bSceneBlendingEnabled(false)
		, typeSceneBlending(F_SceneBlending_Alpha)
		, typeSceneBlendingOP(F_SceneBlendingOP_Add)
		, typeSceneBlendingFactorSrc(F_SceneBlendingFactor_One)
		, typeSceneBlendingFactorDst(F_SceneBlendingFactor_Zero)
		, bSceneBlendingSeperateEnabled(false)
		, typeSceneBlendingOP2(F_SceneBlendingOP_Add)
		, typeSceneBlendingFactorSrc2(F_SceneBlendingFactor_One)
		, typeSceneBlendingFactorDst2(F_SceneBlendingFactor_Zero)
		, bColorRWriteEnabled(true)
		, bColorGWriteEnabled(true)
		, bColorBWriteEnabled(true)
		, bColorAWriteEnabled(true)
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
		: bLightingEnabled(true)
		, typeLighting(F_Lighting_Gouraud) 
		, nMaxEffectLightNum(MAX_LIGHT_COUNT)

	////Flat/Gouraud/Phong
		, clAmbient(FMath::ms_clWhite)
		, clDiffuse(FMath::ms_clWhite)
		, clSpecular(FMath::ms_clBlack)
		, clEmissive(FMath::ms_clBlack)
		, fShininess(0.0f)
		, nColorFromVertexFlag(0)

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
		, pTexture(nullptr)
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
		pTexture = TextureManager::GetSingleton().LoadTexture(GetGroup(), GetName());
		if (pTexture == nullptr)
		{
			F_LogError("*********************** RenderStateTexture::LoadTexture: Load texture, group: [%u], name: [%s] failed !", GetGroup(), GetName().c_str());
			return false;
		}

		return true;
	}
	void RenderStateTexture::UnloadTexture()
	{
		if (pTexture != nullptr)
		{
			TextureManager::GetSingleton().UnloadTexture(pTexture);
		}
		pTexture = nullptr;
	}


    //////////////////////////////////// RenderStateShader //////////////////////////////
	RenderStateShaderItem::RenderStateShaderItem(const String& nameShader, FShaderType type)
		: Base(nameShader)
		, typeShader(type)
		, pShader(nullptr)
	{

	}
	RenderStateShaderItem::~RenderStateShaderItem()
	{
		Destroy();
	}
	void RenderStateShaderItem::Destroy()
	{
		typeShader = F_Shader_Vertex;
		DeleteStateParamAll();
		DeleteStateTextureAll();
		UnloadShader();
	}

	bool RenderStateShaderItem::LoadShader()
	{
		uint32 nGroup = FPathManager::PathGroup_Shader;
		pShader = ShaderManager::GetSingleton().LoadShader(nGroup, GetName());
		if (pShader == nullptr)
		{
			F_LogError("*********************** RenderStateShaderItem::LoadShader: Load shader, group: [%u], name: [%s] failed !", nGroup, GetName().c_str());
			return false;
		}

		return true;
	}
	void RenderStateShaderItem::UnloadShader()
	{
		if (pShader != nullptr)
		{
			ShaderManager::GetSingleton().UnloadShader(pShader);
		}
		pShader = nullptr;
	}

////Param
	int RenderStateShaderItem::GetStateParamCount() const
	{
		return (int)aRenderStateParam.size();
	}
	RenderStateParam* RenderStateShaderItem::GetStateParam(int index) const
	{
		if (index < 0 || index >= (int)aRenderStateParam.size())
			return nullptr;
		return aRenderStateParam[index];
	}
	RenderStateParam* RenderStateShaderItem::GetStateParamByName(const String& name)
	{
		RenderStateParamPtrMap::iterator itFind = mapRenderStateParam.find(name);
		if (itFind == mapRenderStateParam.end())
			return nullptr;
		return itFind->second;
	}
	void RenderStateShaderItem::AddStateParam(RenderStateParam* pStateParam)
	{
		const String& nameSP = pStateParam->GetName();
		RenderStateParam* pSP = GetStateParamByName(nameSP);
		if (pSP != nullptr)
			return;

		aRenderStateParam.push_back(pStateParam);
		mapRenderStateParam[nameSP] = pStateParam;
	}
	void RenderStateShaderItem::DeleteStateParam(RenderStateParam* pStateParam)
	{
		if (pStateParam == nullptr)
			return;

		const String& nameSP = pStateParam->GetName();
		RenderStateParamPtrMap::iterator itFind = mapRenderStateParam.find(nameSP);
		if (itFind != mapRenderStateParam.end())
		{
			mapRenderStateParam.erase(itFind);
		}
		RenderStateParamPtrVector::iterator itFindA = std::find(aRenderStateParam.begin(), aRenderStateParam.end(), pStateParam);
		if (itFindA != aRenderStateParam.end())
			aRenderStateParam.erase(itFindA);
		F_DELETE(pStateParam)
	}
	void RenderStateShaderItem::DeleteStateParam(int index)
	{
		RenderStateParam* pStateParam = GetStateParam(index);
		DeleteStateParam(pStateParam);
	}
	void RenderStateShaderItem::DeleteStateParamAll()
	{
		int count = (int)aRenderStateParam.size();
		for (int i = 0; i < count; i++)
		{
			F_DELETE(aRenderStateParam[i])
		}
		aRenderStateParam.clear();
		mapRenderStateParam.clear();
	}


////Texture
	int RenderStateShaderItem::GetStateTextureCount() const
	{
		return (int)aRenderStateTexture.size();
	}
	RenderStateTexture* RenderStateShaderItem::GetStateTexture(int index) const
	{
		if (index < 0 || index >= (int)aRenderStateTexture.size())
			return nullptr;
		return aRenderStateTexture[index];
	}
	RenderStateTexture* RenderStateShaderItem::GetStateTextureByName(const String& name)
	{
		RenderStateTexturePtrMap::iterator itFind = mapRenderStateTexture.find(name);
		if (itFind == mapRenderStateTexture.end())
			return nullptr;
		return itFind->second;
	}
	void RenderStateShaderItem::AddStateTexture(RenderStateTexture* pStateTexture)
	{
		const String& nameST = pStateTexture->GetName();
		RenderStateTexture* pST = GetStateTextureByName(nameST);
		if (pST != nullptr)
			return;

		aRenderStateTexture.push_back(pStateTexture);
		mapRenderStateTexture[nameST] = pStateTexture;
	}
	void RenderStateShaderItem::DeleteStateTexture(RenderStateTexture* pStateTexture)
	{
		if (pStateTexture == nullptr)
			return;

		const String& nameST = pStateTexture->GetName();
		RenderStateTexturePtrMap::iterator itFind = mapRenderStateTexture.find(nameST);
		if (itFind != mapRenderStateTexture.end())
		{
			mapRenderStateTexture.erase(itFind);
		}
		RenderStateTexturePtrVector::iterator itFindA = std::find(aRenderStateTexture.begin(), aRenderStateTexture.end(), pStateTexture);
		if (itFindA != aRenderStateTexture.end())
			aRenderStateTexture.erase(itFindA);
		F_DELETE(pStateTexture)
	}
	void RenderStateShaderItem::DeleteStateTexture(int index)
	{
		RenderStateTexture* pStateTexture = GetStateTexture(index);
		DeleteStateTexture(pStateTexture);
	}
	void RenderStateShaderItem::DeleteStateTextureAll()
	{
		int count = (int)aRenderStateTexture.size();
		for (int i = 0; i < count; i++)
		{
			F_DELETE(aRenderStateTexture[i])
		}
		aRenderStateTexture.clear();
		mapRenderStateTexture.clear();
	}



    RenderStateShader::RenderStateShader()
		: nameDescriptorSetLayout("")
		, pVKDescriptorSetLayout(nullptr)
    {

    }
	RenderStateShader::~RenderStateShader()
    {
		Destroy();
    }
	void RenderStateShader::Destroy()
	{
		DeleteRenderStateShaderItemAll();
		UnloadVKDescriptorSetLayout();
	}

	bool RenderStateShader::LoadVKDescriptorSetLayout()
	{
		pVKDescriptorSetLayout = VKDescriptorSetLayoutManager::GetSingleton().LoadVKDescriptorSetLayout(nameDescriptorSetLayout);
		if (pVKDescriptorSetLayout == nullptr)
		{
			F_LogError("*********************** RenderStateShader::LoadVKDescriptorSetLayout: Load VKDescriptorSetLayout, name: [%s] failed !", nameDescriptorSetLayout.c_str());
			return false;
		}

		return true;
	}
	void RenderStateShader::UnloadVKDescriptorSetLayout()
	{
		if (pVKDescriptorSetLayout != nullptr)
		{
			VKDescriptorSetLayoutManager::GetSingleton().UnloadVKDescriptorSetLayout(pVKDescriptorSetLayout);
		}
		pVKDescriptorSetLayout = nullptr;
	}

	bool RenderStateShader::HasRenderStateShaderItem(const String& nameShader)
	{
		return GetRenderStateShaderItem(nameShader) != nullptr;
	}
	RenderStateShaderItem* RenderStateShader::GetRenderStateShaderItem(const String& nameShader)
	{
		RenderStateShaderItemPtrMap::iterator itFind = mapRenderStateShaderItem.find(nameShader);
		if (itFind != mapRenderStateShaderItem.end())
			return itFind->second;
		return nullptr;
	}
	void RenderStateShader::AddRenderStateShaderItem(RenderStateShaderItem* pItem)
	{
		const String& nameShader = pItem->GetName();
		RenderStateShaderItemPtrMap::iterator itFind = mapRenderStateShaderItem.find(nameShader);
		if (itFind != mapRenderStateShaderItem.end())
			return;

		aRenderStateShaderItem.push_back(pItem);
		mapRenderStateShaderItem[nameShader] = pItem;
	}
	void RenderStateShader::DeleteRenderStateShaderItem(const String& nameShader)
	{
		RenderStateShaderItemPtrMap::iterator itFind = mapRenderStateShaderItem.find(nameShader);
		if (itFind == mapRenderStateShaderItem.end())
			return;
		
		RenderStateShaderItemPtrVector::iterator itFindA = std::find(aRenderStateShaderItem.begin(), aRenderStateShaderItem.end(), itFind->second);
		if (itFindA != aRenderStateShaderItem.end())
			aRenderStateShaderItem.erase(itFindA);
		F_DELETE(itFind->second)
		mapRenderStateShaderItem.erase(itFind);
	}
	void RenderStateShader::DeleteRenderStateShaderItem(RenderStateShaderItem* pItem)
	{
		DeleteRenderStateShaderItem(pItem->GetName());
	}
	void RenderStateShader::DeleteRenderStateShaderItemAll()
	{
		for (RenderStateShaderItemPtrVector::iterator it = aRenderStateShaderItem.begin();
			 it != aRenderStateShaderItem.end(); ++it)
		{
			F_DELETE(*it)
		}
		aRenderStateShaderItem.clear();
		mapRenderStateShaderItem.clear();
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

}; //LostPeter
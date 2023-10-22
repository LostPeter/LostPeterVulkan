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
    

	//////////////////////////////////// RenderStateTexture /////////////////////////////
	RenderStateTexture::RenderStateTexture()
	{

	}
	RenderStateTexture::~RenderStateTexture()
	{

	}


    //////////////////////////////////// RenderStateShader //////////////////////////////
    RenderStateShader::RenderStateShader()
    {

    }
	RenderStateShader::~RenderStateShader()
    {

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
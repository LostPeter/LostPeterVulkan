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

#ifndef _RENDER_STATE_H_
#define _RENDER_STATE_H_

#include "Base.h"

namespace LostPeterEngine
{
	//////////////////////////////////// RenderStateCommon //////////////////////////////
	class engineExport RenderStateCommon : public FNonCopyable
	{
	public:
		RenderStateCommon();
		~RenderStateCommon();

	public:
		FPolygonType m_ePolygon;
		FFrontFaceType m_eFrontFace;
		FCullingType m_eCulling;

		float m_fPointSize;
		bool m_bPointSpriteEnabled;
		bool m_bPointAttenuEnabled;
		float m_fPointAttenuConstant;
		float m_fPointAttenuLinear;
		float m_fPointAttenuQuadratic;
		float m_fPointMinSize;
		float m_fPointMaxSize;

		bool m_bDepthTestEnabled;
		bool m_bDepthWriteEnabled;
		FCompareFuncType m_eDepthCompareFunc;
		float m_fDepthBiasConstant;
		float m_fDepthBiasSlopeScale;

		bool m_bStencilEnabled;
		bool m_bStencilTwoSidedEnabled;
		FCompareFuncType m_eStencilCompareFunc_Front;
		uint32 m_nStencilRefValue_Front;
		uint32 m_nStencilCompareMask_Front;
		uint32 m_nStencilWriteMask_Front;
		FStencilOPType m_eStencilFailOP_Front;
		FStencilOPType m_eStencilDepthFailOP_Front;
		FStencilOPType m_eStencilPassOP_Front;
		FCompareFuncType m_eStencilCompareFunc_Back;
		uint32 m_nStencilRefValue_Back;
		uint32 m_nStencilCompareMask_Back;
		uint32 m_nStencilWriteMask_Back;
		FStencilOPType m_eStencilFailOP_Back;
		FStencilOPType m_eStencilDepthFailOP_Back;
		FStencilOPType m_eStencilPassOP_Back;
			
		bool m_bScissorTestEnabled;
		uint32 m_nScissorTest_Left;
		uint32 m_nScissorTest_Top;
		uint32 m_nScissorTest_Right;
		uint32 m_nScissorTest_Bottom;

		bool m_bAlphaTestEnabled;
		FCompareFuncType m_eAlphaRejectCompareFunc;
		uint8 m_nAlphaRejectValue;
		
		bool m_bSceneBlendingEnabled;
		FSceneBlendingOPType m_eSceneBlendingOP_Color;
		FSceneBlendingFactorType m_eSceneBlendingFactorSrc_Color;
		FSceneBlendingFactorType m_eSceneBlendingFactorDst_Color;
		FSceneBlendingOPType m_eSceneBlendingOP_Alpha;
		FSceneBlendingFactorType m_eSceneBlendingFactorSrc_Alpha;
		FSceneBlendingFactorType m_eSceneBlendingFactorDst_Alpha;

		bool m_bColorRWriteEnabled;
		bool m_bColorGWriteEnabled;
		bool m_bColorBWriteEnabled;
		bool m_bColorAWriteEnabled;

	public:
		void CopyFrom(const RenderStateCommon* pStateCommon);
		void CopyTo(RenderStateCommon* pStateCommon);
	};


	//////////////////////////////////// RenderStateLighting ////////////////////////////
	class engineExport RenderStateLighting : public FNonCopyable
	{
	public:
		RenderStateLighting();
		~RenderStateLighting();

	public:
		bool m_bLightingEnabled;
		FLightingType m_eLighting;
		uint8 m_nMaxEffectLightNum;

	////Flat/Gouraud/Phong
		FColor m_clAmbient;
		FColor m_clDiffuse;
		FColor m_clSpecular;
		FColor m_clEmissive;
		float m_fShininess;
		uint32 m_nColorFromVertexFlag;

	////Pbr


	public:
		void CopyFrom(const RenderStateLighting* pStateLighting);
		void CopyTo(RenderStateLighting* pStateLighting);
	};


	//////////////////////////////////// RenderStateParam ///////////////////////////////
	class engineExport RenderStateParam : public Base
									    , public FNonCopyable
	{
	public:
		RenderStateParam(const String& _nameParam);
		~RenderStateParam();

	public:
	protected:

	public:

	public:
		void Destroy();

		
	};

	//////////////////////////////////// RenderStateTexture /////////////////////////////
	class engineExport RenderStateTexture : public Base
										  , public FNonCopyable
	{
	public:
		RenderStateTexture(uint32 _group, const String& _nameTexture);
		~RenderStateTexture();

	public:
	protected:
		Texture* m_pTexture;

	public:
		F_FORCEINLINE Texture* GetTexture() const { return m_pTexture; }

	public:
		void Destroy();

		bool LoadTexture();
		void UnloadTexture();
	};

	
	//////////////////////////////////// RenderStateShader //////////////////////////////
	class engineExport RenderStateShaderProgramItem : public Base
										   	 		, public FNonCopyable
	{
	public:
		RenderStateShaderProgramItem(const String& nameShaderProgram, FShaderType type);
		~RenderStateShaderProgramItem();

	public:
		FShaderType m_eShader;
		ShaderProgram* m_pShaderProgram;

	////Param
		RenderStateParamPtrVector m_aRenderStateParam;
		RenderStateParamPtrMap m_mapRenderStateParam;
	////Texture
		RenderStateTexturePtrVector m_aRenderStateTexture;
		RenderStateTexturePtrMap m_mapRenderStateTexture;

	public:
		F_FORCEINLINE FShaderType GetShaderType() const { return m_eShader; }
		F_FORCEINLINE ShaderProgram* GetShaderProgram() const { return m_pShaderProgram; }

	////Param
		F_FORCEINLINE const RenderStateParamPtrVector& GetRenderStateParamPtrVector() const { return m_aRenderStateParam; }
        F_FORCEINLINE RenderStateParamPtrVector& GetRenderStateParamPtrVector() { return m_aRenderStateParam; }
        F_FORCEINLINE const RenderStateParamPtrMap& GetRenderStateParamPtrMap() const { return m_mapRenderStateParam; }
        F_FORCEINLINE RenderStateParamPtrMap& GetRenderStateParamPtrMap() { return m_mapRenderStateParam; }
	////Texture
		F_FORCEINLINE const RenderStateTexturePtrVector& GetRenderStateTexturePtrVector() const { return m_aRenderStateTexture; }
        F_FORCEINLINE RenderStateTexturePtrVector& GetRenderStateTexturePtrVector() { return m_aRenderStateTexture; }
        F_FORCEINLINE const RenderStateTexturePtrMap& GetRenderStateTexturePtrMap() const { return m_mapRenderStateTexture; }
        F_FORCEINLINE RenderStateTexturePtrMap& GetRenderStateTexturePtrMap() { return m_mapRenderStateTexture; }

	public:	
		void Destroy();

		bool LoadShaderProgram();
		void UnloadShader();
		
	////Param
	public:
		int GetStateParamCount() const;
		RenderStateParam* GetStateParam(int index) const;
		RenderStateParam* GetStateParamByName(const String& name);
		void AddStateParam(RenderStateParam* pStateParam);
		void DeleteStateParam(RenderStateParam* pStateParam);
		void DeleteStateParam(int index);
		void DeleteStateParamAll();

	////Texture
	public:
		int GetStateTextureCount() const;
		RenderStateTexture* GetStateTexture(int index) const;
		RenderStateTexture* GetStateTextureByName(const String& name);
		void AddStateTexture(RenderStateTexture* pStateTexture);
		void DeleteStateTexture(RenderStateTexture* pStateTexture);
		void DeleteStateTexture(int index);
		void DeleteStateTextureAll();

	};

	class engineExport RenderStateShader : public FNonCopyable
	{
	public:
		RenderStateShader();
		~RenderStateShader();

	public:
	protected:
		String m_strNameDescriptorSetLayout;

		RenderStateShaderProgramItemPtrVector m_aRenderStateShaderProgramItem;
		RenderStateShaderProgramItemPtrMap m_mapRenderStateShaderProgramItem;

	public:
		F_FORCEINLINE const String& GetNameDescriptorSetLayout() const { return m_strNameDescriptorSetLayout; }
		F_FORCEINLINE void SetNameDescriptorSetLayout(const String& nameDSL) { m_strNameDescriptorSetLayout = nameDSL; }

		F_FORCEINLINE const RenderStateShaderProgramItemPtrVector& GetRenderStateShaderProgramItemPtrVector() const { return m_aRenderStateShaderProgramItem; }
        F_FORCEINLINE RenderStateShaderProgramItemPtrVector& GetRenderStateShaderProgramItemPtrVector() { return m_aRenderStateShaderProgramItem; }
        F_FORCEINLINE const RenderStateShaderProgramItemPtrMap& GetRenderStateShaderProgramItemPtrMap() const { return m_mapRenderStateShaderProgramItem; }
        F_FORCEINLINE RenderStateShaderProgramItemPtrMap& GetRenderStateShaderProgramItemPtrMap() { return m_mapRenderStateShaderProgramItem; }

	public:
		void Destroy();

	public:
		bool HasRenderStateShaderProgramItem(const String& nameShaderProgram);
		RenderStateShaderProgramItem* GetRenderStateShaderProgramItem(const String& nameShaderProgram);

		void AddRenderStateShaderProgramItem(RenderStateShaderProgramItem* pItem);
		void DeleteRenderStateShaderProgramItem(const String& nameShaderProgram);
		void DeleteRenderStateShaderProgramItem(RenderStateShaderProgramItem* pItem);
		void DeleteRenderStateShaderProgramItemAll();
	};


	//////////////////////////////////// RenderState ////////////////////////////////////
    class engineExport RenderState : public Base
								   , public FNonCopyable
    {
    public:
        RenderState(const String& namePass,
					FRenderPassType typeRenderPass);
        virtual ~RenderState();

    public:
    protected:
		FRenderPassType m_typeRenderPass;
		RenderStateCommon* m_pStateCommon;
		RenderStateLighting* m_pStateLighting;
		RenderStateShader* m_pStateShader;

    public:
		F_FORCEINLINE RenderStateCommon* GetStateCommon() const { return m_pStateCommon; }
		F_FORCEINLINE RenderStateLighting* GetStateLighting() const { return m_pStateLighting; }
		F_FORCEINLINE RenderStateShader* GetStateShader() const { return m_pStateShader; }

	////RenderStateCommon
		F_FORCEINLINE FPolygonType GetPolygonType() const { return m_pStateCommon->m_ePolygon; }
		F_FORCEINLINE void SetPolygonType(const FPolygonType& ePolygon) { m_pStateCommon->m_ePolygon = ePolygon; }
		F_FORCEINLINE FFrontFaceType GetFrontFaceType() const { return m_pStateCommon->m_eFrontFace; }
		F_FORCEINLINE void SetFrontFaceType(const FFrontFaceType& eFrontFace) { m_pStateCommon->m_eFrontFace = eFrontFace; }
		F_FORCEINLINE FCullingType GetCullingType() const { return m_pStateCommon->m_eCulling; }
		F_FORCEINLINE void SetCullingType(const FCullingType& eCulling) { m_pStateCommon->m_eCulling = eCulling; }


		F_FORCEINLINE float GetPointSize() const { return m_pStateCommon->m_fPointSize; }
		F_FORCEINLINE void SetPointSize(const float& fPointSize) { m_pStateCommon->m_fPointSize = fPointSize; }
		F_FORCEINLINE bool GetPointSpriteEnabled() const { return m_pStateCommon->m_bPointSpriteEnabled; }
		F_FORCEINLINE void SetPointSpriteEnabled(const bool& bEnabled) { m_pStateCommon->m_bPointSpriteEnabled = bEnabled; }
		F_FORCEINLINE bool GetPointAttenuEnabled() const { return m_pStateCommon->m_bPointAttenuEnabled; }
		F_FORCEINLINE void SetPointAttenuEnabled(const bool& bEnabled) { m_pStateCommon->m_bPointAttenuEnabled = bEnabled; }
		F_FORCEINLINE float GetPointAttenuConstant() const	{ return m_pStateCommon->m_fPointAttenuConstant; }
		F_FORCEINLINE void SetPointAttenuConstant(const float& fPointAttenuConstant) { m_pStateCommon->m_fPointAttenuConstant = fPointAttenuConstant; }
		F_FORCEINLINE float GetPointAttenuLinear() const { return m_pStateCommon->m_fPointAttenuLinear; }
		F_FORCEINLINE void SetPointAttenuLinear(const float& fPointAttenuLinear) { m_pStateCommon->m_fPointAttenuLinear = fPointAttenuLinear; }
		F_FORCEINLINE float GetPointAttenuQuadratic() const { return m_pStateCommon->m_fPointAttenuQuadratic; }
		F_FORCEINLINE void SetPointAttenuQuadratic(const float& fPointAttenuQuadratic) { m_pStateCommon->m_fPointAttenuQuadratic = fPointAttenuQuadratic; }
		F_FORCEINLINE float GetPointMinSize() const { return m_pStateCommon->m_fPointMinSize; }
		F_FORCEINLINE void SetPointMinSize(const float& fPointMinSize) { m_pStateCommon->m_fPointMinSize = fPointMinSize; }
		F_FORCEINLINE float GetPointMaxSize() const { return m_pStateCommon->m_fPointMaxSize; }
		F_FORCEINLINE void SetPointMaxSize(const float& fPointMaxSize) { m_pStateCommon->m_fPointMaxSize = fPointMaxSize; }
		F_FORCEINLINE void GetPointSetting(float& fSize, bool& bSpriteEnabled, bool& bAttenuEnabled,
										   float& fAttenuConstant, float& fAttenuLinear, float& fAttenuQuadratic,
										   float& fMinSize, float& fMaxSize) const
		{
			fSize = m_pStateCommon->m_fPointSize;
			bSpriteEnabled = m_pStateCommon->m_bPointSpriteEnabled;
			bAttenuEnabled = m_pStateCommon->m_bPointAttenuEnabled;
			fAttenuConstant = m_pStateCommon->m_fPointAttenuConstant;
			fAttenuLinear = m_pStateCommon->m_fPointAttenuLinear;
			fAttenuQuadratic = m_pStateCommon->m_fPointAttenuQuadratic;
			fMinSize = m_pStateCommon->m_fPointMinSize;
			fMaxSize = m_pStateCommon->m_fPointMaxSize;
		}
		F_FORCEINLINE void SetPointSetting(float fSize, bool bSpriteEnabled, bool bAttenuEnabled,
										   float fAttenuConstant, float fAttenuLinear, float fAttenuQuadratic,
										   float fMinSize, float fMaxSize)
		{

			m_pStateCommon->m_fPointSize = fSize;
			m_pStateCommon->m_bPointSpriteEnabled = bSpriteEnabled;
			m_pStateCommon->m_bPointAttenuEnabled = bAttenuEnabled;
			m_pStateCommon->m_fPointAttenuConstant = fAttenuConstant;
			m_pStateCommon->m_fPointAttenuLinear = fAttenuLinear;
			m_pStateCommon->m_fPointAttenuQuadratic = fAttenuQuadratic;
			m_pStateCommon->m_fPointMinSize = fMinSize;
			m_pStateCommon->m_fPointMaxSize = fMaxSize;
		}


		F_FORCEINLINE bool GetDepthTestEnabled() const { return m_pStateCommon->m_bDepthTestEnabled; }
		F_FORCEINLINE void SetDepthTestEnabled(const bool& bEnabled) { m_pStateCommon->m_bDepthTestEnabled = bEnabled; }
		F_FORCEINLINE bool GetDepthWriteEnabled() const { return m_pStateCommon->m_bDepthWriteEnabled; }
		F_FORCEINLINE void SetDepthWriteEnabled(const bool& bEnabled) { m_pStateCommon->m_bDepthWriteEnabled = bEnabled; }
		F_FORCEINLINE FCompareFuncType GetDepthCompareFunc() const { return m_pStateCommon->m_eDepthCompareFunc; }
		F_FORCEINLINE void SetDepthCompareFunc(const FCompareFuncType& eDepthCompareFunc) { m_pStateCommon->m_eDepthCompareFunc = eDepthCompareFunc; }
		F_FORCEINLINE float GetDepthBiasConstant() const { return m_pStateCommon->m_fDepthBiasConstant; }
		F_FORCEINLINE void SetDepthBiasConstant(const float& fDepthBiasConstant) { m_pStateCommon->m_fDepthBiasConstant = fDepthBiasConstant; }
		F_FORCEINLINE float GetDepthBiasSlopeScale() const	{ return m_pStateCommon->m_fDepthBiasSlopeScale; }
		F_FORCEINLINE void SetDepthBiasSlopeScale(const float& fDepthBiasSlopeScale) { m_pStateCommon->m_fDepthBiasSlopeScale = fDepthBiasSlopeScale; }
		F_FORCEINLINE void GetDepthSetting(bool& bDepthTestEnabled, bool& bDepthWriteEnabled, FCompareFuncType& eDepthCompareFunc,
										   float& fDepthBiasConstant, float& fDepthBiasSlopeScale) const
		{
			bDepthTestEnabled = m_pStateCommon->m_bDepthTestEnabled;
			bDepthWriteEnabled = m_pStateCommon->m_bDepthWriteEnabled;
			eDepthCompareFunc = m_pStateCommon->m_eDepthCompareFunc;
			fDepthBiasConstant = m_pStateCommon->m_fDepthBiasConstant;
			fDepthBiasSlopeScale = m_pStateCommon->m_fDepthBiasSlopeScale;
		}
		F_FORCEINLINE void SetDepthSetting(bool bDepthTestEnabled, bool bDepthWriteEnabled, FCompareFuncType eDepthCompareFunc,
										   float fDepthBiasConstant, float fDepthBiasSlopeScale)
		{
			m_pStateCommon->m_bDepthTestEnabled = bDepthTestEnabled;
			m_pStateCommon->m_bDepthWriteEnabled = bDepthWriteEnabled;
			m_pStateCommon->m_eDepthCompareFunc = eDepthCompareFunc;
			m_pStateCommon->m_fDepthBiasConstant = fDepthBiasConstant;
			m_pStateCommon->m_fDepthBiasSlopeScale = fDepthBiasSlopeScale;
		}	


		F_FORCEINLINE bool GetStencilEnabled() const { return m_pStateCommon->m_bStencilEnabled; }
		F_FORCEINLINE void SetStencilEnabled(const bool& bEnabled) { m_pStateCommon->m_bStencilEnabled = bEnabled; }
		F_FORCEINLINE bool GetStencilTwoSidedEnabled() const { return m_pStateCommon->m_bStencilTwoSidedEnabled; }
		F_FORCEINLINE void SetStencilTwoSidedEnabled(const bool& bEnabled) { m_pStateCommon->m_bStencilTwoSidedEnabled = bEnabled; }
		//Front
		F_FORCEINLINE FCompareFuncType GetStencilCompareFunc_Front() const { return m_pStateCommon->m_eStencilCompareFunc_Front; }
		F_FORCEINLINE void SetStencilCompareFunc_Front(const FCompareFuncType& eStencilCompareFunc) { m_pStateCommon->m_eStencilCompareFunc_Front = eStencilCompareFunc; }
		F_FORCEINLINE uint32 GetStencilRefValue_Front() const { return m_pStateCommon->m_nStencilRefValue_Front; }
		F_FORCEINLINE void SetStencilRefValue_Front(const uint32& nStencilRefValue) { m_pStateCommon->m_nStencilRefValue_Front = nStencilRefValue; }
		F_FORCEINLINE uint32 GetStencilCompareMask_Front() const { return m_pStateCommon->m_nStencilCompareMask_Front; }
		F_FORCEINLINE void SetStencilCompareMask_Front(const uint32& nStencilCompareMask) { m_pStateCommon->m_nStencilCompareMask_Front = nStencilCompareMask; }
		F_FORCEINLINE uint32 GetStencilWriteMask_Front() const { return m_pStateCommon->m_nStencilWriteMask_Front; }
		F_FORCEINLINE void SetStencilWriteMask_Front(const uint32& nStencilWriteMask) { m_pStateCommon->m_nStencilWriteMask_Front = nStencilWriteMask; }
		F_FORCEINLINE FStencilOPType GetStencilFailOP_Front() const { return m_pStateCommon->m_eStencilFailOP_Front; }
		F_FORCEINLINE void SetStencilFailOP_Front(const FStencilOPType& eStencilFailOP) { m_pStateCommon->m_eStencilFailOP_Front = eStencilFailOP; }
		F_FORCEINLINE FStencilOPType GetStencilDepthFailOP_Front() const { return m_pStateCommon->m_eStencilDepthFailOP_Front; }
		F_FORCEINLINE void SetStencilDepthFailOP_Front(const FStencilOPType& eStencilDepthFailOP) { m_pStateCommon->m_eStencilDepthFailOP_Front = eStencilDepthFailOP; }
		F_FORCEINLINE FStencilOPType GetStencilPassOP_Front() const { return m_pStateCommon->m_eStencilPassOP_Front; }
		F_FORCEINLINE void SetStencilPassOP_Front(const FStencilOPType& eStencilPassOP) { m_pStateCommon->m_eStencilPassOP_Front = eStencilPassOP; }
		//Back
		F_FORCEINLINE FCompareFuncType GetStencilCompareFunc_Back() const { return m_pStateCommon->m_eStencilCompareFunc_Back; }
		F_FORCEINLINE void SetStencilCompareFunc_Back(const FCompareFuncType& eStencilCompareFunc) { m_pStateCommon->m_eStencilCompareFunc_Back = eStencilCompareFunc; }
		F_FORCEINLINE uint32 GetStencilRefValue_Back() const { return m_pStateCommon->m_nStencilRefValue_Back; }
		F_FORCEINLINE void SetStencilRefValue_Back(const uint32& nStencilRefValue) { m_pStateCommon->m_nStencilRefValue_Back = nStencilRefValue; }
		F_FORCEINLINE uint32 GetStencilCompareMask_Back() const { return m_pStateCommon->m_nStencilCompareMask_Back; }
		F_FORCEINLINE void SetStencilCompareMask_Back(const uint32& nStencilCompareMask) { m_pStateCommon->m_nStencilCompareMask_Back = nStencilCompareMask; }
		F_FORCEINLINE uint32 GetStencilWriteMask_Back() const { return m_pStateCommon->m_nStencilWriteMask_Back; }
		F_FORCEINLINE void SetStencilWriteMask_Back(const uint32& nStencilWriteMask) { m_pStateCommon->m_nStencilWriteMask_Back = nStencilWriteMask; }
		F_FORCEINLINE FStencilOPType GetStencilFailOP_Back() const { return m_pStateCommon->m_eStencilFailOP_Back; }
		F_FORCEINLINE void SetStencilFailOP_Back(const FStencilOPType& eStencilFailOP) { m_pStateCommon->m_eStencilFailOP_Back = eStencilFailOP; }
		F_FORCEINLINE FStencilOPType GetStencilDepthFailOP_Back() const { return m_pStateCommon->m_eStencilDepthFailOP_Back; }
		F_FORCEINLINE void SetStencilDepthFailOP_Back(const FStencilOPType& eStencilDepthFailOP) { m_pStateCommon->m_eStencilDepthFailOP_Back = eStencilDepthFailOP; }
		F_FORCEINLINE FStencilOPType GetStencilPassOP_Back() const { return m_pStateCommon->m_eStencilPassOP_Back; }
		F_FORCEINLINE void SetStencilPassOP_Back(const FStencilOPType& eStencilPassOP) { m_pStateCommon->m_eStencilPassOP_Back = eStencilPassOP; }
		
		F_FORCEINLINE void GetStencilSetting(bool& bEnabled, 
											 bool& bTwoSided,
											 FCompareFuncType& eType,
											 uint32& nRefValue, 
											 uint32& nCompareMask,
											 uint32& nWriteMask,
											 FStencilOPType& eFailOP, 
											 FStencilOPType& eDepthFailOP, 
											 FStencilOPType& ePassOP,
											 bool bIsFront = true) const
		{
			bEnabled = m_pStateCommon->m_bStencilEnabled;
			bTwoSided = m_pStateCommon->m_bStencilTwoSidedEnabled;
			if (bIsFront)
			{
				eType = m_pStateCommon->m_eStencilCompareFunc_Front;
				nRefValue = m_pStateCommon->m_nStencilRefValue_Front;
				nCompareMask = m_pStateCommon->m_nStencilCompareMask_Front;
				nWriteMask = m_pStateCommon->m_nStencilWriteMask_Front;
				eFailOP = m_pStateCommon->m_eStencilFailOP_Front;
				eDepthFailOP = m_pStateCommon->m_eStencilDepthFailOP_Front;
				ePassOP	= m_pStateCommon->m_eStencilPassOP_Front;
			}
			else
			{
				eType = m_pStateCommon->m_eStencilCompareFunc_Back;
				nRefValue = m_pStateCommon->m_nStencilRefValue_Back;
				nCompareMask = m_pStateCommon->m_nStencilCompareMask_Back;
				nWriteMask = m_pStateCommon->m_nStencilWriteMask_Back;
				eFailOP = m_pStateCommon->m_eStencilFailOP_Back;
				eDepthFailOP = m_pStateCommon->m_eStencilDepthFailOP_Back;
				ePassOP	= m_pStateCommon->m_eStencilPassOP_Back;
			}
		}
		F_FORCEINLINE void SetStencilSetting(bool bEnabled, 
											 bool bTwoSided,
											 FCompareFuncType eType,
											 uint32 nRefValue, 
											 uint32 nCompareMask,
											 uint32 nWriteMask,
											 FStencilOPType eFailOP, 
											 FStencilOPType eDepthFailOP, 
											 FStencilOPType ePassOP,
											 bool bIsFront = true)
		{
			m_pStateCommon->m_bStencilEnabled = bEnabled;
			m_pStateCommon->m_bStencilTwoSidedEnabled = bTwoSided;
			if (bIsFront)
			{
				m_pStateCommon->m_eStencilCompareFunc_Front = eType;		  
				m_pStateCommon->m_nStencilRefValue_Front = nRefValue;	  
				m_pStateCommon->m_nStencilCompareMask_Front = nCompareMask;		
				m_pStateCommon->m_nStencilWriteMask_Front = nWriteMask;		  
				m_pStateCommon->m_eStencilFailOP_Front = eFailOP;		  
				m_pStateCommon->m_eStencilDepthFailOP_Front = eDepthFailOP;  
				m_pStateCommon->m_eStencilPassOP_Front = ePassOP;		  
			}
			else
			{
				m_pStateCommon->m_eStencilCompareFunc_Back = eType;		  
				m_pStateCommon->m_nStencilRefValue_Back = nRefValue;	  
				m_pStateCommon->m_nStencilCompareMask_Back = nCompareMask;		
				m_pStateCommon->m_nStencilWriteMask_Back = nWriteMask;			  
				m_pStateCommon->m_eStencilFailOP_Back = eFailOP;		  
				m_pStateCommon->m_eStencilDepthFailOP_Back = eDepthFailOP;  
				m_pStateCommon->m_eStencilPassOP_Back = ePassOP;		 
			}
		}


		F_FORCEINLINE bool GetScissorTestEnabled() const { return m_pStateCommon->m_bScissorTestEnabled; }
		F_FORCEINLINE void SetScissorTestEnabled(const bool& bEnabled) { m_pStateCommon->m_bScissorTestEnabled = bEnabled; }
		F_FORCEINLINE void GetScissorTestParam(uint32& nLeft, uint32& nTop, uint32& nRight, uint32& nBottom)
		{
			nLeft = m_pStateCommon->m_nScissorTest_Left;
			nTop = m_pStateCommon->m_nScissorTest_Top;
			nRight = m_pStateCommon->m_nScissorTest_Right;
			nBottom = m_pStateCommon->m_nScissorTest_Bottom;
		}
		F_FORCEINLINE void SetScissorTestParam(bool bEnabled, uint32 nLeft, uint32 nTop, uint32 nRight, uint32 nBottom)
		{
			m_pStateCommon->m_bScissorTestEnabled = bEnabled;
			m_pStateCommon->m_nScissorTest_Left = nLeft;
			m_pStateCommon->m_nScissorTest_Top = nTop;
			m_pStateCommon->m_nScissorTest_Right = nRight;
			m_pStateCommon->m_nScissorTest_Bottom = nBottom;
		}	


		F_FORCEINLINE bool GetAlphaTestEnabled() const { return m_pStateCommon->m_bAlphaTestEnabled; }
		F_FORCEINLINE void SetAlphaTestEnabled(const bool& bEnabled) { m_pStateCommon->m_bAlphaTestEnabled = bEnabled; }
		F_FORCEINLINE FCompareFuncType GetAlphaRejectCompareFunc() const { return m_pStateCommon->m_eAlphaRejectCompareFunc; }
		F_FORCEINLINE void SetAlphaRejectCompareFunc(const FCompareFuncType& eAlphaRejectCompareFunc) { m_pStateCommon->m_eAlphaRejectCompareFunc = eAlphaRejectCompareFunc; }
		F_FORCEINLINE uint8 GetAlphaRejectValue() const { return m_pStateCommon->m_nAlphaRejectValue; }
		F_FORCEINLINE void SetAlphaRejectValue(const uint8& c) { m_pStateCommon->m_nAlphaRejectValue = c; }
		F_FORCEINLINE void GetAlphaTestSetting(bool& bEnabled, FCompareFuncType& eAlphaRejectCompareFunc, uint8& nValue) const
		{
			bEnabled = m_pStateCommon->m_bAlphaTestEnabled;
			eAlphaRejectCompareFunc = m_pStateCommon->m_eAlphaRejectCompareFunc;
			nValue = m_pStateCommon->m_nAlphaRejectValue;
		}
		F_FORCEINLINE void SetAlphaTestSetting(bool bEnabled, FCompareFuncType eAlphaRejectCompareFunc, uint8 nValue)
		{
			m_pStateCommon->m_bAlphaTestEnabled = bEnabled;
			m_pStateCommon->m_eAlphaRejectCompareFunc = eAlphaRejectCompareFunc;
			m_pStateCommon->m_nAlphaRejectValue = nValue;
		}
		

		F_FORCEINLINE bool GetSceneBlendingEnabled() const	{ return m_pStateCommon->m_bSceneBlendingEnabled; }
		F_FORCEINLINE void SetSceneBlendingEnabled(const bool& bEnabled) { m_pStateCommon->m_bSceneBlendingEnabled = bEnabled; }
		
		F_FORCEINLINE FSceneBlendingOPType GetSceneBlendingOperationOPType_Color() const { return m_pStateCommon->m_eSceneBlendingOP_Color; }
		F_FORCEINLINE void SetSceneBlendingOperationOPType_Color(const FSceneBlendingOPType& eSceneBlendingOP) { m_pStateCommon->m_eSceneBlendingOP_Color = eSceneBlendingOP; }
		F_FORCEINLINE FSceneBlendingFactorType GetSceneBlendingFactorSrcType_Color() const { return m_pStateCommon->m_eSceneBlendingFactorSrc_Color; }
		F_FORCEINLINE void SetSceneBlendingFactorSrcType_Color(const FSceneBlendingFactorType& eSceneBlendingFactorSrc) { m_pStateCommon->m_eSceneBlendingFactorSrc_Color = eSceneBlendingFactorSrc; }
		F_FORCEINLINE FSceneBlendingFactorType GetSceneBlendingFactorDstType_Color() const { return m_pStateCommon->m_eSceneBlendingFactorDst_Color; }
		F_FORCEINLINE void SetSceneBlendingFactorDstType_Color(const FSceneBlendingFactorType& eSceneBlendingFactorDst) { m_pStateCommon->m_eSceneBlendingFactorDst_Color = eSceneBlendingFactorDst; }
		
		F_FORCEINLINE FSceneBlendingOPType GetSceneBlendingOperationOPType_Alpha() const { return m_pStateCommon->m_eSceneBlendingOP_Alpha; }
		F_FORCEINLINE void SetSceneBlendingOperationOPType_Alpha(const FSceneBlendingOPType& eSceneBlendingOP) { m_pStateCommon->m_eSceneBlendingOP_Alpha = eSceneBlendingOP; }
		F_FORCEINLINE FSceneBlendingFactorType GetSceneBlendingFactorSrcType_Alpha() const { return m_pStateCommon->m_eSceneBlendingFactorSrc_Alpha; }
		F_FORCEINLINE void SetSceneBlendingFactorSrcType_Alpha(const FSceneBlendingFactorType& eSceneBlendingFactorSrc) { m_pStateCommon->m_eSceneBlendingFactorSrc_Alpha = eSceneBlendingFactorSrc; }
		F_FORCEINLINE FSceneBlendingFactorType GetSceneBlendingFactorDstType_Alpha() const { return m_pStateCommon->m_eSceneBlendingFactorDst_Alpha; }
		F_FORCEINLINE void SetSceneBlendingFactorDstType_Alpha(const FSceneBlendingFactorType& eSceneBlendingFactorDst) { m_pStateCommon->m_eSceneBlendingFactorDst_Alpha = eSceneBlendingFactorDst; }

		F_FORCEINLINE void GetSceneBlendingSetting(bool& bEnabled, 
												   FSceneBlendingOPType& eSBOT_Color, 
												   FSceneBlendingFactorType& eSrc_Color, 
												   FSceneBlendingFactorType& eDst_Color,
												   FSceneBlendingOPType& eSBOT_Alpha, 
												   FSceneBlendingFactorType& eSrc_Alpha, 
												   FSceneBlendingFactorType& eDst_Alpha) const
		{
			bEnabled = m_pStateCommon->m_bSceneBlendingEnabled;
			eSBOT_Color = m_pStateCommon->m_eSceneBlendingOP_Color;
			eSrc_Color = m_pStateCommon->m_eSceneBlendingFactorSrc_Color;
			eDst_Color = m_pStateCommon->m_eSceneBlendingFactorDst_Color;
			eSBOT_Alpha = m_pStateCommon->m_eSceneBlendingOP_Alpha;
			eSrc_Alpha = m_pStateCommon->m_eSceneBlendingFactorSrc_Alpha;
			eDst_Alpha = m_pStateCommon->m_eSceneBlendingFactorDst_Alpha;
		}
		F_FORCEINLINE void SetSceneBlendingSetting(bool bEnabled,
												   FSceneBlendingOPType eSBOT_Color, 
												   FSceneBlendingFactorType eSrc_Color, 
												   FSceneBlendingFactorType eDst_Color,
												   FSceneBlendingOPType eSBOT_Alpha, 
												   FSceneBlendingFactorType eSrc_Alpha, 
												   FSceneBlendingFactorType eDst_Alpha)
		{
			m_pStateCommon->m_bSceneBlendingEnabled = bEnabled;
			m_pStateCommon->m_eSceneBlendingOP_Color = eSBOT_Color; 
			m_pStateCommon->m_eSceneBlendingFactorSrc_Color = eSrc_Color;
			m_pStateCommon->m_eSceneBlendingFactorDst_Color = eDst_Color;
			m_pStateCommon->m_eSceneBlendingOP_Alpha = eSBOT_Alpha; 
			m_pStateCommon->m_eSceneBlendingFactorSrc_Alpha = eSrc_Alpha;
			m_pStateCommon->m_eSceneBlendingFactorDst_Alpha = eDst_Alpha;
		}

		
		F_FORCEINLINE bool GetColorRWriteEnabled() const { return m_pStateCommon->m_bColorRWriteEnabled; }
		F_FORCEINLINE void SetColorRWriteEnabled(const bool& bEnabled) { m_pStateCommon->m_bColorRWriteEnabled = bEnabled; }
		F_FORCEINLINE bool GetColorGWriteEnabled() const { return m_pStateCommon->m_bColorGWriteEnabled; }
		F_FORCEINLINE void SetColorGWriteEnabled(const bool& bEnabled) { m_pStateCommon->m_bColorGWriteEnabled = bEnabled; }
		F_FORCEINLINE bool GetColorBWriteEnabled() const { return m_pStateCommon->m_bColorBWriteEnabled; }
		F_FORCEINLINE void SetColorBWriteEnabled(const bool& bEnabled) { m_pStateCommon->m_bColorBWriteEnabled = bEnabled; }
		F_FORCEINLINE bool GetColorAWriteEnabled() const { return m_pStateCommon->m_bColorAWriteEnabled; }
		F_FORCEINLINE void SetColorAWriteEnabled(const bool& bEnabled) { m_pStateCommon->m_bColorAWriteEnabled = bEnabled; }

		F_FORCEINLINE void GetColorWriteEnabled(bool& r, bool& g, bool& b, bool& a) const					
		{ 
			r = m_pStateCommon->m_bColorRWriteEnabled;
			g = m_pStateCommon->m_bColorGWriteEnabled;
			b = m_pStateCommon->m_bColorBWriteEnabled;
			a = m_pStateCommon->m_bColorAWriteEnabled;
		}
		F_FORCEINLINE void SetColorWriteEnabled(bool r, bool g, bool b, bool a)  	
		{
			m_pStateCommon->m_bColorRWriteEnabled = r;
			m_pStateCommon->m_bColorGWriteEnabled = g;
			m_pStateCommon->m_bColorBWriteEnabled = b;
			m_pStateCommon->m_bColorAWriteEnabled = a;
		}

	////RenderStateLighting
		F_FORCEINLINE bool GetLightingEnabled() const { return m_pStateLighting->m_bLightingEnabled; }
		F_FORCEINLINE void SetLightingEnabled(bool b) { m_pStateLighting->m_bLightingEnabled = b; }

		F_FORCEINLINE FLightingType GetLightingType() const { return m_pStateLighting->m_eLighting; }
		F_FORCEINLINE void SetLightingType(const FLightingType& eLighting) { m_pStateLighting->m_eLighting = eLighting; }

		F_FORCEINLINE uint8 GetMaxEffectLightNum() const { return m_pStateLighting->m_nMaxEffectLightNum; }
		F_FORCEINLINE void SetMaxEffectLightNum(uint8 nNum) { m_pStateLighting->m_nMaxEffectLightNum = nNum; }
		
		F_FORCEINLINE const FColor& GetAmbient() const { return m_pStateLighting->m_clAmbient; }
		F_FORCEINLINE void SetAmbient(const FColor& cl) { m_pStateLighting->m_clAmbient = cl; }
		F_FORCEINLINE const FColor& GetDiffuse() const { return m_pStateLighting->m_clDiffuse; }
		F_FORCEINLINE void SetDiffuse(const FColor& cl)	{ m_pStateLighting->m_clDiffuse = cl; }
		F_FORCEINLINE const FColor& GetSpecular() const	{ return m_pStateLighting->m_clSpecular; }
		F_FORCEINLINE void SetSpecular(const FColor& cl) { m_pStateLighting->m_clSpecular = cl; }
		F_FORCEINLINE const FColor& GetEmissive() const { return m_pStateLighting->m_clEmissive; }
		F_FORCEINLINE void SetEmissive(const FColor& cl) { m_pStateLighting->m_clEmissive = cl; }
		F_FORCEINLINE float GetShininess() const { return m_pStateLighting->m_fShininess; }
		F_FORCEINLINE void SetShininess(float f) { m_pStateLighting->m_fShininess = f; }
		F_FORCEINLINE uint32 GetColorFromVertexFlag() const { return m_pStateLighting->m_nColorFromVertexFlag; }
		F_FORCEINLINE void SetColorFromVertexFlag(uint32 n) { m_pStateLighting->m_nColorFromVertexFlag = n; }


	////RenderStateShader



    public:
		void Destroy();


		void SetStateCommonFrom(const RenderStateCommon* pStateCommon);
		void SetStateCommonTo(RenderStateCommon* pStateCommon);

		void SetStateLightingFrom(const RenderStateLighting* pStateLighting);
		void SetStateLightingTo(RenderStateLighting* pStateLighting);

		void SetStateShaderFrom(const RenderStateShader* pStateShader);
		void SetStateShaderTo(RenderStateShader* pStateShader);
    };

}; //LostPeterEngine

#endif
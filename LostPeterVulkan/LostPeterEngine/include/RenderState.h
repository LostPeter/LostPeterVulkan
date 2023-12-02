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
		FPolygonType				typePolygon;
		FCullingType			    typeCulling;
		float					    fPointSize;
		bool					    bPointSpriteEnabled;
		bool					    bPointAttenuEnabled;
		float					    fPointAttenuConstant;
		float					    fPointAttenuLinear;
		float					    fPointAttenuQuadratic;
		float					    fPointMinSize;
		float					    fPointMaxSize;
		bool					    bDepthTestEnabled;
		bool					    bDepthWriteEnabled;
		FCompareFuncType			typeDepthFunc;
		float					    fDepthBiasConstant;
		float					    fDepthBiasSlopeScale;
		bool					    bStencilEnabled;
		FCompareFuncType			typeStencilFunc;
		uint32					    nStencilRefValue;
		uint32					    nStencilMask;
		FStencilOPType	            typeStencilFailOP;
		FStencilOPType	            typeStencilDepthFailOP;
		FStencilOPType	            typeStencilPassOP;
		bool					    bStencilTwoSidedEnabled;
		bool					    bScissorTestEnabled;
		uint32					    nScissorTest_Left;
		uint32					    nScissorTest_Top;
		uint32					    nScissorTest_Right;
		uint32					    nScissorTest_Bottom;
		bool 						bAlphaTestEnabled;
		FCompareFuncType 			typeAlphaRejectFunc;
		uint8					    nAlphaRejectValue;
		bool					    bSceneBlendingEnabled;
		FSceneBlendingType		    typeSceneBlending;
		FSceneBlendingOPType		typeSceneBlendingOP;
		FSceneBlendingFactorType	typeSceneBlendingFactorSrc;
		FSceneBlendingFactorType	typeSceneBlendingFactorDst;
		bool					    bSceneBlendingSeperateEnabled;
		FSceneBlendingOPType		typeSceneBlendingOP2;
		FSceneBlendingFactorType	typeSceneBlendingFactorSrc2;
		FSceneBlendingFactorType	typeSceneBlendingFactorDst2;
		bool					    bColorRWriteEnabled;
		bool					    bColorGWriteEnabled;
		bool					    bColorBWriteEnabled;
		bool					    bColorAWriteEnabled;

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
		bool					    bLightingEnabled;
		FLightingType 				typeLighting;
		uint8					    nMaxEffectLightNum;

	////Flat/Gouraud/Phong
		FColor					    clAmbient;
		FColor					    clDiffuse;
		FColor					    clSpecular;
		FColor					    clEmissive;
		float					    fShininess;
		uint32					    nColorFromVertexFlag;

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
		Texture* pTexture;

	public:
		E_FORCEINLINE Texture* GetTexture() const { return pTexture; }

	public:
		void Destroy();

		bool LoadTexture();
		void UnloadTexture();
	};

	
	//////////////////////////////////// RenderStateShader //////////////////////////////
	class engineExport RenderStateShaderItem : public Base
										   , public FNonCopyable
	{
	public:
		RenderStateShaderItem(const String& nameShader, FShaderType type);
		~RenderStateShaderItem();

	public:
		FShaderType typeShader;
		Shader* pShader;

	////Param
		RenderStateParamPtrVector aRenderStateParam;
		RenderStateParamPtrMap mapRenderStateParam;
	////Texture
		RenderStateTexturePtrVector aRenderStateTexture;
		RenderStateTexturePtrMap mapRenderStateTexture;

	public:
		E_FORCEINLINE FShaderType GetShaderType() const { return typeShader; }
		E_FORCEINLINE Shader* GetShader() const { return pShader; }

	////Param
		E_FORCEINLINE const RenderStateParamPtrVector& GetRenderStateParamPtrVector() const { return aRenderStateParam; }
        E_FORCEINLINE RenderStateParamPtrVector& GetRenderStateParamPtrVector() { return aRenderStateParam; }
        E_FORCEINLINE const RenderStateParamPtrMap& GetRenderStateParamPtrMap() const { return mapRenderStateParam; }
        E_FORCEINLINE RenderStateParamPtrMap& GetRenderStateParamPtrMap() { return mapRenderStateParam; }
	////Texture
		E_FORCEINLINE const RenderStateTexturePtrVector& GetRenderStateTexturePtrVector() const { return aRenderStateTexture; }
        E_FORCEINLINE RenderStateTexturePtrVector& GetRenderStateTexturePtrVector() { return aRenderStateTexture; }
        E_FORCEINLINE const RenderStateTexturePtrMap& GetRenderStateTexturePtrMap() const { return mapRenderStateTexture; }
        E_FORCEINLINE RenderStateTexturePtrMap& GetRenderStateTexturePtrMap() { return mapRenderStateTexture; }

	public:	
		void Destroy();

		bool LoadShader();
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
		String nameDescriptorSetLayout;

		RenderStateShaderItemPtrVector aRenderStateShaderItem;
		RenderStateShaderItemPtrMap mapRenderStateShaderItem;

	public:
		E_FORCEINLINE const String& GetNameDescriptorSetLayout() const { return nameDescriptorSetLayout; }
		E_FORCEINLINE void SetNameDescriptorSetLayout(const String& nameDSL) { nameDescriptorSetLayout = nameDSL; }

		E_FORCEINLINE const RenderStateShaderItemPtrVector& GetRenderStateShaderItemPtrVector() const { return aRenderStateShaderItem; }
        E_FORCEINLINE RenderStateShaderItemPtrVector& GetRenderStateShaderItemPtrVector() { return aRenderStateShaderItem; }
        E_FORCEINLINE const RenderStateShaderItemPtrMap& GetRenderStateShaderItemPtrMap() const { return mapRenderStateShaderItem; }
        E_FORCEINLINE RenderStateShaderItemPtrMap& GetRenderStateShaderItemPtrMap() { return mapRenderStateShaderItem; }

	public:
		void Destroy();

	public:
		bool HasRenderStateShaderItem(const String& nameShader);
		RenderStateShaderItem* GetRenderStateShaderItem(const String& nameShader);

		void AddRenderStateShaderItem(RenderStateShaderItem* pItem);
		void DeleteRenderStateShaderItem(const String& nameShader);
		void DeleteRenderStateShaderItem(RenderStateShaderItem* pItem);
		void DeleteRenderStateShaderItemAll();
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
		E_FORCEINLINE RenderStateCommon* GetStateCommon() const { return m_pStateCommon; }
		E_FORCEINLINE RenderStateLighting* GetStateLighting() const { return m_pStateLighting; }
		E_FORCEINLINE RenderStateShader* GetStateShader() const { return m_pStateShader; }

	////RenderStateCommon
		E_FORCEINLINE FPolygonType GetPolygonType() const { return m_pStateCommon->typePolygon; }
		E_FORCEINLINE void	SetPolygonType(const FPolygonType& t)	{ m_pStateCommon->typePolygon = t; }
		E_FORCEINLINE FCullingType GetCullingType() const { return m_pStateCommon->typeCulling; }
		E_FORCEINLINE void	SetCullingType(const FCullingType& t) { m_pStateCommon->typeCulling = t; }


		E_FORCEINLINE float GetPointSize() const { return m_pStateCommon->fPointSize; }
		E_FORCEINLINE void	SetPointSize(const float& f) { m_pStateCommon->fPointSize = f; }
		E_FORCEINLINE bool	GetPointSpriteEnabled() const { return m_pStateCommon->bPointSpriteEnabled; }
		E_FORCEINLINE void	SetPointSpriteEnabled(const bool& bEnabled) { m_pStateCommon->bPointSpriteEnabled = bEnabled; }
		E_FORCEINLINE bool	GetPointAttenuEnabled() const { return m_pStateCommon->bPointAttenuEnabled; }
		E_FORCEINLINE void	SetPointAttenuEnabled(const bool& bEnabled) { m_pStateCommon->bPointAttenuEnabled = bEnabled; }
		E_FORCEINLINE float GetPointAttenuConstant() const	{ return m_pStateCommon->fPointAttenuConstant; }
		E_FORCEINLINE void	SetPointAttenuConstant(const float& f) { m_pStateCommon->fPointAttenuConstant = f; }
		E_FORCEINLINE float GetPointAttenuLinear() const { return m_pStateCommon->fPointAttenuLinear; }
		E_FORCEINLINE void	SetPointAttenuLinear(const float& f) { m_pStateCommon->fPointAttenuLinear = f; }
		E_FORCEINLINE float GetPointAttenuQuadratic() const { return m_pStateCommon->fPointAttenuQuadratic; }
		E_FORCEINLINE void	SetPointAttenuQuadratic(const float& f) { m_pStateCommon->fPointAttenuQuadratic = f; }
		E_FORCEINLINE float GetPointMinSize() const { return m_pStateCommon->fPointMinSize; }
		E_FORCEINLINE void	SetPointMinSize(const float& f) { m_pStateCommon->fPointMinSize = f; }
		E_FORCEINLINE float GetPointMaxSize() const { return m_pStateCommon->fPointMaxSize; }
		E_FORCEINLINE void	SetPointMaxSize(const float& f) { m_pStateCommon->fPointMaxSize = f; }
		E_FORCEINLINE void GetPointSetting(float& fSize, bool& bSpriteEnabled, bool& bAttenuEnabled,
										    float& fAttenuConstant, float& fAttenuLinear, float& fAttenuQuadratic,
											float& fMinSize, float& fMaxSize) const
		{
			fSize = m_pStateCommon->fPointSize;
			bSpriteEnabled = m_pStateCommon->bPointSpriteEnabled;
			bAttenuEnabled = m_pStateCommon->bPointAttenuEnabled;
			fAttenuConstant = m_pStateCommon->fPointAttenuConstant;
			fAttenuLinear = m_pStateCommon->fPointAttenuLinear;
			fAttenuQuadratic = m_pStateCommon->fPointAttenuQuadratic;
			fMinSize = m_pStateCommon->fPointMinSize;
			fMaxSize = m_pStateCommon->fPointMaxSize;
		}
		E_FORCEINLINE void SetPointSetting(float fSize, bool bSpriteEnabled, bool bAttenuEnabled,
											float fAttenuConstant, float fAttenuLinear, float fAttenuQuadratic,
											float fMinSize, float fMaxSize)
		{

			m_pStateCommon->fPointSize = fSize;
			m_pStateCommon->bPointSpriteEnabled = bSpriteEnabled;
			m_pStateCommon->bPointAttenuEnabled = bAttenuEnabled;
			m_pStateCommon->fPointAttenuConstant = fAttenuConstant;
			m_pStateCommon->fPointAttenuLinear = fAttenuLinear;
			m_pStateCommon->fPointAttenuQuadratic = fAttenuQuadratic;
			m_pStateCommon->fPointMinSize = fMinSize;
			m_pStateCommon->fPointMaxSize = fMaxSize;
		}


		E_FORCEINLINE bool	GetDepthTestEnabled() const { return m_pStateCommon->bDepthTestEnabled; }
		E_FORCEINLINE void	SetDepthTestEnabled(const bool& bEnabled) { m_pStateCommon->bDepthTestEnabled = bEnabled; }
		E_FORCEINLINE bool	GetDepthWriteEnabled() const { return m_pStateCommon->bDepthWriteEnabled; }
		E_FORCEINLINE void	SetDepthWriteEnabled(const bool& bEnabled) { m_pStateCommon->bDepthWriteEnabled = bEnabled; }
		E_FORCEINLINE FCompareFuncType GetDepthFunc() const { return m_pStateCommon->typeDepthFunc; }
		E_FORCEINLINE void	SetDepthFunc(const FCompareFuncType& func) { m_pStateCommon->typeDepthFunc = func; }
		E_FORCEINLINE float GetDepthBiasConstant() const { return m_pStateCommon->fDepthBiasConstant; }
		E_FORCEINLINE void	SetDepthBiasConstant(const float& f) { m_pStateCommon->fDepthBiasConstant = f; }
		E_FORCEINLINE float GetDepthBiasSlopeScale() const	{ return m_pStateCommon->fDepthBiasSlopeScale; }
		E_FORCEINLINE void	SetDepthBiasSlopeScale(const float& f) { m_pStateCommon->fDepthBiasSlopeScale = f; }
		E_FORCEINLINE void	GetDepthSetting(bool& bDepthTestEnabled, bool& bDepthWriteEnabled, FCompareFuncType& eFunc,
											float& fDepthBiasConstant, float& fDepthBiasSlopeScale) const
		{
			bDepthTestEnabled = m_pStateCommon->bDepthTestEnabled;
			bDepthWriteEnabled = m_pStateCommon->bDepthWriteEnabled;
			eFunc = m_pStateCommon->typeDepthFunc;
			fDepthBiasConstant = m_pStateCommon->fDepthBiasConstant;
			fDepthBiasSlopeScale = m_pStateCommon->fDepthBiasSlopeScale;
		}
		E_FORCEINLINE void	SetDepthSetting(bool bDepthTestEnabled, bool bDepthWriteEnabled, FCompareFuncType eFunc,
										    float fDepthBiasConstant, float fDepthBiasSlopeScale)
		{
			m_pStateCommon->bDepthTestEnabled = bDepthTestEnabled;
			m_pStateCommon->bDepthWriteEnabled = bDepthWriteEnabled;
			m_pStateCommon->typeDepthFunc = eFunc;
			m_pStateCommon->fDepthBiasConstant = fDepthBiasConstant;
			m_pStateCommon->fDepthBiasSlopeScale = fDepthBiasSlopeScale;
		}


		E_FORCEINLINE bool	GetAlphaTestEnabled() const	{ return m_pStateCommon->bAlphaTestEnabled; }
		E_FORCEINLINE void	SetAlphaTestEnabled(const bool& bEnabled) { m_pStateCommon->bAlphaTestEnabled = bEnabled; }
		E_FORCEINLINE FCompareFuncType GetAlphaRejectFunc() const { return m_pStateCommon->typeAlphaRejectFunc; }
		E_FORCEINLINE void	SetAlphaRejectFunc(const FCompareFuncType& func) { m_pStateCommon->typeAlphaRejectFunc = func; }
		E_FORCEINLINE uint8 GetAlphaRejectValue() const { return m_pStateCommon->nAlphaRejectValue; }
		E_FORCEINLINE void	SetAlphaRejectValue(const uint8& c)	{ m_pStateCommon->nAlphaRejectValue = c; }
		E_FORCEINLINE void	GetAlphaTestSetting(bool& bEnabled, FCompareFuncType& eFunc, uint8& nValue) const
		{
			bEnabled = m_pStateCommon->bAlphaTestEnabled;
			eFunc = m_pStateCommon->typeAlphaRejectFunc;
			nValue = m_pStateCommon->nAlphaRejectValue;
		}
		E_FORCEINLINE void	SetAlphaTestSetting(bool bEnabled, FCompareFuncType eFunc, uint8 nValue)
		{
			m_pStateCommon->bAlphaTestEnabled = bEnabled;
			m_pStateCommon->typeAlphaRejectFunc = eFunc;
			m_pStateCommon->nAlphaRejectValue = nValue;
		}


		E_FORCEINLINE bool	GetStencilEnabled() const { return m_pStateCommon->bStencilEnabled; }
		E_FORCEINLINE void	SetStencilEnabled(const bool& bEnabled) { m_pStateCommon->bStencilEnabled = bEnabled; }
		E_FORCEINLINE FCompareFuncType GetStencilFunc() const { return m_pStateCommon->typeStencilFunc; }
		E_FORCEINLINE void	SetStencilFunc(const FCompareFuncType& func) { m_pStateCommon->typeStencilFunc = func; }
		E_FORCEINLINE uint32 GetStencilRefValue() const { return m_pStateCommon->nStencilRefValue; }
		E_FORCEINLINE void	SetStencilRefValue(const uint32& n)	{ m_pStateCommon->nStencilRefValue = n; }
		E_FORCEINLINE uint32 GetStencilMask() const { return m_pStateCommon->nStencilMask; }
		E_FORCEINLINE void	SetStencilMask(const uint32& n)	{ m_pStateCommon->nStencilMask = n; }
		E_FORCEINLINE FStencilOPType GetStencilFailOP() const { return m_pStateCommon->typeStencilFailOP; }
		E_FORCEINLINE void	SetStencilFailOP(const FStencilOPType& func) { m_pStateCommon->typeStencilFailOP = func; }
		E_FORCEINLINE FStencilOPType GetStencilDepthFailOP() const { return m_pStateCommon->typeStencilDepthFailOP; }
		E_FORCEINLINE void	SetStencilDepthFailOP(const FStencilOPType& func)	{ m_pStateCommon->typeStencilDepthFailOP = func; }
		E_FORCEINLINE FStencilOPType GetStencilPassOP() const { return m_pStateCommon->typeStencilPassOP; }
		E_FORCEINLINE void	SetStencilPassOP(const FStencilOPType& func) { m_pStateCommon->typeStencilPassOP = func; }
		E_FORCEINLINE bool	GetStencilTwoSidedEnabled() const { return m_pStateCommon->bStencilTwoSidedEnabled; }
		E_FORCEINLINE void	SetStencilTwoSidedEnabled(const bool& bEnabled) { m_pStateCommon->bStencilTwoSidedEnabled = bEnabled; }
		E_FORCEINLINE void	GetStencilSetting(bool& bEnabled, FCompareFuncType& eType,
											  uint32& nRefValue, uint32& nMask,
											  FStencilOPType& eFailOP, FStencilOPType& eDepthFailOP, FStencilOPType& ePassOP,
											  bool& bTwoSided) const
		{
			bEnabled = m_pStateCommon->bStencilEnabled;
			eType = m_pStateCommon->typeStencilFunc;
			nRefValue = m_pStateCommon->nStencilRefValue;
			nMask = m_pStateCommon->nStencilMask;
			eFailOP = m_pStateCommon->typeStencilFailOP;
			eDepthFailOP = m_pStateCommon->typeStencilDepthFailOP;
			ePassOP	= m_pStateCommon->typeStencilPassOP;
			bTwoSided = m_pStateCommon->bStencilTwoSidedEnabled;
		}
		E_FORCEINLINE void	SetStencilSetting(bool bEnabled, FCompareFuncType eType,
											  uint32 nRefValue, uint32 nMask,
											  FStencilOPType eFailOP, FStencilOPType eDepthFailOP, FStencilOPType ePassOP,
											  bool bTwoSided)
		{
			m_pStateCommon->bStencilEnabled = bEnabled;
			m_pStateCommon->typeStencilFunc = eType;		  
			m_pStateCommon->nStencilRefValue = nRefValue;	  
			m_pStateCommon->nStencilMask = nMask;		  
			m_pStateCommon->typeStencilFailOP = eFailOP;		  
			m_pStateCommon->typeStencilDepthFailOP = eDepthFailOP;  
			m_pStateCommon->typeStencilPassOP = ePassOP;		  
			m_pStateCommon->bStencilTwoSidedEnabled = bTwoSided;
		}

		E_FORCEINLINE bool	GetScissorTestEnabled() const { return m_pStateCommon->bScissorTestEnabled; }
		E_FORCEINLINE void	SetScissorTestEnabled(const bool& bEnabled) { m_pStateCommon->bScissorTestEnabled = bEnabled; }
		E_FORCEINLINE void	GetScissorTestParam(uint32& nLeft, uint32& nTop, uint32& nRight, uint32& nBottom)
		{
			nLeft = m_pStateCommon->nScissorTest_Left;
			nTop = m_pStateCommon->nScissorTest_Top;
			nRight = m_pStateCommon->nScissorTest_Right;
			nBottom = m_pStateCommon->nScissorTest_Bottom;
		}
		E_FORCEINLINE void	SetScissorTestParam(bool bEnabled, uint32 nLeft, uint32 nTop, uint32 nRight, uint32 nBottom)
		{
			m_pStateCommon->bScissorTestEnabled = bEnabled;
			m_pStateCommon->nScissorTest_Left = nLeft;
			m_pStateCommon->nScissorTest_Top = nTop;
			m_pStateCommon->nScissorTest_Right = nRight;
			m_pStateCommon->nScissorTest_Bottom = nBottom;
		}	

		
		E_FORCEINLINE bool	GetSceneBlendingEnabled() const	{ return m_pStateCommon->bSceneBlendingEnabled; }
		E_FORCEINLINE void	SetSceneBlendingEnabled(const bool& bEnabled) { m_pStateCommon->bSceneBlendingEnabled = bEnabled; }
		E_FORCEINLINE FSceneBlendingType GetSceneBlendingType() const { return m_pStateCommon->typeSceneBlending; }
		E_FORCEINLINE void	SetSceneBlendingType(const FSceneBlendingType& t)	{ m_pStateCommon->typeSceneBlending = t; }
		E_FORCEINLINE FSceneBlendingOPType GetSceneBlendingOperationOPType() const { return m_pStateCommon->typeSceneBlendingOP; }
		E_FORCEINLINE void	SetSceneBlendingOperationOPType(const FSceneBlendingOPType& t) { m_pStateCommon->typeSceneBlendingOP = t; }
		E_FORCEINLINE FSceneBlendingFactorType GetSceneBlendingFactorSrcType() const	{ return m_pStateCommon->typeSceneBlendingFactorSrc; }
		E_FORCEINLINE void	SetSceneBlendingFactorSrcType(const FSceneBlendingFactorType& t) { m_pStateCommon->typeSceneBlendingFactorSrc = t; }
		E_FORCEINLINE FSceneBlendingFactorType GetSceneBlendingFactorDstType() const	{ return m_pStateCommon->typeSceneBlendingFactorDst; }
		E_FORCEINLINE void	SetSceneBlendingFactorDstType(const FSceneBlendingFactorType& t) { m_pStateCommon->typeSceneBlendingFactorDst = t; }
		E_FORCEINLINE bool	GetSceneBlendingSeperateEnabled() const	{ return m_pStateCommon->bSceneBlendingSeperateEnabled; }
		E_FORCEINLINE void	SetSceneBlendingSeperateEnabled(const bool& bEnabled) { m_pStateCommon->bSceneBlendingSeperateEnabled = bEnabled; }
		E_FORCEINLINE FSceneBlendingOPType GetSceneBlendingOperationOPType2() const { return m_pStateCommon->typeSceneBlendingOP2; }
		E_FORCEINLINE void	SetSceneBlendingOperationOPType2(const FSceneBlendingOPType& t) { m_pStateCommon->typeSceneBlendingOP2 = t; }
		E_FORCEINLINE FSceneBlendingFactorType GetSceneBlendingFactorSrcType2() const { return m_pStateCommon->typeSceneBlendingFactorSrc2; }
		E_FORCEINLINE void	SetSceneBlendingFactorSrcType2(const FSceneBlendingFactorType& t) { m_pStateCommon->typeSceneBlendingFactorSrc2 = t; }
		E_FORCEINLINE FSceneBlendingFactorType GetSceneBlendingFactorDstType2() const { return m_pStateCommon->typeSceneBlendingFactorDst2; }
		E_FORCEINLINE void	SetSceneBlendingFactorDstType2(const FSceneBlendingFactorType& t) { m_pStateCommon->typeSceneBlendingFactorDst2 = t; }
		E_FORCEINLINE void	GetSceneBlendingSetting(bool& bEnabled, FSceneBlendingOPType& eSBOT,
													FSceneBlendingFactorType& eSrc, FSceneBlendingFactorType& eDst) const
		{
			bEnabled = m_pStateCommon->bSceneBlendingEnabled;
			eSBOT = m_pStateCommon->typeSceneBlendingOP;
			eSrc = m_pStateCommon->typeSceneBlendingFactorSrc;
			eDst = m_pStateCommon->typeSceneBlendingFactorDst;
		}
		E_FORCEINLINE void	SetSceneBlendingSetting(bool bEnabled, FSceneBlendingOPType eSBOT,
													FSceneBlendingFactorType eSrc, FSceneBlendingFactorType eDst)
		{
			m_pStateCommon->bSceneBlendingEnabled = bEnabled;
			m_pStateCommon->typeSceneBlendingOP = eSBOT; 
			m_pStateCommon->typeSceneBlendingFactorSrc = eSrc;
			m_pStateCommon->typeSceneBlendingFactorDst = eDst;
		}

		E_FORCEINLINE void	GetSceneBlendingSeperateSetting(bool& bEnabled, bool& bSeperateEnabled,
															FSceneBlendingOPType& eSBOT, FSceneBlendingFactorType& eSrc, FSceneBlendingFactorType& eDst,
															FSceneBlendingOPType& eSBOT2, FSceneBlendingFactorType& eSrc2, FSceneBlendingFactorType& eDst2) const
		{
			bEnabled = m_pStateCommon->bSceneBlendingEnabled;
			bSeperateEnabled = m_pStateCommon->bSceneBlendingSeperateEnabled;
			eSBOT = m_pStateCommon->typeSceneBlendingOP;
			eSrc = m_pStateCommon->typeSceneBlendingFactorSrc;
			eDst = m_pStateCommon->typeSceneBlendingFactorDst;
			eSBOT2 = m_pStateCommon->typeSceneBlendingOP2;
			eSrc2 = m_pStateCommon->typeSceneBlendingFactorSrc2;
			eDst2 = m_pStateCommon->typeSceneBlendingFactorDst2;
		}
		E_FORCEINLINE void	SetSceneBlendingSeperateSetting(bool bEnabled, bool bSeperateEnabled,
															FSceneBlendingOPType eSBOT, FSceneBlendingFactorType eSrc, FSceneBlendingFactorType eDst,
															FSceneBlendingOPType eSBOT2, FSceneBlendingFactorType eSrc2, FSceneBlendingFactorType eDst2)
		{
			m_pStateCommon->bSceneBlendingEnabled = bEnabled;
			m_pStateCommon->bSceneBlendingSeperateEnabled = bSeperateEnabled;
			m_pStateCommon->typeSceneBlendingOP = eSBOT; 
			m_pStateCommon->typeSceneBlendingFactorSrc = eSrc;
			m_pStateCommon->typeSceneBlendingFactorDst = eDst;
			m_pStateCommon->typeSceneBlendingOP2 = eSBOT2; 
			m_pStateCommon->typeSceneBlendingFactorSrc2 = eSrc2;
			m_pStateCommon->typeSceneBlendingFactorDst2 = eDst2;
		}

		
		E_FORCEINLINE bool	GetColorRWriteEnabled() const { return m_pStateCommon->bColorRWriteEnabled; }
		E_FORCEINLINE void	SetColorRWriteEnabled(const bool& bEnabled) { m_pStateCommon->bColorRWriteEnabled = bEnabled; }
		E_FORCEINLINE bool	GetColorGWriteEnabled() const { return m_pStateCommon->bColorGWriteEnabled; }
		E_FORCEINLINE void	SetColorGWriteEnabled(const bool& bEnabled) { m_pStateCommon->bColorGWriteEnabled = bEnabled; }
		E_FORCEINLINE bool	GetColorBWriteEnabled() const { return m_pStateCommon->bColorBWriteEnabled; }
		E_FORCEINLINE void	SetColorBWriteEnabled(const bool& bEnabled) { m_pStateCommon->bColorBWriteEnabled = bEnabled; }
		E_FORCEINLINE bool	GetColorAWriteEnabled() const { return m_pStateCommon->bColorAWriteEnabled; }
		E_FORCEINLINE void	SetColorAWriteEnabled(const bool& bEnabled) { m_pStateCommon->bColorAWriteEnabled = bEnabled; }

		E_FORCEINLINE void	GetColorWriteEnabled(bool& r, bool& g, bool& b, bool& a) const					
		{ 
			r = m_pStateCommon->bColorRWriteEnabled;
			g = m_pStateCommon->bColorGWriteEnabled;
			b = m_pStateCommon->bColorBWriteEnabled;
			a = m_pStateCommon->bColorAWriteEnabled;
		}
		E_FORCEINLINE void	SetColorWriteEnabled(bool r, bool g, bool b, bool a)  	
		{
			m_pStateCommon->bColorRWriteEnabled = r;
			m_pStateCommon->bColorGWriteEnabled = g;
			m_pStateCommon->bColorBWriteEnabled = b;
			m_pStateCommon->bColorAWriteEnabled = a;
		}

	////RenderStateLighting
		E_FORCEINLINE FLightingType GetLightingType() const { return m_pStateLighting->typeLighting; }
		E_FORCEINLINE void	SetLightingType(const FLightingType& t) { m_pStateLighting->typeLighting = t; }

		E_FORCEINLINE bool	GetLightingEnabled() const { return m_pStateLighting->bLightingEnabled; }
		E_FORCEINLINE void	SetLightingEnabled(bool b) { m_pStateLighting->bLightingEnabled = b; }
		E_FORCEINLINE uint8 GetMaxEffectLightNum() const { return m_pStateLighting->nMaxEffectLightNum; }
		E_FORCEINLINE void	SetMaxEffectLightNum(uint8 nNum) { m_pStateLighting->nMaxEffectLightNum = nNum; }
		
		E_FORCEINLINE const FColor& GetAmbient() const { return m_pStateLighting->clAmbient; }
		E_FORCEINLINE void	SetAmbient(const FColor& cl) { m_pStateLighting->clAmbient = cl; }
		E_FORCEINLINE const FColor& GetDiffuse() const { return m_pStateLighting->clDiffuse; }
		E_FORCEINLINE void	SetDiffuse(const FColor& cl)	{ m_pStateLighting->clDiffuse = cl; }
		E_FORCEINLINE const FColor& GetSpecular() const	{ return m_pStateLighting->clSpecular; }
		E_FORCEINLINE void	SetSpecular(const FColor& cl) { m_pStateLighting->clSpecular = cl; }
		E_FORCEINLINE const FColor& GetEmissive() const { return m_pStateLighting->clEmissive; }
		E_FORCEINLINE void	SetEmissive(const FColor& cl) { m_pStateLighting->clEmissive = cl; }
		E_FORCEINLINE float GetShininess() const { return m_pStateLighting->fShininess; }
		E_FORCEINLINE void	SetShininess(float f) { m_pStateLighting->fShininess = f; }
		E_FORCEINLINE uint32 GetColorFromVertexFlag() const { return m_pStateLighting->nColorFromVertexFlag; }
		E_FORCEINLINE void	SetColorFromVertexFlag(uint32 n) { m_pStateLighting->nColorFromVertexFlag = n; }


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
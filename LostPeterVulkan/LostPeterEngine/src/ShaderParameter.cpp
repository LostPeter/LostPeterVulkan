/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ShaderParameter.h"
#include "../include/ShaderParamSourceAuto.h"
#include "../include/ShaderProgramGroup.h"
#include "../include/ObjectLight.h"

namespace LostPeterEngine
{
    ////////////////////////////// ShaderConstantRegister //////////////////////////////
    ShaderConstantRegister::ShaderConstantRegister()
    {
        memset(m_fBuf, 0, sizeof(float) * 4);
    }


    ////////////////////////////// ShaderConstantEntryAuto /////////////////////////////
    ShaderConstantEntryAuto::ShaderConstantEntryAuto(FShaderParamConstantAutoType eShaderParamConstantAuto, 
                                                     uint32 nIndex, 
                                                     uint32 nData,
                                                     uint32 nElementCount,
                                                     bool bIsArray /*= false*/)
			: m_eShaderParamConstantAuto(eShaderParamConstantAuto)
			, m_nElementCount(nElementCount)
			, m_nRegisterIndex(nIndex)
			, m_nData(nData) 
		{
			m_nRegisterCount = ((m_nElementCount+3)>>2);
			if (bIsArray)
			{
				m_nRegisterCount *= m_nData;
			}
		}

	 ShaderConstantEntryAuto::ShaderConstantEntryAuto(FShaderParamConstantAutoType eShaderParamConstantAuto, 
                                                      uint32 nIndex,
                                                      float fData,
                                                      uint32 nElementCount)
			: m_eShaderParamConstantAuto(eShaderParamConstantAuto)
			, m_nElementCount(nElementCount)
			, m_nRegisterIndex(nIndex)
			, m_fData(fData)
		{
			m_nRegisterCount = ((m_nElementCount + 3) >> 2);
		}

    bool ShaderConstantEntryAuto::operator ==(const ShaderConstantEntryAuto& src)
    {
        return (m_eShaderParamConstantAuto == src.m_eShaderParamConstantAuto &&
            m_nElementCount == src.m_nElementCount &&
            m_nRegisterCount == src.m_nRegisterCount);
    }


    ////////////////////////////// ShaderConstantEntryManual ///////////////////////////
    ShaderConstantEntryManual::ShaderConstantEntryManual(const String& strName, uint32 nIndex, const float* pData, uint32 nDataCount)
        : Base(strName)
        , m_nRegisterIndex(nIndex)
        , m_pRegisterData(nullptr)
    {
        m_nRegisterCount = ((nDataCount+3)>>2);
        if (pData && nDataCount > 0)
        {
            m_pRegisterData = new ShaderConstantRegister[m_nRegisterCount];
            memcpy(m_pRegisterData, pData, nDataCount * sizeof(float));
        }
    }

	ShaderConstantEntryManual::ShaderConstantEntryManual(const ShaderConstantEntryManual& src)
        : Base(src.m_strName)
        , m_nRegisterIndex(src.m_nRegisterIndex)
        , m_nRegisterCount(src.m_nRegisterCount)
    {
        m_pRegisterData = new ShaderConstantRegister[m_nRegisterCount];
        memcpy(m_pRegisterData, src.m_pRegisterData, m_nRegisterCount * sizeof(ShaderConstantRegister));
    }

	ShaderConstantEntryManual::~ShaderConstantEntryManual()
    {
        F_DELETE_T(m_pRegisterData)
    }

    void ShaderConstantEntryManual::SetManualConstantEntryData(const float* pData, uint32 nDataCount)
    {
        uint32 nRegisterCount = ((nDataCount + 3) >> 2);
        if (nRegisterCount > 0 && nRegisterCount <= m_nRegisterCount)
        {
            memcpy(m_pRegisterData, pData, nRegisterCount * sizeof(float));
        }
    }

	
    ////////////////////////////// ShaderParameter /////////////////////////////////////
    Name2ShaderConstantAutoDefinitionPtrMap	ShaderParameter::m_mapName2ShaderConstantAutoDefinition;
	ShaderConstantAutoDefinition ShaderParameter::ms_aShaderConstantAutoDefinitions[] = 
	{
        //FShaderParamConstantAutoType                                                                      //FShaderParamConstantDataType          //ElementCount      //bIsArray
	////World
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldMatrix,								    F_ShaderParamConstantData_Float,		16,		            false),			//0
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldMatrixTranspose,					    F_ShaderParamConstantData_Float,		16,		            false),			//1
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldMatrixInverse,						    F_ShaderParamConstantData_Float,		16,		            false),			//2
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldMatrixInverseTranspose,				    F_ShaderParamConstantData_Float,		16,		            false),			//3
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldMatrixArray4x4,						    F_ShaderParamConstantData_Float,		16,		            true),			//4
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldMatrixArray3x4,						    F_ShaderParamConstantData_Float,		12,		            true),			//5
	////View	
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewMatrix,								    F_ShaderParamConstantData_Float,		16,		            false),			//6
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewMatrixTranspose,						    F_ShaderParamConstantData_Float,		16,		            false),			//7
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewMatrixInverse,						    F_ShaderParamConstantData_Float,		16,		            false),			//8
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewMatrixInverseTranspose,				    F_ShaderParamConstantData_Float,		16,		            false),			//9
	////Projection
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ProjectionMatrix,							F_ShaderParamConstantData_Float,		16,		            false),			//10
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ProjectionMatrixTranspose,				    F_ShaderParamConstantData_Float,		16,		            false),			//11
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ProjectionMatrixInverse,					    F_ShaderParamConstantData_Float,		16,		            false),			//12
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ProjectionMatrixInverseTranspose,			F_ShaderParamConstantData_Float,		16,		            false),			//13
	////World * View																																							
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldViewMatrix,							    F_ShaderParamConstantData_Float,		16,		            false),			//14
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldViewMatrixTranspose,					F_ShaderParamConstantData_Float,		16,		            false),			//15
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldViewMatrixInverse,					    F_ShaderParamConstantData_Float,		16,		            false),			//16
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldViewMatrixInverseTranspose,			    F_ShaderParamConstantData_Float,		16,		            false),			//17
	////View * Projection
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewProjectionMatrix,						F_ShaderParamConstantData_Float,		16,		            false),			//18
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewProjectionMatrixTranspose,			    F_ShaderParamConstantData_Float,		16,		            false),			//19
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewProjectionMatrixInverse,				    F_ShaderParamConstantData_Float,		16,		            false),			//20
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewProjectionMatrixInverseTranspose,		F_ShaderParamConstantData_Float,		16,		            false),			//21
	////World * View * Projection																																				
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldViewProjectionMatrix,				    F_ShaderParamConstantData_Float,		16,		            false),			//22
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldViewProjectionMatrixTranspose,		    F_ShaderParamConstantData_Float,		16,		            false),			//23
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldViewProjectionMatrixInverse,			F_ShaderParamConstantData_Float,		16,		            false),			//24
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_WorldViewProjectionMatrixInverseTranspose,   F_ShaderParamConstantData_Float,		16,		            false),			//25
	////Viewport
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewportSize,								F_ShaderParamConstantData_Float,		4,		            false),			//26
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewportWidth,							    F_ShaderParamConstantData_Float,		1,		            false),			//27
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewportWidthInverse,						F_ShaderParamConstantData_Float,		1,		            false),			//28
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewportHeight,							    F_ShaderParamConstantData_Float,		1,		            false),			//29
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_ViewportHeightInverse,					    F_ShaderParamConstantData_Float,		1,		            false),			//30
	////Camera																																									
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraPosInObjectSpace,					    F_ShaderParamConstantData_Float,		4,		            false),			//31
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraRightInObjectSpace,					F_ShaderParamConstantData_Float,		4,		            false),			//32
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraUpInObjectSpace,					    F_ShaderParamConstantData_Float,		4,		            false),			//33
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraDirInObjectSpace,					    F_ShaderParamConstantData_Float,		4,		            false),			//34
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraPosInWorldSpace,					    F_ShaderParamConstantData_Float,		4,		            false),			//35
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraRightInWorldSpace,					    F_ShaderParamConstantData_Float,		4,		            false),			//36
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraUpInWorldSpace,						F_ShaderParamConstantData_Float,		4,		            false),			//37
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraDirInWorldSpace,					    F_ShaderParamConstantData_Float,		4,		            false),			//38
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraFov,								    F_ShaderParamConstantData_Float,	    1,		            false),			//39
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraNearClipDistance,					    F_ShaderParamConstantData_Float,		1,		            false),			//40
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraFarClipDistance,					    F_ShaderParamConstantData_Float,		1,		            false),			//41
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_CameraParam,								    F_ShaderParamConstantData_Float,		4,		            false),			//42
	////Light
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightPosInModelSpace,						F_ShaderParamConstantData_Float,		4,		            false),			//43
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightPosInWorldSpace,						F_ShaderParamConstantData_Float,		4,		            false),			//44
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightPosInViewSpace,						    F_ShaderParamConstantData_Float,		4,		            false),			//45
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDirInModelSpace,						F_ShaderParamConstantData_Float,		4,		            false),			//46
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDirInWorldSpace,						F_ShaderParamConstantData_Float,		4,		            false),			//47
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDirInViewSpace,						    F_ShaderParamConstantData_Float,		4,		            false),			//48
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDistanceModelSpace,					    F_ShaderParamConstantData_Float,		1,		            false),			//49
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightPowerScale,							    F_ShaderParamConstantData_Float,		1,		            false),			//50
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDiffuseColor,						    F_ShaderParamConstantData_Float,		4,		            false),			//51
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightSpecularColor,						    F_ShaderParamConstantData_Float,		4,		            false),			//52
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightAttenuation,							F_ShaderParamConstantData_Float,		4,		            false),			//53
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightSpotParam,							    F_ShaderParamConstantData_Float,		4,		            false),			//54
		
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightPosInModelSpaceArray,				    F_ShaderParamConstantData_Float,		4,		            false),			//55
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightPosInWorldSpaceArray,				    F_ShaderParamConstantData_Float,		4,		            false),			//56
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightPosInViewSpaceArray,					F_ShaderParamConstantData_Float,		4,		            false),			//57
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDirInModelSpaceArray,				    F_ShaderParamConstantData_Float,		4,		            false),			//58
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDirInWorldSpaceArray,				    F_ShaderParamConstantData_Float,		4,		            false),			//59
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDirInViewSpaceArray,					F_ShaderParamConstantData_Float,		4,		            false),			//60
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDistanceModelSpaceArray,				F_ShaderParamConstantData_Float,		1,		            false),			//61
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightPowerScaleArray,						F_ShaderParamConstantData_Float,		1,		            false),			//62
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightDiffuseColorArray,					    F_ShaderParamConstantData_Float,		4,		            false),			//63
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightSpecularColorArray,					    F_ShaderParamConstantData_Float,		4,		            false),			//64
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightAttenuationArray,					    F_ShaderParamConstantData_Float,		4,		            false),			//65
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_LightSpotParamArray,						    F_ShaderParamConstantData_Float,		4,		            false),			//66
	////Material																																								
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_GlobalAmbient,							    F_ShaderParamConstantData_Float,		4,		            false),			//67
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialAmbientColor,						F_ShaderParamConstantData_Float,		4,		            false),			//68
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialDiffuseColor,						F_ShaderParamConstantData_Float,		4,		            false),			//69
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialSpecularColor,					    F_ShaderParamConstantData_Float,		4,		            false),			//70
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialEmissiveColor,					    F_ShaderParamConstantData_Float,		4,		            false),			//71
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialShininess,						    F_ShaderParamConstantData_Float,		1,		            false),			//72
	
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialLightAmbientColor,				    F_ShaderParamConstantData_Float,		4,		            false),			//73
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialLightDiffuseColor,				    F_ShaderParamConstantData_Float,		4,		            false),			//74
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialLightSpecularColor,				    F_ShaderParamConstantData_Float,		4,		            false),			//75
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialLightSceneColor,					    F_ShaderParamConstantData_Float,		4,		            false),			//76
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialLightDiffuseColorArray,			    F_ShaderParamConstantData_Float,		4,		            false),			//77
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_MaterialLightSpecularColorArray,			    F_ShaderParamConstantData_Float,		1,		            false),			//78
	////Fog
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_FogColor,									F_ShaderParamConstantData_Float,		4,		            false),			//79
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_FogParam,									F_ShaderParamConstantData_Float,		4,		            false),			//80
	////Texture																																									  
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TextureSizeWHD,							    F_ShaderParamConstantData_Float,		4,		            false),			//81
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TextureSizeWHDInverse,					    F_ShaderParamConstantData_Float,		4,		            false),			//82
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TextureSizeWH,							    F_ShaderParamConstantData_Float,		4,		            false),			//83
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TextureUVTranslation,						F_ShaderParamConstantData_Float,		4,		            false),			//84
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TextureUVRotation,						    F_ShaderParamConstantData_Float,		4,		            false),			//85
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TextureUVScale,							    F_ShaderParamConstantData_Float,		4,		            false),			//86
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TextureUVTransformMatrix,					F_ShaderParamConstantData_Float,		16,		            false),			//87
	////Time																																									  
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed,								    F_ShaderParamConstantData_Float,		1,		            false),			//88
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_0_X,							    F_ShaderParamConstantData_Float,		1,		            false),			//89
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Sin_0_X,						    F_ShaderParamConstantData_Float,		1,		            false),			//90
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Cos_0_X,						    F_ShaderParamConstantData_Float,		1,		            false),			//91
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Tan_0_X,						    F_ShaderParamConstantData_Float,		1,		            false),			//92
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Packed_0_X,					    F_ShaderParamConstantData_Float,		4,		            false),			//93
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_0_1,							    F_ShaderParamConstantData_Float,		1,		            false),			//94
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Sin_0_1,						    F_ShaderParamConstantData_Float,		1,		            false),			//95
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Cos_0_1,						    F_ShaderParamConstantData_Float,		1,		            false),			//96
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Tan_0_1,						    F_ShaderParamConstantData_Float,		1,		            false),			//97
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Packed_0_1,					    F_ShaderParamConstantData_Float,		4,		            false),			//98
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_0_2PI,						    F_ShaderParamConstantData_Float,		1,		            false),			//99
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Sin_0_2PI,					    F_ShaderParamConstantData_Float,		1,		            false),			//100
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Cos_0_2PI,					    F_ShaderParamConstantData_Float,		1,		            false),			//101
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Tan_0_2PI,					    F_ShaderParamConstantData_Float,		1,		            false),			//102
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_TimeElapsed_Packed_0_2PI,				    F_ShaderParamConstantData_Float,		4,		            false),			//103
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_FrameTime,								    F_ShaderParamConstantData_Float,		1,		            false),			//104
		ShaderConstantAutoDefinition(F_ShaderParamConstantAuto_FPS,										    F_ShaderParamConstantData_Float,		1,		            false)			//105
	};

    void ShaderParameter::initParamStaticMap()
	{
		if (!m_mapName2ShaderConstantAutoDefinition.empty())
			return;

		uint32 nCount = GetShaderConstantAutoDefinitionCount();
		for (uint32 i = 0; i < nCount; i++)
		{
			ShaderConstantAutoDefinition* pDefinition = &ms_aShaderConstantAutoDefinitions[i];
			F_Assert(i == static_cast<uint32>(pDefinition->m_eShaderParamConstantAuto) && "ShaderParameter::initParamStaticMap")
			m_mapName2ShaderConstantAutoDefinition[F_GetShaderParamConstantAutoTypeName(pDefinition->m_eShaderParamConstantAuto)] = pDefinition;
		}
	}
	uint32 ShaderParameter::GetShaderConstantAutoDefinitionCount()
	{
		return sizeof(ms_aShaderConstantAutoDefinitions) / sizeof(ShaderConstantAutoDefinition);
	}
	const ShaderConstantAutoDefinition* ShaderParameter::GetConstantAutoDefinition(const String& strName)
	{
		F_Assert(!m_mapName2ShaderConstantAutoDefinition.empty() && "ShaderParameter::GetConstantAutoDefinition");
		Name2ShaderConstantAutoDefinitionPtrMap::iterator itFind = m_mapName2ShaderConstantAutoDefinition.find(strName);
		if (itFind != m_mapName2ShaderConstantAutoDefinition.end())
			return itFind->second;
		
		F_Assert(false && "ShaderParameter::GetConstantAutoDefinition: Can not find ConstantAutoDefinition !")
		return nullptr;
	}
	const ShaderConstantAutoDefinition* ShaderParameter::GetConstantAutoDefinition(uint32 nIndex)
	{
		if (nIndex < GetShaderConstantAutoDefinitionCount())
		{
			F_Assert(nIndex == static_cast<uint32>(ms_aShaderConstantAutoDefinitions[nIndex].m_eShaderParamConstantAuto) && "ShaderParameter::GetConstantAutoDefinition")
			return &ms_aShaderConstantAutoDefinitions[nIndex];
		}
		return nullptr;
	}
	uint32 ShaderParameter::GetDefaultRangeIndexStart()
	{
		return 0;
	}
	uint32 ShaderParameter::GetDefaultRangeIndexEnd()
	{
		return std::numeric_limits<uint32>::max();
	}


	ShaderParameter::ShaderParameter()
		: m_nStartRangeIndex(0)
		, m_nEndRangeIndex(std::numeric_limits<uint32>::max())
		, m_bIsTransposeMatrix(false)
		, m_pShaderProgramGroup(nullptr)
	{
		initParamStaticMap();
	}

	ShaderParameter::ShaderParameter(uint32 nStartRangeIndex, uint32 nEndRangeIndex)
		: m_nStartRangeIndex(nStartRangeIndex)
		, m_nEndRangeIndex(nEndRangeIndex)
		, m_bIsTransposeMatrix(false)
		, m_pShaderProgramGroup(nullptr)
	{
		F_Assert(nEndRangeIndex > nStartRangeIndex && "ShaderParameter::ShaderParameter");

		initParamStaticMap();
	}
	
	ShaderParameter& ShaderParameter::operator= (const ShaderParameter& src)
	{
		m_aShaderConstantEntryAuto.clear();
		clearConstantManuals();
		m_nStartRangeIndex = src.m_nStartRangeIndex;
		m_nEndRangeIndex = src.m_nEndRangeIndex;
		m_aShaderConstantEntryAuto = src.m_aShaderConstantEntryAuto;

		uint32 nSize = (uint32)src.m_aShaderConstantEntryManual.size();
		for(uint32 i = 0; i < nSize; i++)
		{
			ShaderConstantEntryManual* pEntryScr = src.m_aShaderConstantEntryManual[i];
			ShaderConstantEntryManual* pDesEntry = new ShaderConstantEntryManual(pEntryScr->GetName(), 
                                                                                 pEntryScr->m_nRegisterIndex, 
                                                                                 pEntryScr->m_pRegisterData->m_fBuf, 
                                                                                 pEntryScr->m_nRegisterCount * 4);
			m_aShaderConstantEntryManual.push_back(pDesEntry);
		}
		return *this;
	}

	ShaderParameter::~ShaderParameter()	
	{
		clearConstantManuals();
	}

	void ShaderParameter::clearConstantManuals()
	{
		uint32 nSize = (uint32)m_aShaderConstantEntryManual.size();
		for (uint32 i = 0; i < nSize; i++)
		{
            F_DELETE(m_aShaderConstantEntryManual[i])
		}
		m_aShaderConstantEntryManual.clear();
	}
    
	bool ShaderParameter::IsRegisterUsed(uint32 nRegisterIndex, uint32 nRegisterCount)
	{
		if (nRegisterIndex < m_nStartRangeIndex || 
		    nRegisterIndex >= m_nEndRangeIndex || 
			(nRegisterIndex + nRegisterCount) <= m_nStartRangeIndex || 
			(nRegisterIndex + nRegisterCount) >= m_nEndRangeIndex)
		{
			return true;
		}

		bool bRet = false;
		//Auto
		uint32 nSize = (uint32)m_aShaderConstantEntryAuto.size();
		for (uint32 i = 0; i < nSize; i++)
		{
			uint32 nDesEndRegisterIndex = m_aShaderConstantEntryAuto[i]->m_nRegisterIndex + m_aShaderConstantEntryAuto[i]->m_nRegisterCount;
			uint32 nSrcEndRegisterIndex = nRegisterIndex + nRegisterCount;
			if (!(nRegisterIndex >= nDesEndRegisterIndex || 
				nSrcEndRegisterIndex < m_aShaderConstantEntryAuto[i]->m_nRegisterIndex))
			{
				bRet = true;
				break;
			}
		}

		//Manual
		if (!bRet)	
		{	
			nSize = (uint32)m_aShaderConstantEntryManual.size();
			for (uint32 i = 0;i<nSize;i++)
			{
				uint32 nDesEndRegisterIndex = m_aShaderConstantEntryManual[i]->m_nRegisterIndex + m_aShaderConstantEntryManual[i]->m_nRegisterCount;
				uint32 nSrcEndRegisterIndex = nRegisterIndex + nRegisterCount;
				if (!(nRegisterIndex >= nDesEndRegisterIndex || 
					nSrcEndRegisterIndex < m_aShaderConstantEntryManual[i]->m_nRegisterIndex))
				{
					bRet = true;
					break;
				}
			}
		}

		return bRet;
	}

	void ShaderParameter::SetRegisterRange(uint32 nStartRangeIndex, uint32 nEndRangeIndex)
	{
		m_nStartRangeIndex = nStartRangeIndex;
		m_nEndRangeIndex   = nEndRangeIndex;
    
		//Auto
		SahderConstantEntryAutoPtrVector::iterator itAuto = m_aShaderConstantEntryAuto.begin();
		while (itAuto != m_aShaderConstantEntryAuto.end())
		{
			uint32 nRegisterIndex = (*itAuto)->m_nRegisterIndex;
			uint32 nRegisterCount = (*itAuto)->m_nRegisterCount;

			if (nRegisterIndex >= m_nEndRangeIndex ||
				nRegisterIndex < m_nStartRangeIndex ||
				(nRegisterIndex + nRegisterCount) > m_nEndRangeIndex ||
				(nRegisterIndex + nRegisterCount) <= m_nStartRangeIndex)
			{
				itAuto = m_aShaderConstantEntryAuto.erase(itAuto);
			}
			else
			{
				++itAuto;
			}
		}

		//Manual
		SahderConstantEntryManualPtrVector::iterator itManual = m_aShaderConstantEntryManual.begin();
		while (itManual != m_aShaderConstantEntryManual.end())
		{
			uint32 nRegisterIndex = (*itManual)->m_nRegisterIndex;
			uint32 nRegisterCount = (*itManual)->m_nRegisterCount;

			if(nRegisterIndex >= m_nEndRangeIndex ||
				nRegisterIndex < m_nStartRangeIndex ||
				(nRegisterIndex + nRegisterCount) > m_nEndRangeIndex||
				(nRegisterIndex + nRegisterCount) <= m_nStartRangeIndex)
			{
				itManual = m_aShaderConstantEntryManual.erase(itManual);
			}
			else
			{
				++itManual;
			}
		}
	}	
	
	ShaderConstantEntryAuto* ShaderParameter::GetConstantAutoByIndex(uint32 nIndex)
	{
		for (SahderConstantEntryAutoPtrVector::iterator it = m_aShaderConstantEntryAuto.begin();
			 it != m_aShaderConstantEntryAuto.end(); ++it)
		{
			if((*it)->m_nRegisterIndex == nIndex)
			{
				return (*it);
			}
		}
		return nullptr;
	}

	const String& ShaderParameter::GetConstantManualName(uint32 nIndex)
	{
		uint32 nSize = (uint32)m_aShaderConstantEntryManual.size();
		if (nIndex >= nSize)
			return FUtilString::BLANK;
		return m_aShaderConstantEntryManual[nIndex]->GetName();
	}
	ShaderConstantEntryManual* ShaderParameter::GetConstantManualByIndex(uint32 nIndex)
	{
		ShaderConstantEntryManual* pRet = nullptr;
		uint32 nSize = (uint32)m_aShaderConstantEntryManual.size();
		for (uint32 i = 0; i < nSize; ++i)
		{
			if (m_aShaderConstantEntryManual[i]->m_nRegisterIndex == nIndex)
			{
				pRet = m_aShaderConstantEntryManual[i];
				break;
			}
		}
		return pRet;
	}
	ShaderConstantEntryManual* ShaderParameter::GetConstantManualByName(const String& strName)
	{
		ShaderConstantEntryManual* pRet = nullptr;
		uint32 nSize = (uint32)m_aShaderConstantEntryManual.size();
		for (uint32 i = 0; i < nSize; i++)
		{
			if (m_aShaderConstantEntryManual[i]->GetName() == strName) 
			{
				pRet = m_aShaderConstantEntryManual[i];
				break;
			}
		}
		return pRet;
	}
	bool ShaderParameter::SetConstantManualByName(const String& strName, const float* pData, uint32 nDataCount)
	{
		ShaderConstantEntryManual* pManualConstantEntry = GetConstantManualByName(strName);
		bool bRet = false;
		if (nullptr == pManualConstantEntry)
		{
			bRet = false;
		}
		else
		{
			if ((pManualConstantEntry->m_nRegisterCount << 2) < nDataCount)
			{
				bRet = false;
			}
			else
			{
				memcpy(pManualConstantEntry->m_pRegisterData, pData, sizeof(float) * nDataCount);
				bRet = true;
			}
		}
		return bRet;
	}
	bool ShaderParameter::SetConstantManualByIndex(uint32 nBeginIndex, const float* pData, uint32 nDataCount)
	{
		ShaderConstantEntryManual* pManualConstantEntry = GetConstantManualByIndex(nBeginIndex);
		bool bRet = false;
		if (nullptr == pManualConstantEntry)
		{
			bRet = false;
		}
		else
		{
			if ((pManualConstantEntry->m_nRegisterCount << 2) < nDataCount)
			{
				bRet = false;
			}
			else
			{
				memcpy(pManualConstantEntry->m_pRegisterData, pData, sizeof(float) * nDataCount);
				bRet = true;
			}
		}
		return bRet;
	}
	bool ShaderParameter::SetConstantManualByIndex(uint32 nBeginIndex, const FVector4& v4)
	{
		uint32 nSize = (uint32)m_aShaderConstantEntryManual.size();
		for (uint32 i = 0; i < nSize; i++)
		{
			ShaderConstantEntryManual* pEntry = m_aShaderConstantEntryManual[i];
			if (nBeginIndex == pEntry->m_nRegisterIndex)
			{
				memcpy(pEntry->m_pRegisterData->m_fBuf, &v4[0], sizeof(float) * 4);
				return true;
			}
		}
		return false;
	}
	

	bool ShaderParameter::InsertConstantAutoInt(uint32 nRegisterIndex, 
                                                  FShaderParamConstantAutoType eShaderParamConstantAuto,
                                                  uint32 nData,
                                                  uint32 nElementSize,
                                                  bool bIsArray /*= false*/)
	{
		ShaderConstantEntryAuto* pEntry = new ShaderConstantEntryAuto(eShaderParamConstantAuto, nRegisterIndex, nData, nElementSize, bIsArray);
		if (!IsRegisterUsed(pEntry->m_nRegisterIndex, pEntry->m_nRegisterCount))
		{
			m_aShaderConstantEntryAuto.push_back(pEntry);
			return true;
		}

        F_DELETE(pEntry)
		return false;
	}

	bool ShaderParameter::InsertConstantAutoFloat(uint32 nRegisterIndex,
                                                    FShaderParamConstantAutoType eShaderParamConstantAuto,
                                                    float fData,
                                                    uint32 nElementSize)
	{
		ShaderConstantEntryAuto* pEntry = new ShaderConstantEntryAuto(eShaderParamConstantAuto, nRegisterIndex, fData, nElementSize);
		if (!IsRegisterUsed(pEntry->m_nRegisterIndex, pEntry->m_nRegisterCount))
		{
			m_aShaderConstantEntryAuto.push_back(pEntry);
			return true;
		}

        F_DELETE(pEntry)
		return false;
	}

	bool ShaderParameter::InsertConstantManual(uint32 nRegisterIndex,
                                                 const String& strName,
                                                 float* pData,
                                                 uint32 nDataCount)
	{
		ShaderConstantEntryManual* pEntry = new ShaderConstantEntryManual(strName, nRegisterIndex, pData, nDataCount);
		if (!IsRegisterUsed(pEntry->m_nRegisterIndex, pEntry->m_nRegisterCount))
		{
			m_aShaderConstantEntryManual.push_back(pEntry);
			return true;
		}
    
		F_DELETE(pEntry)
		return false;
	}

	void ShaderParameter::CopyFrom(const ShaderParameter& src)
	{
		//Auto
		uint32 nSize = (uint32)src.m_aShaderConstantEntryAuto.size();
		for (uint32 i = 0; i < nSize; i++)
		{
			if (!IsRegisterUsed(src.m_aShaderConstantEntryAuto[i]->m_nRegisterIndex, src.m_aShaderConstantEntryAuto[i]->m_nRegisterCount))
			{
                ShaderConstantEntryAuto* pEntry = new ShaderConstantEntryAuto(*src.m_aShaderConstantEntryAuto[i]);
				m_aShaderConstantEntryAuto.push_back(pEntry);
			}
		}

		//Manual
		nSize = (uint32)src.m_aShaderConstantEntryManual.size();
		for (uint32 i = 0; i < nSize; i++)
		{
			ShaderConstantEntryManual* pEntryScr = src.m_aShaderConstantEntryManual[i];
			if (!IsRegisterUsed(pEntryScr->m_nRegisterIndex, pEntryScr->m_nRegisterCount))
			{
				ShaderConstantEntryManual* pEntryDest = new ShaderConstantEntryManual(pEntryScr->GetName(), pEntryScr->m_nRegisterIndex, pEntryScr->m_pRegisterData->m_fBuf, pEntryScr->m_nRegisterCount*4);
				m_aShaderConstantEntryManual.push_back(pEntryDest);
			}
		}
	}

    void ShaderParameter::UpdateParamAll(const ShaderParamSourceAuto* pParamSourceAuto)
	{
		UpdateParamAutos(pParamSourceAuto);
		UpdateParamManuals();
	}
    void ShaderParameter::UpdateParamRange(const ShaderParamSourceAuto* pParamSourceAuto, uint32 nStartRegisterIndex, uint32 nEndRegisterIndex)
	{
        
	}
	
	void ShaderParameter::UpdateParamAutos(const ShaderParamSourceAuto* pParamSourceAuto, uint32 nBeginIndex /*= 0*/, uint32 nEndIndex /*= (uint32)(-1)*/)
	{
		if (!m_pShaderProgramGroup)
			return;
		
		uint32 nIndex;
		uint32 nMatrixCount;
		const FMatrix4* pMatrix;

		uint32 nSize = (uint32)m_aShaderConstantEntryAuto.size();
		if(nEndIndex > nSize)
			nEndIndex = nSize;

		for (uint32 i = nBeginIndex; i < nEndIndex; ++i)
		{
			const ShaderConstantEntryAuto* pEntry = m_aShaderConstantEntryAuto[i];
			switch(pEntry->m_eShaderParamConstantAuto)
			{
            ////World
			case F_ShaderParamConstantAuto_WorldMatrix:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldMatrix());
				}
				break;
			case F_ShaderParamConstantAuto_WorldMatrixTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldMatrix_Transpose());
				}
				break;
			case F_ShaderParamConstantAuto_WorldMatrixInverse:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldMatrix_Inverse());
				}	
				break;
			case F_ShaderParamConstantAuto_WorldMatrixInverseTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldMatrix_InverseTranspose());
				}	
				break;
			case F_ShaderParamConstantAuto_WorldMatrixArray4x4:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldMatrixArray(), pParamSourceAuto->GetWorldMatrixCount());
				}
				break;
			case F_ShaderParamConstantAuto_WorldMatrixArray3x4:
				{
                    pMatrix	= pParamSourceAuto->GetWorldMatrixArray();
					if (pMatrix)
					{
						nMatrixCount = pParamSourceAuto->GetWorldMatrixCount();
						nIndex = pEntry->m_nRegisterIndex;
						for (uint32 i = 0; i < nMatrixCount; ++i)
						{
							m_pShaderProgramGroup->WriteConstant(nIndex, &((*pMatrix)[0][0]), 12);
							nIndex += 3;
							++pMatrix;
						}
					}
					else
					{
						const float* pFloat=pParamSourceAuto->GetWorldFloat();
						nMatrixCount = pParamSourceAuto->GetWorldMatrixCount();
						m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pFloat, nMatrixCount * 12);
					}
				}
				break;
            ////View	
			case F_ShaderParamConstantAuto_ViewMatrix:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewMatrix());
				}
				break;
			case F_ShaderParamConstantAuto_ViewMatrixTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewMatrix_Transpose());
				}	
				break;
			case F_ShaderParamConstantAuto_ViewMatrixInverse:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewMatrix_Inverse());
				}
				break;
			case F_ShaderParamConstantAuto_ViewMatrixInverseTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewMatrix_InverseTranspose());
				}
				break;
            ////Projection		
			case F_ShaderParamConstantAuto_ProjectionMatrix:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetProjectionMatrix());
				}
				break;
			case F_ShaderParamConstantAuto_ProjectionMatrixTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetProjectionMatrix_Transpose());
				}
				break;
			case F_ShaderParamConstantAuto_ProjectionMatrixInverse:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetProjectionMatrix_Inverse());
				}
				break;
			case F_ShaderParamConstantAuto_ProjectionMatrixInverseTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetProjectionMatrix_InverseTranspose());
				}
				break;
            ////World * View
			case F_ShaderParamConstantAuto_WorldViewMatrix:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldViewMatrix());
				}
				break;
			case F_ShaderParamConstantAuto_WorldViewMatrixTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldViewMatrix_Transpose());
				}
				break;
			case F_ShaderParamConstantAuto_WorldViewMatrixInverse:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldViewMatrix_Inverse());
				}
				break;
			case F_ShaderParamConstantAuto_WorldViewMatrixInverseTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldViewMatrix_InverseTranspose());
				}
				break;
            ////View * Projection	
			case F_ShaderParamConstantAuto_ViewProjectionMatrix:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewProjectionMatrix());
				}
				break;
			case F_ShaderParamConstantAuto_ViewProjectionMatrixTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewProjectionMatrix_Transpose());
				}
				break;
			case F_ShaderParamConstantAuto_ViewProjectionMatrixInverse:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewProjectionMatrix_Inverse());
				}
				break;
			case F_ShaderParamConstantAuto_ViewProjectionMatrixInverseTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewProjectionMatrix_InverseTranspose());
				}
				break;
            ////World * View * Projection		
			case F_ShaderParamConstantAuto_WorldViewProjectionMatrix:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldViewProjectionMatrix());
				}
				break;
			case F_ShaderParamConstantAuto_WorldViewProjectionMatrixTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldViewProjectionMatrix_Transpose());
				}
				break;
			case F_ShaderParamConstantAuto_WorldViewProjectionMatrixInverse:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldViewProjectionMatrix_Inverse());
				}
				break;
			case F_ShaderParamConstantAuto_WorldViewProjectionMatrixInverseTranspose:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetWorldViewProjectionMatrix_InverseTranspose());
				}
				break;
            ////Viewport	
			case F_ShaderParamConstantAuto_ViewportSize:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewportSize());
				}
				break;
			case F_ShaderParamConstantAuto_ViewportWidth:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewportWidth());
				}
				break;
			case F_ShaderParamConstantAuto_ViewportWidthInverse:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewportWidth_Inverse());
				}
				break;
			case F_ShaderParamConstantAuto_ViewportHeight:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewportHeight());
				}
				break;
			case F_ShaderParamConstantAuto_ViewportHeightInverse:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetViewportHeight_Inverse());
				}
				break;
            ////Camera		
			case F_ShaderParamConstantAuto_CameraPosInObjectSpace:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraPosInObjectSpace());
				}
				break;
			case F_ShaderParamConstantAuto_CameraRightInObjectSpace:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraRightInObjectSpace());
				}
				break;
			case F_ShaderParamConstantAuto_CameraUpInObjectSpace:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraUpInObjectSpace());
				}
				break;
			case F_ShaderParamConstantAuto_CameraDirInObjectSpace:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraDirInObjectSpace());
				}
				break;
			case F_ShaderParamConstantAuto_CameraPosInWorldSpace:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraPos());
				}			
				break;
			case F_ShaderParamConstantAuto_CameraRightInWorldSpace:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraRight());
				}
				break;
			case F_ShaderParamConstantAuto_CameraUpInWorldSpace:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraUp());
				}		
				break;
			case F_ShaderParamConstantAuto_CameraDirInWorldSpace:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraDirection());
				}
				break;
			case F_ShaderParamConstantAuto_CameraFov:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraFOV());
				}
				break;
			case F_ShaderParamConstantAuto_CameraNearClipDistance:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraNearClipDistance());
				}		
				break;
			case F_ShaderParamConstantAuto_CameraFarClipDistance:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraFarClipDistance());
				}
				break;
			case F_ShaderParamConstantAuto_CameraParam:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetCameraParam());
				}
				break;
            ////Light
			case F_ShaderParamConstantAuto_LightPosInModelSpace:
				{
					// FVector4 vPosInModel = FMath::TransformAffine(pParamSourceAuto->GetWorldMatrix_Inverse(), pParamSourceAuto->GetLight(pEntry->m_nData)->GetPosAsVector4());
					// m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, vPosInModel);
				}
				break;
			case F_ShaderParamConstantAuto_LightPosInWorldSpace:
				{
					//m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetLight(pEntry->m_nData)->GetPosAsVector4());
				}
				break;
			case F_ShaderParamConstantAuto_LightPosInViewSpace:
				{
					// FVector4 vPosInView = FMath::TransformAffine(pParamSourceAuto->GetViewMatrix(), pParamSourceAuto->GetLight(pEntry->m_nData)->GetPosAsVector4());
					// m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,vPosInView);
				}
				break;
			case F_ShaderParamConstantAuto_LightDirInModelSpace:
				{
					// FMatrix3 mat3;
					// pParamSourceAuto->GetWorldMatrix_Inverse().Extract3x3Matrix(mat3);
					// FVector3 vDir = mat3 * pParamSourceAuto->GetLight(pEntry->m_nData)->GetDerivedDir();
					// vDir.Normalize();
					// m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, FVector4(vDir.x, vDir.y, vDir.z, 1.0f));
				}
				break;
			case F_ShaderParamConstantAuto_LightDirInWorldSpace:
				{
					//m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetLight(pEntry->m_nData)->GetDirAsVector4());
				}
				break;
			case F_ShaderParamConstantAuto_LightDirInViewSpace:
				{
					// FMatrix3 mat3;
					// pParamSourceAuto->GetViewMatrix().Extract3x3Matrix(mat3);
					// FVector3 vDir = mat3 * pParamSourceAuto->GetLight(pEntry->m_nData)->GetDerivedDir();
					// vDir.Normalize();
					// m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,FVector4(vDir.x, vDir.y, vDir.z, 1.0f));
				}
				break;
			case F_ShaderParamConstantAuto_LightDistanceModelSpace:
				{
					// FVector3 vDis = FMath::TransformAffine(pParamSourceAuto->GetWorldMatrix_Inverse(), pParamSourceAuto->GetLight(pEntry->m_nData)->GetDerivedPos());
					// m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, vDis.Length());
				}
				break;
			case F_ShaderParamConstantAuto_LightPowerScale:
				{
					//m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetLight(pEntry->m_nData)->GetPowerScale());
				}		
				break;
			case F_ShaderParamConstantAuto_LightDiffuseColor:
				{
					//m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetLight(pEntry->m_nData)->GetLightDiffuse());
				}
				break;
			case F_ShaderParamConstantAuto_LightSpecularColor:
				{
					//m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pParamSourceAuto->GetLight(pEntry->m_nData)->GetLightSpecular());
				}	
				break;
			case F_ShaderParamConstantAuto_LightAttenuation:
				{
					// FVector4 vLT;
					// const ObjectLight* pLight = pParamSourceAuto->GetLight(pEntry->m_nData);
					// vLT.x = pLight->GetAttenuationRange();
					// vLT.y = pLight->GetAttenuationConst();
					// vLT.z = pLight->GetAttenuationLinear();
					// vLT.w = pLight->GetAttenuationQuad();
					// m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, vLT);
				}
				break;
			case F_ShaderParamConstantAuto_LightSpotParam:
				{
					// FVector4 vLT;
					// const ObjectLight* pLight = pParamSourceAuto->GetLight(pEntry->m_nData);
					// if (pLight->GetLightType() == Light_Spot)
					// {
					// 	vLT.x = FMath::Cos(pLight->GetSpotInner().GetRadians() * 0.5f);
					// 	vLT.y = FMath::Cos(pLight->GetSpotOuter().GetRadians() * 0.5f);
					// 	vLT.z = pLight->GetSpotFalloff();
					// 	vLT.w = 1.0f;
					// }
					// else
					// {
					// 	vLT.x = 1.0f;
					// 	vLT.y = 0.0f;
					// 	vLT.z = 0.0f; 
					// 	vLT.w = 1.0f;
					// }
					// m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, vLT);
				}
				break;
			case F_ShaderParamConstantAuto_LightPosInModelSpaceArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	FVector4 vPosInModel = FMath::TransformAffine(pParamSourceAuto->GetWorldMatrix_Inverse(), pParamSourceAuto->GetLight(i)->GetPosAsVector4());
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, vPosInModel);
					// }
				}
			    break;
			case F_ShaderParamConstantAuto_LightPosInWorldSpaceArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, pParamSourceAuto->GetLight(i)->GetPosAsVector4());
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightPosInViewSpaceArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	FVector4 vPosInView = FMath::TransformAffine(pParamSourceAuto->GetViewMatrix(), pParamSourceAuto->GetLight(i)->GetPosAsVector4());
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, vPosInView);
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightDirInModelSpaceArray:
				{
					// FMatrix3 mat3;
					// pParamSourceAuto->GetWorldMatrix_Inverse().Extract3x3Matrix(mat3);
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	FVector3 vDir = mat3 * pParamSourceAuto->GetLight(i)->GetDerivedDir();
					// 	vDir.Normalize();
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, FVector4(vDir.x, vDir.y, vDir.z, 1.0f)); 
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightDirInWorldSpaceArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex+i,pParamSourceAuto->GetLight(i)->GetDirAsVector4());
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightDirInViewSpaceArray:
				{
					// FMatrix3 mat3;
					// pParamSourceAuto->GetViewMatrix().Extract3x3Matrix(mat3);
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	FVector3 vDir = mat3 * pParamSourceAuto->GetLight(i)->GetDerivedDir();
					// 	vDir.Normalize();
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, FVector4(vDir.x, vDir.y, vDir.z, 1.0f));
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightDistanceModelSpaceArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	FVector3 vDis = FMath::TransformAffine(pParamSourceAuto->GetWorldMatrix_Inverse(), pParamSourceAuto->GetLight(i)->GetDerivedPos());
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, vDis.Length());
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightPowerScaleArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, pParamSourceAuto->GetLight(i)->GetPowerScale());
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightDiffuseColorArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, pParamSourceAuto->GetLight(i)->GetLightDiffuse());
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightSpecularColorArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, pParamSourceAuto->GetLight(i)->GetLightSpecular());
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightAttenuationArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	FVector4 vLT;
					// 	const ObjectLight* pLight = pParamSourceAuto->GetLight(i);
					// 	vLT.x = pLight->GetAttenuationRange();
					// 	vLT.y = pLight->GetAttenuationConst();
					// 	vLT.z = pLight->GetAttenuationLinear();
					// 	vLT.w = pLight->GetAttenuationQuad();
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, vLT);
					// }
				}
				break;
			case F_ShaderParamConstantAuto_LightSpotParamArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	FVector4 vLT;
					// 	const ObjectLight* pLight = pParamSourceAuto->GetLight(i);
					// 	if (pLight->GetLightType() == Light_Spot)
					// 	{
					// 		vLT.x = FMath::Cos(pLight->GetSpotInner().GetRadians() * 0.5f);
					// 		vLT.y = FMath::Cos(pLight->GetSpotOuter().GetRadians() * 0.5f);
					// 		vLT.z = pLight->GetSpotFalloff();
					// 		vLT.w = 1.0f;
					// 	}
					// 	else
					// 	{
					// 		vLT.x = 1.0f;
					// 		vLT.y = 0.0f;
					// 		vLT.z = 0.0f; 
					// 		vLT.w = 1.0f;
					// 	}
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex+i,vLT);
					// }
				}
				break;
            ////Material	
			case F_ShaderParamConstantAuto_GlobalAmbient:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetGlobalAmbient((uint8)pEntry->m_nData));
				}
				break;
			case F_ShaderParamConstantAuto_MaterialAmbientColor:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetAmbient());
				}
				break;
			case F_ShaderParamConstantAuto_MaterialDiffuseColor:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetDiffuse());
				}		
				break;
			case F_ShaderParamConstantAuto_MaterialSpecularColor:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetSpecular());
				}
				break;
			case F_ShaderParamConstantAuto_MaterialEmissiveColor:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetEmissive());
				}
				break;
			case F_ShaderParamConstantAuto_MaterialShininess:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetShininess());
				}
				break;
			case F_ShaderParamConstantAuto_MaterialLightAmbientColor:
				{
					FColor clResult = pParamSourceAuto->GetGlobalAmbient((uint8)pEntry->m_nData) * pParamSourceAuto->GetAmbient();
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, clResult);
				}
				break;
			case F_ShaderParamConstantAuto_MaterialLightDiffuseColor:
				{
					// FColor clResult = pParamSourceAuto->GetLight(pEntry->m_nData)->GetLightDiffuse() * pParamSourceAuto->GetDiffuse();
					// m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, clResult);
				}
				break;
			case F_ShaderParamConstantAuto_MaterialLightSpecularColor:
				{
					// FColor clResult = pParamSourceAuto->GetLight(pEntry->m_nData)->GetLightSpecular() * pParamSourceAuto->GetSpecular();
					// m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, clResult);
				}
				break;
			case F_ShaderParamConstantAuto_MaterialLightSceneColor:
				{
					FColor clResult = pParamSourceAuto->GetGlobalAmbient((uint8)pEntry->m_nData) * pParamSourceAuto->GetAmbient() + pParamSourceAuto->GetEmissive();
					clResult.a = pParamSourceAuto->GetDiffuse().a;
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, clResult);
				}
				break;
			case F_ShaderParamConstantAuto_MaterialLightDiffuseColorArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	FColor clResult = pParamSourceAuto->GetLight(i)->GetLightDiffuse() * pParamSourceAuto->GetDiffuse();
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex + i, clResult);
					// }
				}
				break;
			case F_ShaderParamConstantAuto_MaterialLightSpecularColorArray:
				{
					// for (uint32 i = 0; i < pEntry->m_nData; ++i)
					// {
					// 	FColor clResult = pParamSourceAuto->GetLight(i)->GetLightSpecular() * pParamSourceAuto->GetSpecular();
					// 	m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex+i,clResult);
					// }
				}
				break;
            ////Fog		
			case F_ShaderParamConstantAuto_FogColor:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetViewportHeight_Inverse());
				}
				break;
			case F_ShaderParamConstantAuto_FogParam:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetViewportHeight_Inverse());
				}
				break;
            ////Texture	
			case F_ShaderParamConstantAuto_TextureSizeWHD:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTextureSize(pEntry->m_nData));
				}
				break;
			case F_ShaderParamConstantAuto_TextureSizeWHDInverse:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTextureSize_Inverse(pEntry->m_nData));
				}
				break;
			case F_ShaderParamConstantAuto_TextureSizeWH:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTextureSize_Packed(pEntry->m_nData));
				}
				break;
			case F_ShaderParamConstantAuto_TextureUVTranslation:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTextureUVTranslate(pEntry->m_nData));
				}
				break;
			case F_ShaderParamConstantAuto_TextureUVRotation:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTextureUVRotate(pEntry->m_nData));
				}	
				break;
			case F_ShaderParamConstantAuto_TextureUVScale:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTextureUVScale(pEntry->m_nData));
				}	
				break;
			case F_ShaderParamConstantAuto_TextureUVTransformMatrix:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTextureUVTransformMatrix(pEntry->m_nData));
				}
				break;
            ////Time		
			case F_ShaderParamConstantAuto_TimeElapsed:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed() * pEntry->m_fData);
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_0_X:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_0_X(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Sin_0_X:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Sin_0_X(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Cos_0_X:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Cos_0_X(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Tan_0_X:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Tan_0_X(pEntry->m_fData));	
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Packed_0_X:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Packed_0_X(pEntry->m_fData));
				}
				break;	
			case F_ShaderParamConstantAuto_TimeElapsed_0_1:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_0_1(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Sin_0_1:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Sin_0_1(pEntry->m_fData));
				}		
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Cos_0_1:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Cos_0_1(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Tan_0_1:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Tan_0_1(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Packed_0_1:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Packed_0_1(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_0_2PI:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_0_2PI(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Sin_0_2PI:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Sin_0_2PI(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Cos_0_2PI:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Cos_0_2PI(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Tan_0_2PI:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Tan_0_2PI(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_TimeElapsed_Packed_0_2PI:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetTimeElapsed_Packed_0_2PI(pEntry->m_fData));
				}
				break;
			case F_ShaderParamConstantAuto_FrameTime:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetFrameTime() * pEntry->m_fData);
				}
				break;
			case F_ShaderParamConstantAuto_FPS:
				{
					m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex,pParamSourceAuto->GetFPS());
				}
				break;
			default:
				{
					F_Assert(false && "ShaderParameter::UpdateParamAutos: Wrong shader param type!");
				}
			}
		}
	}

	void ShaderParameter::UpdateParamManuals(uint32 nBeginIndex /*= 0*/, uint32 nEndIndex /*= (uint32)(-1)*/)
	{
		uint32 nSize = (uint32)m_aShaderConstantEntryManual.size();
		if(nEndIndex > nSize)
		{
			nEndIndex = nSize;
		}
		for (uint32 i = nBeginIndex; i < nEndIndex; i++)
		{
			ShaderConstantEntryManual* pEntry = m_aShaderConstantEntryManual[i];
			m_pShaderProgramGroup->WriteConstant(pEntry->m_nRegisterIndex, pEntry->m_pRegisterData->m_fBuf, (pEntry->m_nRegisterCount << 2));
		}
	}

	uint32 ShaderParameter::CalculateSize() const
	{
		
		return 0;
	}

}; //LostPeterEngine
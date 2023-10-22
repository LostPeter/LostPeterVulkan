/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MaterialDataSerializer.h"
#include "../include/VulkanWindow.h"
#include "../include/MaterialDataManager.h"
#include "../include/MaterialData.h"
#include "../include/MaterialManager.h"
#include "../include/Material.h"
#include "../include/RenderState.h"
#include "../include/Mesh.h"
#include "../include/Object.h"

namespace LostPeter
{
#define MATERIAL_DATA_TAG_MATERIAL_CFG								            "cfg_material_data"
    #define	MATERIAL_DATA_TAG_MATERIALS									            "material_datas"
        #define	MATERIAL_DATA_TAG_MATERIAL									            "material_data"
            #define	MATERIAL_DATA_TAG_PASS                                                  "pass"
                #define	MATERIAL_DATA_TAG_STATE_COMMON							                "state_common"
                    #define	MATERIAL_DATA_TAG_STATE_COMMON_POLYGON_TYPE				                "polygon_type"					//1
                    #define	MATERIAL_DATA_TAG_STATE_COMMON_CULLING_TYPE				                "culling_type"					//2
                    #define	MATERIAL_DATA_TAG_STATE_COMMON_POINT_SETTING				            "point_setting"					//3
                    #define	MATERIAL_DATA_TAG_STATE_COMMON_DEPTH_SETTING				            "depth_setting"					//4
                    #define	MATERIAL_DATA_TAG_STATE_COMMON_STENCIL_SETTING			                "stencil_setting"				//5
                    #define	MATERIAL_DATA_TAG_STATE_COMMON_SCISSOR_TEST			                    "scissor_test"					//6
                    #define	MATERIAL_DATA_TAG_STATE_COMMON_ALPHA_TEST			                    "alpha_test"					//7
                    #define	MATERIAL_DATA_TAG_STATE_COMMON_SCENE_BLENDING_SETTING	                "scene_blending_setting"		//8
                    #define	MATERIAL_DATA_TAG_STATE_COMMON_COLOR_WRITE				                "color_write"					//9
                    
                #define	MATERIAL_DATA_TAG_STATE_LIGHTING							            "state_lighting"
                    #define	MATERIAL_DATA_TAG_STATE_LIGHTING_LIGHTING_TYPE			                "lighting_type"					//1
                    #define MATERIAL_DATA_TAG_STATE_LIGHTING_MATERIAL_SETTING		                "material_setting"				//2

                #define	MATERIAL_DATA_TAG_STATE_SHADER							                "state_shader"
                    #define MATERIAL_DATA_TAG_STATE_SHADER_VERT							            "vert"						    //1
                    #define MATERIAL_DATA_TAG_STATE_SHADER_TESC						                "tesc"						    //2
                    #define MATERIAL_DATA_TAG_STATE_SHADER_TESE						                "tese"						    //3
                    #define MATERIAL_DATA_TAG_STATE_SHADER_GROM						                "geom"						    //4
                    #define MATERIAL_DATA_TAG_STATE_SHADER_FRAG						                "frag"						    //5
                    #define MATERIAL_DATA_TAG_STATE_SHADER_COMP						                "comp"						    //6

                    #define	MATERIAL_DATA_TAG_STATE_TEXTURE							                "state_texture"					
                        #define MATERIAL_DATA_TAG_STATE_TEXTURE_TEXTURE_UNIT						    "tex_unit"
                            #define MATERIAL_DATA_TAG_TAT_TEXTURE_TEXTURE_SETTING						    "tex_setting"		    //1
                            #define MATERIAL_DATA_TAG_TAT_TEXTURE_ANIMATION_SETTING					        "anim_setting"			//2


#define MATERIAL_DATA_TAG_ATTRIBUTE_GROUP			            "group"
#define MATERIAL_DATA_TAG_ATTRIBUTE_NAME		                "name"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_TYPE				        "type"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_SOURCE			            "src"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_DST				            "dst"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_SOURCE2			            "src2"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_DST2				        "dst2"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE			            "enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC			    "compare_func"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_VALUE			            "value"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_MASK				        "mask"
#define MATERIAL_DATA_TAG_ATTRIBUTE_FLAG				        "flag"

    //State Common
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_POINT_SIZE				        "size"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_POINT_SPRITE_ENABLE	            "sprite_enable"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_POINT_ATTENU_ENABLE	            "attenu_enable"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_POINT_CONSTANT			        "constant"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_POINT_LINEAR			        "linear"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_POINT_QUADRATIC		            "quadratic"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_POINT_MIN_SIZE			        "min_size"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_POINT_MAX_SIZE			        "max_size"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_TEST_ENABLE		        "test_enable"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_WRITE_ENABLE		        "write_enable"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_BIAS_CONSTANT	            "bias_constant"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_BIAS_SLOPE_SCALE	        "bias_slope_scale"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_FAIL_OP			        "fail_op"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_DEPTH_FAIL_OP	        "depth_fail_op"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_PASS_OP			        "pass_op"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_TWO_SIDED_ENABLE	    "two_sided_enable"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_LEFT               "left"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_TOP                "top"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_RIGHT              "right"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_BOTTOM             "bottom"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_TYPE			    "scene_blending_type"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_OP_TYPE		    "scene_blending_op_type"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_SEPERATE_ENABLE  "seperate_enable"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_OP2_TYPE		    "scene_blending_op2_type"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_R_ENABLE	                "color_r_enable"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_G_ENABLE	                "color_g_enable"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_B_ENABLE	                "color_b_enable"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_A_ENABLE	                "color_a_enable"

    //State Lighting
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_AMBIENT			        "ambient"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_DIFFUSE			        "diffuse"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_SPECULAR			    "specular"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_EMISSIVE			    "emissive"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_SHININESS		        "shininess"

    //State Texture
    #define MATERIAL_DATA_TAG_ATTRIBUTE_TEXCOORD			    "texcoord"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_U				        "u"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_V				        "v"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_W				        "w"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_BORDER			        "border"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_MIN				        "min"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_MAG				        "mag"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_MIP				        "mip"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_ANISO			        "aniso"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_BIAS				    "bias"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_NUM				        "num"
    #define MATERIAL_DATA_TAG_ATTRIBUTE_PARAM			        "param"

    //State Shader
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_DATA_TYPE		        "data_type"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_FLOAT4			        "float4"
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_MATRIX44			    "matrix44"



        static bool s_parserXML_StateCommon(FXMLElement* pStateCommon, RenderStateCommon* pSC)
        {
            F_Assert(pStateCommon && pSC && "s_parserXML_StateCommon")

            FXMLAttribute* pAttr = nullptr;
            int count_state_items = pStateCommon->GetElementChildrenCount();
            for (int i = 0; i < count_state_items; i++)
            {
                FXMLElement* pChild = pStateCommon->GetElementChild(i);
                const String& nameTag = pChild->GetName();

                //1> polygon_type
                if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_POLYGON_TYPE)
                {
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->typePolygon = F_ParsePolygonType(pAttr->GetValue());
                    }
                }
                //2> culling_type
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_CULLING_TYPE)
                {
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->typeCulling = F_ParseCullingType(pAttr->GetValue());
                    }
                }
                //3> point_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_POINT_SETTING)
                {
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_SIZE, pSC->fPointSize);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_SPRITE_ENABLE, pSC->bPointSpriteEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_ATTENU_ENABLE, pSC->bPointAttenuEnabled);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_CONSTANT, pSC->fPointAttenuConstant);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_LINEAR, pSC->fPointAttenuLinear);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_QUADRATIC, pSC->fPointAttenuQuadratic);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_MIN_SIZE, pSC->fPointMinSize);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_MAX_SIZE, pSC->fPointMaxSize);
                }
                //4> depth_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_DEPTH_SETTING)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_TEST_ENABLE, pSC->bDepthTestEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_WRITE_ENABLE, pSC->bDepthWriteEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC);
                    if (pAttr != nullptr)
                    {
                        pSC->typeDepthFunc = F_ParseCompareFuncType(pAttr->GetValue());
                    }
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_BIAS_CONSTANT, pSC->fDepthBiasConstant);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_BIAS_SLOPE_SCALE, pSC->fDepthBiasSlopeScale);
                }
                //5> stencil_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_STENCIL_SETTING)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, pSC->bStencilEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC);
                    if (pAttr != nullptr)
                    {
                        pSC->typeStencilFunc = F_ParseCompareFuncType(pAttr->GetValue());
                    }
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_VALUE, pSC->nStencilRefValue);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_MASK, pSC->nStencilMask);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_FAIL_OP);
                    if (pAttr != nullptr)
                    {
                        pSC->typeStencilFailOP = F_ParseStencilOPType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_DEPTH_FAIL_OP);
                    if (pAttr != nullptr)
                    {
                        pSC->typeStencilDepthFailOP = F_ParseStencilOPType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_PASS_OP);
                    if (pAttr != nullptr)
                    {
                        pSC->typeStencilPassOP = F_ParseStencilOPType(pAttr->GetValue());
                    }
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_TWO_SIDED_ENABLE, pSC->bStencilTwoSidedEnabled);
                }
                //6> scissor_test
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_SCISSOR_TEST)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, pSC->bScissorTestEnabled);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_LEFT, pSC->nScissorTest_Left);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_TOP, pSC->nScissorTest_Top);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_RIGHT, pSC->nScissorTest_Right);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_BOTTOM, pSC->nScissorTest_Bottom);
                }
                //7> alpha_test
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_ALPHA_TEST)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, pSC->bAlphaTestEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC);
                    if (pAttr != nullptr)
                    {
                        pSC->typeAlphaRejectFunc = F_ParseCompareFuncType(pAttr->GetValue());
                    }
                    pChild->ParserAttribute_UInt8(MATERIAL_DATA_TAG_ATTRIBUTE_VALUE, pSC->nAlphaRejectValue);
                }
                //8> scene_blending_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_SCENE_BLENDING_SETTING)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, pSC->bSceneBlendingEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->typeSceneBlending = F_ParseSceneBlendingType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_OP_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->typeSceneBlendingOP = F_ParseSceneBlendingOPType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SOURCE);
                    if (pAttr != nullptr)
                    {
                        pSC->typeSceneBlendingFactorSrc = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_DST);
                    if (pAttr != nullptr)
                    {
                        pSC->typeSceneBlendingFactorDst = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }

                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_SEPERATE_ENABLE, pSC->bSceneBlendingSeperateEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_OP2_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->typeSceneBlendingOP2 = F_ParseSceneBlendingOPType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SOURCE2);
                    if (pAttr != nullptr)
                    {
                        pSC->typeSceneBlendingFactorSrc2 = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_DST2);
                    if (pAttr != nullptr)
                    {
                        pSC->typeSceneBlendingFactorDst2 = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }
                }
                //9> color_write
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_COLOR_WRITE)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_R_ENABLE, pSC->bColorRWriteEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_G_ENABLE, pSC->bColorGWriteEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_B_ENABLE, pSC->bColorBWriteEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_A_ENABLE, pSC->bColorAWriteEnabled);
                }
            }
            return true;
        }
        static bool s_parserXML_StateLighting(FXMLElement* pStateLighting, RenderStateLighting* pSL)
        {
            F_Assert(pStateLighting && pSL && "s_parserXML_StateLighting")

            int count_lighting_items = pStateLighting->GetElementChildrenCount();
            for (int i = 0; i < count_lighting_items; i++)
            {
                FXMLElement* pChild = pStateLighting->GetElementChild(i);
                
                
            }
            return true;
        }
            static bool s_parserXML_StateTexture(FXMLElement* pStateTexture, RenderStateTexture* pST)
            {
                F_Assert(pStateTexture && pST && "s_parserXML_StateTexture")

                int count_texture_items = pStateTexture->GetElementChildrenCount();
                for (int i = 0; i < count_texture_items; i++)
                {
                    FXMLElement* pChild = pStateTexture->GetElementChild(i);
                    
                }
                return true;
            }
        static bool s_parserXML_StateShader(FXMLElement* pStateShader, RenderStateShader* pSS)
        {
            F_Assert(pStateShader && pSS && "s_parserXML_StateShader")

            int count_shader_items = pStateShader->GetElementChildrenCount();
            for (int i = 0; i < count_shader_items; i++)
            {
                FXMLElement* pChild = pStateShader->GetElementChild(i);
                

            }
            return true;
        }


    static bool s_parserXML_Pass(FXMLElement* pElementPass, RenderState* pRenderState)
    {
        F_Assert(pElementPass && pRenderState && "s_parserXML_Pass")

        int count_pass_items = pElementPass->GetElementChildrenCount();
        for (int i = 0; i < count_pass_items; i++)
        {
            FXMLElement* pState = pElementPass->GetElementChild(i);
            
            const String& strNameState = pState->GetName();
            if (strNameState == MATERIAL_DATA_TAG_STATE_COMMON)
            {
                if (!s_parserXML_StateCommon(pState, pRenderState->GetStateCommon()))
                {
                    F_LogError("*********************** s_parserXML_Pass: Parse material data state [RenderStateCommon] failed !");
                    return false;
                }
            }
            else if (strNameState == MATERIAL_DATA_TAG_STATE_LIGHTING)
            {
                if (!s_parserXML_StateLighting(pState, pRenderState->GetStateLighting()))
                {
                    F_LogError("*********************** s_parserXML_Pass: Parse material data state [RenderStateLighting] failed !");
                    return false;
                }
            }
            else if (strNameState == MATERIAL_DATA_TAG_STATE_SHADER)
            {
                if (!s_parserXML_StateShader(pState, pRenderState->GetStateShader()))
                {
                    F_LogError("*********************** s_parserXML_Pass: Parse material data state [RenderStateShader] failed !");
                    return false;
                }
            }
        }
        return true;
    }

    static bool	s_parserXML_MaterialData(FXMLElement* pElementMaterialData, MaterialData* pMaterialData)
    {
        F_Assert(pElementMaterialData && pMaterialData && "s_parserXML_MaterialData")

        FXMLAttribute* pAttr = nullptr;
        int count_material_data = pElementMaterialData->GetElementChildrenCount();
		for (int i = 0; i < count_material_data; i++)
        {
            FXMLElement* pElementPass = pElementMaterialData->GetElementChild(i);

            //name
            pAttr = pElementPass->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_NAME);
            if (pAttr == nullptr)
            {
                F_LogError("*********************** s_parserXML_MaterialData: Can not find 'name' Attr from state: [%s] !", pMaterialData->GetName().c_str());
                return false;
            }
            const String& strNamePass = pAttr->GetValue();
            //type
            pAttr = pElementPass->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_TYPE);
            if (pAttr == nullptr)
            {
                F_LogError("*********************** s_parserXML_MaterialData: Can not find 'type' Attr from state: [%s] !", pMaterialData->GetName().c_str());
                return false;
            }
            FRenderPassType typeRenderPass = F_ParseRenderPassType(pAttr->GetValue());

            //State
            RenderState* pRenderState = new RenderState(strNamePass, typeRenderPass);
            if (!s_parserXML_Pass(pElementPass, pRenderState))
            {
                F_DELETE(pRenderState)
                F_LogError("*********************** s_parserXML_MaterialData: s_parserXML_Pass failed, [%s] !", pMaterialData->GetName().c_str());
                return false;
            }
            pMaterialData->AddRenderState(pRenderState);
        }

        return true;
    }


    
    MaterialDataSerializer::MaterialDataSerializer(MaterialDataManager* pMaterialDataManager)
        : m_pMaterialDataManager(pMaterialDataManager)
    {
        
    }
    MaterialDataSerializer::~MaterialDataSerializer()
    {

    }
    
    bool MaterialDataSerializer::Parser(uint32 nGroup, const String& strName, MaterialData* pMaterialData, MaterialDataPtrVector* pRet /*= nullptr*/)
    {
        return true;
    }

    bool MaterialDataSerializer::ParserXML(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet /*= nullptr*/)
    {
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
        if (strPath.empty())
		{
			F_LogError("*********************** MaterialDataSerializer::ParserXML: Can not find material file by group: [%u], name: [%s] !", nGroup, strName.c_str());
			return false;
		}
		
		return ParserXML(strPath.c_str(), nullptr, pRet);
    }

    bool MaterialDataSerializer::ParserXML(const char* szFilePath, MaterialData* pMaterialData /*= nullptr*/, MaterialDataPtrVector* pRet /*= nullptr*/)
    {
        FFileXML xml;
		if (!xml.LoadXMLIndirect(szFilePath))
        {
            F_LogError("*********************** MaterialDataSerializer::ParserXML: Load material file [%s] failed !", szFilePath);
			F_Assert(false && "MaterialDataSerializer::ParserXML: Load file failed !")
			return false;
        }

        FXMLDocument* pXMLDocument = xml.GetXMLDocument();
        FXMLElement* pRoot = pXMLDocument->GetElementRoot();
        int count_material_datas = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_material_datas; i++)
        {
            FXMLElement* pElementMaterialData = pRoot->GetElementChild(i);

            //name
            String strNameMaterial;
            if (!pElementMaterialData->ParserAttribute_String(MATERIAL_DATA_TAG_ATTRIBUTE_NAME, strNameMaterial))
            {
                F_LogError("*********************** MaterialDataSerializer::ParserXML: Can not find attribute: 'name', material data index: [%d] !", i);           
                continue;
            }

            MaterialData* pMD = m_pMaterialDataManager->findMaterialData(strNameMaterial);
            if (pMD != nullptr)
            {
                F_LogWarning("####################### MaterialDataSerializer::ParserXML: Material data [%s] already exist !", strNameMaterial.c_str());
            }
            else
            {
                bool bExtern = pMaterialData ? true : false;
				if (!bExtern)
				{
					pMaterialData = new MaterialData(strNameMaterial);
				}

                if (!s_parserXML_MaterialData(pElementMaterialData, pMaterialData))
				{
					if (!bExtern)
						delete pMaterialData;
					F_LogError("*********************** MaterialDataSerializer::ParserXML: Parser material data [%s] failed !", strNameMaterial.c_str());
					F_Assert(false && "MaterialDataSerializer::ParserXML: Parser material data failed !")
					return false;
				}

                if (!bExtern)
				{
					if (m_pMaterialDataManager->addMaterialData(pMaterialData))
					{
						pMD = pMaterialData;
						F_LogInfo("MaterialDataSerializer::ParserXML: Parser material data [%s] success !", strNameMaterial.c_str());
						pMaterialData = nullptr;
					}
					else
					{
						delete pMaterialData;
						pMaterialData = nullptr;
						F_Assert(false && "MaterialDataSerializer::ParserXML")
					}
				}
				else
				{
					if (pRet && pMaterialData)
					{
						pRet->push_back(pMaterialData);
					}
					F_LogInfo("DfMaterialDataSerializer::ParserXML: Parser material data [%s] success !", strNameMaterial.c_str());
					break;
				}
            }

            if (pRet && pMD)
			{
				pRet->push_back(pMD);
			}
        }

        return true;
    }

    bool MaterialDataSerializer::ParserBinary(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet /*= nullptr*/)
    {
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
        if (strPath.empty())
		{
			F_LogError("*********************** MaterialDataSerializer::ParserBinary: Can not find material file by group: [%u], name: [%s] !", nGroup, strName.c_str());
			return false;
		}
		
		return ParserBinary(strPath.c_str(), nullptr, pRet);
    }

    bool MaterialDataSerializer::ParserBinary(const char* szFilePath, MaterialData* pMaterialData /*= nullptr*/, MaterialDataPtrVector* pRet /*= nullptr*/)
    {

        return true;
    }

    bool MaterialDataSerializer::SaveXML(Material* pMaterial)
    {

        return true;
    }
    bool MaterialDataSerializer::SaveXML(uint32 nGroup, Material* pMaterial)
    {

        return true;
    }
    bool MaterialDataSerializer::SaveXML(Mesh* pMesh)
    {

        return true;
    }
    bool MaterialDataSerializer::SaveXML(Object* pObject, const String& strPath)
    {

        return true;
    }
    bool MaterialDataSerializer::SaveXML(const char* szFilePath, MaterialPtrVector& aMA)
    {

        return true;
    }

    bool MaterialDataSerializer::SaveBinary(Material* pMaterial)
    {

        return true;
    }
    bool MaterialDataSerializer::SaveBinary(uint32 nGroup, Material* pMaterial)
    {

        return true;
    }
    bool MaterialDataSerializer::SaveBinary(Mesh* pMesh)
    {

        return true;
    }
    bool MaterialDataSerializer::SaveBinary(Object* pObject, const String& strPath)
    {

        return true;
    }
    bool MaterialDataSerializer::SaveBinary(const char* szFilePath, MaterialPtrVector& aMA)
    {

        return true;
    }

}; //LostPeter
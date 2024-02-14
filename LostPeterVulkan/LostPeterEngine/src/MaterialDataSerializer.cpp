/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MaterialDataSerializer.h"
#include "../include/MaterialDataManager.h"
#include "../include/MaterialData.h"
#include "../include/MaterialManager.h"
#include "../include/Material.h"
#include "../include/RenderState.h"
#include "../include/Mesh.h"
#include "../include/Object.h"

namespace LostPeterEngine
{
#define MATERIAL_DATA_FILE_XML_EXT								".material"
#define MATERIAL_DATA_FILE_BINARY_EXT							".bmaterial"

#define MATERIAL_DATA_TAG_MATERIAL_CFG								            "cfg_material_data"
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
                #define	MATERIAL_DATA_TAG_STATE_LIGHTING_LIGHTING_SETTING			            "light_setting"					//1
                    #define MATERIAL_DATA_TAG_STATE_LIGHTING_MATERIAL_SETTING		                "material_setting"				//1

            #define	MATERIAL_DATA_TAG_STATE_SHADER							                "state_shader"
                #define MATERIAL_DATA_TAG_STATE_SHADER_VERT							            "vert"						    //1
                #define MATERIAL_DATA_TAG_STATE_SHADER_TESC						                "tesc"						    //2
                #define MATERIAL_DATA_TAG_STATE_SHADER_TESE						                "tese"						    //3
                #define MATERIAL_DATA_TAG_STATE_SHADER_GROM						                "geom"						    //4
                #define MATERIAL_DATA_TAG_STATE_SHADER_FRAG						                "frag"						    //5
                #define MATERIAL_DATA_TAG_STATE_SHADER_COMP						                "comp"						    //6

                    #define	MATERIAL_DATA_TAG_STATE_PARAMS                                          "state_params"		                //1
                        #define	MATERIAL_DATA_TAG_STATE_PARAM                                          "state_param"		                //1

                    #define	MATERIAL_DATA_TAG_STATE_TEXTURE							                "state_texture"			
                        #define	MATERIAL_DATA_TAG_STATE_TEXTURE_UNIT                                    "texture_unit"                  //1
                            #define MATERIAL_DATA_TAG_TAT_TEXTURE_TEXTURE_SETTING			                "texture_setting"		        //1
                            #define MATERIAL_DATA_TAG_TAT_TEXTURE_ANIMATION_SETTING					        "animation_setting"			    //2


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
#define MATERIAL_DATA_TAG_ATTRIBUTE_LAYOUT				        "layout"


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
    #define	MATERIAL_DATA_TAG_ATTRIBUTE_LIGHTING_TYPE			"lighting_type"
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



        static bool s_parserXML_StateCommon(FXMLElement* pElemStateCommon, RenderStateCommon* pSC)
        {
            F_Assert(pElemStateCommon && pSC && "s_parserXML_StateCommon")

            FXMLAttribute* pAttr = nullptr;
            int count_state_items = pElemStateCommon->GetElementChildrenCount();
            for (int i = 0; i < count_state_items; i++)
            {
                FXMLElement* pElemItem = pElemStateCommon->GetElementChild(i);
                const String& nameTag = pElemItem->GetName();

                //1> polygon_type
                if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_POLYGON_TYPE)
                {
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->m_ePolygon = F_ParsePolygonType(pAttr->GetValue());
                    }
                }
                //2> culling_type
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_CULLING_TYPE)
                {
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eCulling = F_ParseCullingType(pAttr->GetValue());
                    }
                }
                //3> point_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_POINT_SETTING)
                {
                    pElemItem->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_SIZE, pSC->m_fPointSize);
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_SPRITE_ENABLE, pSC->m_bPointSpriteEnabled);
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_ATTENU_ENABLE, pSC->m_bPointAttenuEnabled);
                    pElemItem->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_CONSTANT, pSC->m_fPointAttenuConstant);
                    pElemItem->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_LINEAR, pSC->m_fPointAttenuLinear);
                    pElemItem->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_QUADRATIC, pSC->m_fPointAttenuQuadratic);
                    pElemItem->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_MIN_SIZE, pSC->m_fPointMinSize);
                    pElemItem->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_MAX_SIZE, pSC->m_fPointMaxSize);
                }
                //4> depth_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_DEPTH_SETTING)
                {
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_TEST_ENABLE, pSC->m_bDepthTestEnabled);
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_WRITE_ENABLE, pSC->m_bDepthWriteEnabled);
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eDepthCompareFunc = F_ParseCompareFuncType(pAttr->GetValue());
                    }
                    pElemItem->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_BIAS_CONSTANT, pSC->m_fDepthBiasConstant);
                    pElemItem->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_BIAS_SLOPE_SCALE, pSC->m_fDepthBiasSlopeScale);
                }
                //5> stencil_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_STENCIL_SETTING)
                {
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, pSC->m_bStencilEnabled);
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eStencilCompareFunc = F_ParseCompareFuncType(pAttr->GetValue());
                    }
                    pElemItem->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_VALUE, pSC->m_nStencilRefValue);
                    pElemItem->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_MASK, pSC->m_nStencilMask);
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_FAIL_OP);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eStencilFailOP = F_ParseStencilOPType(pAttr->GetValue());
                    }
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_DEPTH_FAIL_OP);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eStencilDepthFailOP = F_ParseStencilOPType(pAttr->GetValue());
                    }
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_PASS_OP);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eStencilPassOP = F_ParseStencilOPType(pAttr->GetValue());
                    }
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_TWO_SIDED_ENABLE, pSC->m_bStencilTwoSidedEnabled);
                }
                //6> scissor_test
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_SCISSOR_TEST)
                {
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, pSC->m_bScissorTestEnabled);
                    pElemItem->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_LEFT, pSC->m_nScissorTest_Left);
                    pElemItem->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_TOP, pSC->m_nScissorTest_Top);
                    pElemItem->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_RIGHT, pSC->m_nScissorTest_Right);
                    pElemItem->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_BOTTOM, pSC->m_nScissorTest_Bottom);
                }
                //7> alpha_test
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_ALPHA_TEST)
                {
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, pSC->m_bAlphaTestEnabled);
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eAlphaRejectCompareFunc = F_ParseCompareFuncType(pAttr->GetValue());
                    }
                    pElemItem->ParserAttribute_UInt8(MATERIAL_DATA_TAG_ATTRIBUTE_VALUE, pSC->m_nAlphaRejectValue);
                }
                //8> scene_blending_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_SCENE_BLENDING_SETTING)
                {
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, pSC->m_bSceneBlendingEnabled);
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eSceneBlending = F_ParseSceneBlendingType(pAttr->GetValue());
                    }
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_OP_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eSceneBlendingOP = F_ParseSceneBlendingOPType(pAttr->GetValue());
                    }
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SOURCE);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eSceneBlendingFactorSrc = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_DST);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eSceneBlendingFactorDst = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }

                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_SEPERATE_ENABLE, pSC->m_bSceneBlendingSeperateEnabled);
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_OP2_TYPE);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eSceneBlendingOP2 = F_ParseSceneBlendingOPType(pAttr->GetValue());
                    }
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SOURCE2);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eSceneBlendingFactorSrc2 = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }
                    pAttr = pElemItem->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_DST2);
                    if (pAttr != nullptr)
                    {
                        pSC->m_eSceneBlendingFactorDst2 = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }
                }
                //9> color_write
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_COLOR_WRITE)
                {
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_R_ENABLE, pSC->m_bColorRWriteEnabled);
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_G_ENABLE, pSC->m_bColorGWriteEnabled);
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_B_ENABLE, pSC->m_bColorBWriteEnabled);
                    pElemItem->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_A_ENABLE, pSC->m_bColorAWriteEnabled);
                }
            }
            return true;
        }


        static bool s_parserXML_StateLighting(FXMLElement* pElemStateLighting, RenderStateLighting* pSL)
        {
            F_Assert(pElemStateLighting && pSL && "s_parserXML_StateLighting")

            FXMLAttribute* pAttr = nullptr;
            FXMLElement* pElemLight = pElemStateLighting->FindElementChild(MATERIAL_DATA_TAG_STATE_LIGHTING_LIGHTING_SETTING);
            F_Assert(pElemLight && "s_parserXML_StateLighting: pElemLight")
            pElemLight->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, pSL->m_bLightingEnabled);
            pAttr = pElemLight->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_LIGHTING_TYPE);
            if (pAttr != nullptr)
            {
                pSL->m_eLighting = F_ParseLightingType(pAttr->GetValue());

                if (pSL->m_eLighting == F_Lighting_Flat ||
                    pSL->m_eLighting == F_Lighting_Gouraud ||
                    pSL->m_eLighting == F_Lighting_Phong)
                {
                    FXMLElement* pElemMaterial = pElemLight->FindElementChild(MATERIAL_DATA_TAG_STATE_LIGHTING_MATERIAL_SETTING);
                }
                else if (pSL->m_eLighting == F_Lighting_Pbr)
                {

                }
            }

            return true;
        }

                static bool s_parserXML_StateParams(FXMLElement* pElemStateParams, RenderStateShaderProgramItem* pSSItem)
                {
                    F_Assert(pElemStateParams && pSSItem && "s_parserXML_StateParams")
                    
                    int count_param_items = pElemStateParams->GetElementChildrenCount();
                    for (int i = 0; i < count_param_items; i++)
                    {
                        FXMLElement* pElemParam = pElemStateParams->GetElementChild(i);
                        
                        
                    }
                    return true;
                }

                static bool s_parserXML_StateTextures(FXMLElement* pElemStateTextures, RenderStateShaderProgramItem* pSSItem)
                {
                    F_Assert(pElemStateTextures && pSSItem && "s_parserXML_StateTextures")

                    int count_texture_items = pElemStateTextures->GetElementChildrenCount();
                    for (int i = 0; i < count_texture_items; i++)
                    {
                        FXMLElement* pElemTexture = pElemStateTextures->GetElementChild(i);
                        
                        uint32 nGroup;
                        if (!pElemTexture->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_GROUP, nGroup))
                        {
                            F_LogError("*********************** s_parserXML_StateTextures: Can not find attribute: 'group', from state texture !");     
                            return false;
                        }
                        String nameTexture;
                        if (!pElemTexture->ParserAttribute_String(MATERIAL_DATA_TAG_ATTRIBUTE_NAME, nameTexture))
                        {
                            F_LogError("*********************** s_parserXML_StateTextures: Can not find attribute: 'name', from state texture !");     
                            return false;
                        }

                        RenderStateTexture* pStateTexture = new RenderStateTexture(nGroup, nameTexture);
                        if (!pStateTexture->LoadTexture())
                        {
                            F_LogError("*********************** s_parserXML_StateTextures: Load texture failed, group: [%u], name: [%s] !", nGroup, nameTexture.c_str()); 
                            return false;
                        }
                        pSSItem->AddStateTexture(pStateTexture);

                        F_LogInfo("############### s_parserXML_StateTextures: Parser texture unit, index: [%d], group: [%u], name: [%s] success !", i, nGroup, nameTexture.c_str());
                    }
                    return true;
                }
            static bool s_parserXML_StateShaderProgramItem(FXMLElement* pElemShaderItem, RenderStateShaderProgramItem* pSSItem)
            {
                F_Assert(pElemShaderItem && "s_parserXML_StateShaderProgramItem")

                int count_items = pElemShaderItem->GetElementChildrenCount();
                for (int i = 0; i < count_items; i++)
                {
                    FXMLElement* pElemItem = pElemShaderItem->GetElementChild(i);

                    const String& strNameItem = pElemItem->GetName();
                    if (strNameItem == MATERIAL_DATA_TAG_STATE_PARAMS)
                    {
                        if (!s_parserXML_StateParams(pElemItem, pSSItem))
                        {
                            F_LogError("*********************** s_parserXML_StateShaderProgramItem: Parse shader params failed !");
                            return false;
                        }
                    }
                    else if (strNameItem == MATERIAL_DATA_TAG_STATE_TEXTURE)
                    {
                        if (!s_parserXML_StateTextures(pElemItem, pSSItem))
                        {
                            F_LogError("*********************** s_parserXML_StateShaderProgramItem: Parse shader textures failed !");
                            return false;
                        }
                    }
                }

                return true;
            }
        static bool s_parserXML_StateShader(FXMLElement* pElemStateShader, RenderStateShader* pSS)
        {
            F_Assert(pElemStateShader && pSS && "s_parserXML_StateShader")

            String nameLayout;
            if (!pElemStateShader->ParserAttribute_String(MATERIAL_DATA_TAG_ATTRIBUTE_LAYOUT, nameLayout))
            {
                F_LogError("*********************** s_parserXML_StateShader: Can not find attribute: 'layout', from state shader !");     
                return false;
            }
            pSS->SetNameDescriptorSetLayout(nameLayout);

            int count_shader_items = pElemStateShader->GetElementChildrenCount();
            for (int i = 0; i < count_shader_items; i++)
            {
                FXMLElement* pElemShaderItem = pElemStateShader->GetElementChild(i);
                
                //Shader Type
                const String& nameShaderType = pElemShaderItem->GetName();  
                FShaderType eShader = F_ParseShaderType(nameShaderType);

                //ShaderProgram Name
                String nameShaderProgram;
                if (!pElemShaderItem->ParserAttribute_String(MATERIAL_DATA_TAG_ATTRIBUTE_NAME, nameShaderProgram))
                {
                    F_LogError("*********************** s_parserXML_StateShader: Can not find attribute: 'name', state shader index: [%d] !", i);           
                    return false;
                }

                RenderStateShaderProgramItem* pSSItem = new RenderStateShaderProgramItem(nameShaderProgram, eShader);
                if (!pSSItem->LoadShaderProgram())
                {
                    F_DELETE(pSSItem)
                    F_LogError("*********************** s_parserXML_StateShader: Load shader program failed, group: [%u], name: [%s], type: [%s] !", FPathManager::PathGroup_Shader, nameShaderProgram.c_str(), nameShaderType.c_str()); 
                    return false;
                }

                if (!s_parserXML_StateShaderProgramItem(pElemShaderItem, pSSItem))
                {
                    F_DELETE(pSSItem)
                    F_LogError("*********************** s_parserXML_StateShader: Parse state shader program item, name: [%s], type: [%s] failed !", nameShaderProgram.c_str(), nameShaderType.c_str());           
                    return false;
                }
                pSS->AddRenderStateShaderProgramItem(pSSItem);

                F_LogInfo("########## s_parserXML_StateShader: Parser shader item, index: [%d], name: [%s], type: [%s] success !", i, nameShaderProgram.c_str(), nameShaderType.c_str());
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
            const String& strTypePass = pAttr->GetValue();
            FRenderPassType typeRenderPass = F_ParseRenderPassType(strTypePass);

            //State
            RenderState* pRenderState = new RenderState(strNamePass, typeRenderPass);
            if (!s_parserXML_Pass(pElementPass, pRenderState))
            {
                F_DELETE(pRenderState)
                F_LogError("*********************** s_parserXML_MaterialData: s_parserXML_Pass failed, [%s] !", pMaterialData->GetName().c_str());
                return false;
            }
            pMaterialData->AddRenderState(pRenderState);

            F_LogInfo("##### s_parserXML_MaterialData: Parser pass, index: [%d], name: [%s], type: [%s] success !", i, strNamePass.c_str(), strTypePass.c_str());
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
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
		if (strPath.empty())
			return false;
        
		if (strPath.find(MATERIAL_DATA_FILE_BINARY_EXT) != String::npos)
		{
			if (!ParserBinary(nGroup, strName, strPath, pMaterialData, pRet))
			{
                F_LogError("*********************** MaterialDataSerializer::Parser: Parser material binary file failed, path: [%s] !", strPath.c_str());
				return false;
			}
			return true;
		}
		else if(strPath.find(MATERIAL_DATA_FILE_XML_EXT) != String::npos)
		{
			if (!ParserXML(nGroup, strName, strPath, pMaterialData, pRet))
			{
                F_LogError("*********************** MaterialDataSerializer::Parser: Parser material xml file failed, path: [%s] !", strPath.c_str());
				return false;
			}
			return true;
		}
		else
		{	
            F_LogError("*********************** MaterialDataSerializer::Parser: Not valid material file, path: [%s] !", strPath.c_str());
		}
		
		return false;
    }

    bool MaterialDataSerializer::ParserXML(uint32 nGroup, const String& strName, MaterialDataPtrVector* pRet /*= nullptr*/)
    {
        String strPath = FPathManager::GetSingleton().GetFilePath(nGroup, strName);
        if (strPath.empty())
		{
			F_LogError("*********************** MaterialDataSerializer::ParserXML: Can not find material file by group: [%u], name: [%s] !", nGroup, strName.c_str());
			return false;
		}
		
		return ParserXML(nGroup, strName, strPath, nullptr, pRet);
    }

    bool MaterialDataSerializer::ParserXML(uint32 nGroup, const String& strName, const String& strPath, MaterialData* pMaterialData /*= nullptr*/, MaterialDataPtrVector* pRet /*= nullptr*/)
    {
        FFileXML xml;
		if (!xml.LoadXMLIndirect(strPath))
        {
            F_LogError("*********************** MaterialDataSerializer::ParserXML: Load material file: [%s] failed !", strPath.c_str());
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
            F_LogInfo("MaterialDataSerializer::ParserXML: Start to parser material data: [%s] !", strNameMaterial.c_str());

            MaterialData* pMD = m_pMaterialDataManager->GetMaterialData(strNameMaterial);
            if (pMD != nullptr)
            {
                F_LogWarning("####################### MaterialDataSerializer::ParserXML: Material data [%s] already exist !", strNameMaterial.c_str());
            }
            else
            {
                bool bExtern = pMaterialData ? true : false;
				if (!bExtern)
				{
					pMaterialData = m_pMaterialDataManager->NewMaterialData(nGroup, strNameMaterial);
				}
                pMaterialData->SetPath(strPath);

                if (!s_parserXML_MaterialData(pElementMaterialData, pMaterialData))
				{
					if (!bExtern)
						delete pMaterialData;
					F_LogError("*********************** MaterialDataSerializer::ParserXML: Parser material data: [%s] failed !", strNameMaterial.c_str());
					return false;
				}

                if (!bExtern)
				{
					if (m_pMaterialDataManager->AddMaterialData(pMaterialData))
					{
						pMD = pMaterialData;
						F_LogInfo("MaterialDataSerializer::ParserXML: Parser material data [%s] success !", strNameMaterial.c_str());
						pMaterialData = nullptr;
					}
					else
					{
                        F_DELETE(pMaterialData)
                        F_LogError("*********************** MaterialDataSerializer::ParserXML: addMaterialData: [%s] failed !", strNameMaterial.c_str());
					    return false;
					}
				}
				else
				{
					if (pRet && pMaterialData)
					{
						pRet->push_back(pMaterialData);
					}
					F_LogInfo("MaterialDataSerializer::ParserXML: Parser material data: [%s] success !", strNameMaterial.c_str());
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
		
		return ParserBinary(nGroup, strName, strPath, nullptr, pRet);
    }

    bool MaterialDataSerializer::ParserBinary(uint32 nGroup, const String& strName, const String& strPath, MaterialData* pMaterialData /*= nullptr*/, MaterialDataPtrVector* pRet /*= nullptr*/)
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
    bool MaterialDataSerializer::SaveXML(const String& strPath, MaterialPtrVector& aMA)
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
    bool MaterialDataSerializer::SaveBinary(const String& strPath, MaterialPtrVector& aMA)
    {

        return true;
    }

}; //LostPeterEngine
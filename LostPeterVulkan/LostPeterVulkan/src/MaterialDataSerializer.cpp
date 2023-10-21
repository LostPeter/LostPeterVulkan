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

		    #define	MATERIAL_DATA_TAG_STATE_TEXTURE							                "state_texture"					
			    #define MATERIAL_DATA_TAG_STATE_TEXTURE_TEXTURE_UNIT						    "tex_unit"
                    #define MATERIAL_DATA_TAG_TAT_TEXTURE_TEXTURE_SETTING						    "tex_setting"				//1
                    #define MATERIAL_DATA_TAG_TAT_TEXTURE_ANIMATION_SETTING					        "anim_setting"				//2

		    #define	MATERIAL_DATA_TAG_STATE_SHADER							                "state_shader"
                #define MATERIAL_DATA_TAG_STATE_SHADER_VERT							            "vert"						    //1
                #define MATERIAL_DATA_TAG_STATE_SHADER_TESC						                "tesc"						    //2
                #define MATERIAL_DATA_TAG_STATE_SHADER_TESE						                "tese"						    //3
                #define MATERIAL_DATA_TAG_STATE_SHADER_GROM						                "geom"						    //4
                #define MATERIAL_DATA_TAG_STATE_SHADER_FRAG						                "frag"						    //5
                #define MATERIAL_DATA_TAG_STATE_SHADER_COMP						                "comp"						    //6


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



        static bool s_parserXML_StateCommon(FXMLElement* pStateCommon, RenderState::StateCommon& sc)
        {
            F_Assert(pStateCommon && "s_parserXML_StateCommon")

            FXMLAttribute* pAttr = nullptr;
            int count_child = pStateCommon->GetElementChildrenCount();
            for (int i = 0; i < count_child; i++)
            {
                FXMLElement* pChild = pStateCommon->GetElementChild(i);
                const String& nameTag = pChild->GetName();

                //1> polygon_type
                if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_POLYGON_TYPE)
                {
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_TYPE);
                    if (pAttr != nullptr)
                    {
                        sc.typePolygon = F_ParsePolygonType(pAttr->GetValue());
                    }
                }
                //2> culling_type
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_CULLING_TYPE)
                {
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_TYPE);
                    if (pAttr != nullptr)
                    {
                        sc.typeCulling = F_ParseCullingType(pAttr->GetValue());
                    }
                }
                //3> point_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_POINT_SETTING)
                {
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_SIZE, sc.fPointSize);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_SPRITE_ENABLE, sc.bPointSpriteEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_ATTENU_ENABLE, sc.bPointAttenuEnabled);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_CONSTANT, sc.fPointAttenuConstant);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_LINEAR, sc.fPointAttenuLinear);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_QUADRATIC, sc.fPointAttenuQuadratic);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_MIN_SIZE, sc.fPointMinSize);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_POINT_MAX_SIZE, sc.fPointMaxSize);
                }
                //4> depth_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_DEPTH_SETTING)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_TEST_ENABLE, sc.bDepthTestEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_WRITE_ENABLE, sc.bDepthWriteEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC);
                    if (pAttr != nullptr)
                    {
                        sc.typeDepthFunc = F_ParseCompareFuncType(pAttr->GetValue());
                    }
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_BIAS_CONSTANT, sc.fDepthBiasConstant);
                    pChild->ParserAttribute_Float(MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_BIAS_SLOPE_SCALE, sc.fDepthBiasSlopeScale);
                }
                //5> stencil_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_STENCIL_SETTING)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, sc.bStencilEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC);
                    if (pAttr != nullptr)
                    {
                        sc.typeStencilFunc = F_ParseCompareFuncType(pAttr->GetValue());
                    }
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_VALUE, sc.nStencilRefValue);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_MASK, sc.nStencilMask);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_FAIL_OP);
                    if (pAttr != nullptr)
                    {
                        sc.typeStencilFailOP = F_ParseStencilOPType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_DEPTH_FAIL_OP);
                    if (pAttr != nullptr)
                    {
                        sc.typeStencilDepthFailOP = F_ParseStencilOPType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_PASS_OP);
                    if (pAttr != nullptr)
                    {
                        sc.typeStencilPassOP = F_ParseStencilOPType(pAttr->GetValue());
                    }
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_STENCIL_TWO_SIDED_ENABLE, sc.bStencilTwoSidedEnabled);
                }
                //6> scissor_test
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_SCISSOR_TEST)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, sc.bScissorTestEnabled);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_LEFT, sc.nScissorTest_Left);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_TOP, sc.nScissorTest_Top);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_RIGHT, sc.nScissorTest_Right);
                    pChild->ParserAttribute_UInt(MATERIAL_DATA_TAG_ATTRIBUTE_SCISSOR_TEST_BOTTOM, sc.nScissorTest_Bottom);
                }
                //7> alpha_test
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_ALPHA_TEST)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, sc.bAlphaTestEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_COMPARE_FUNC);
                    if (pAttr != nullptr)
                    {
                        sc.typeAlphaRejectFunc = F_ParseCompareFuncType(pAttr->GetValue());
                    }
                    pChild->ParserAttribute_UInt8(MATERIAL_DATA_TAG_ATTRIBUTE_VALUE, sc.nAlphaRejectValue);
                }
                //8> scene_blending_setting
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_SCENE_BLENDING_SETTING)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE, sc.bSceneBlendingEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_TYPE);
                    if (pAttr != nullptr)
                    {
                        sc.typeSceneBlending = F_ParseSceneBlendingType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_OP_TYPE);
                    if (pAttr != nullptr)
                    {
                        sc.typeSceneBlendingOP = F_ParseSceneBlendingOPType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SOURCE);
                    if (pAttr != nullptr)
                    {
                        sc.typeSceneBlendingFactorSrc = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_DST);
                    if (pAttr != nullptr)
                    {
                        sc.typeSceneBlendingFactorDst = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }

                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_SEPERATE_ENABLE, sc.bSceneBlendingSeperateEnabled);
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SCENE_BLENDING_OP2_TYPE);
                    if (pAttr != nullptr)
                    {
                        sc.typeSceneBlendingOP2 = F_ParseSceneBlendingOPType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_SOURCE2);
                    if (pAttr != nullptr)
                    {
                        sc.typeSceneBlendingFactorSrc2 = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }
                    pAttr = pChild->FindAttribute(MATERIAL_DATA_TAG_ATTRIBUTE_DST2);
                    if (pAttr != nullptr)
                    {
                        sc.typeSceneBlendingFactorDst2 = F_ParseSceneBlendingFactorType(pAttr->GetValue());
                    }
                }
                //9> color_write
                else if (nameTag == MATERIAL_DATA_TAG_STATE_COMMON_COLOR_WRITE)
                {
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_R_ENABLE, sc.bColorRWriteEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_G_ENABLE, sc.bColorGWriteEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_B_ENABLE, sc.bColorBWriteEnabled);
                    pChild->ParserAttribute_Bool(MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_A_ENABLE, sc.bColorAWriteEnabled);
                }
            }
            return true;
        }
        static bool s_parserXML_StateLighting(FXMLElement* pStateLighting, RenderState::StateLighting& sl)
        {
            F_Assert(pStateLighting && "s_parserXML_StateLighting")

            int count_child = pStateLighting->GetElementChildrenCount();
            for (int i = 0; i < count_child; i++)
            {
                FXMLElement* pChild = pStateLighting->GetElementChild(i);
                
                
            }
            return true;
        }
        static bool s_parserXML_StateTexture(FXMLElement* pStateTexture, RenderState::StateTexture& st)
        {
            F_Assert(pStateTexture && "s_parserXML_StateTexture")

            int count_child = pStateTexture->GetElementChildrenCount();
            for (int i = 0; i < count_child; i++)
            {
                FXMLElement* pChild = pStateTexture->GetElementChild(i);
                
            }
            return true;
        }
        static bool s_parserXML_StateShader(FXMLElement* pStateShader, RenderState::StateShader& ss)
        {
            F_Assert(pStateShader && "s_parserXML_StateShader")

            int count_child = pStateShader->GetElementChildrenCount();
            for (int i = 0; i < count_child; i++)
            {
                FXMLElement* pChild = pStateShader->GetElementChild(i);
                
            }
            return true;
        }

    static bool	s_parserXML_MaterialData(FXMLElement* pElementMaterialData, MaterialData* pMaterialData)
    {
        F_Assert(pElementMaterialData && pMaterialData && "s_parserXML_MaterialData")

        int count_child = pElementMaterialData->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
        {
            FXMLElement* pState = pElementMaterialData->GetElementChild(i);

            const String& strNameState = pState->GetName();
            if (strNameState == MATERIAL_DATA_TAG_STATE_COMMON)
            {
                RenderState::StateCommon sc;
                if (!s_parserXML_StateCommon(pState, sc))
                {
                    F_LogError("*********************** s_parserXML_MaterialData: Parse materal data state [StateCommon] failed !");
                    return false;
                }
                pMaterialData->SerializerFrom(&sc);
            }
            else if (strNameState == MATERIAL_DATA_TAG_STATE_LIGHTING)
            {
                RenderState::StateLighting sl;
                if (!s_parserXML_StateLighting(pState, sl))
                {
                    F_LogError("*********************** s_parserXML_MaterialData: Parse materal data state [StateLighting] failed !");
                    return false;
                }
                pMaterialData->SerializerFrom(&sl);
            }
            else if (strNameState == MATERIAL_DATA_TAG_STATE_TEXTURE)
            {
                RenderState::StateTexture st;
                if (!s_parserXML_StateTexture(pState, st))
                {
                    F_LogError("*********************** s_parserXML_MaterialData: Parse materal data state [StateTexture] failed !");
                    return false;
                }
                pMaterialData->SerializerFrom(&st);
            }
            else if (strNameState == MATERIAL_DATA_TAG_STATE_SHADER)
            {
                RenderState::StateShader ss;
                if (!s_parserXML_StateShader(pState, ss))
                {
                    F_LogError("*********************** s_parserXML_MaterialData: Parse materal data state [StateShader] failed !");
                    return false;
                }
                pMaterialData->SerializerFrom(&ss);
            }
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
        int count_child = pRoot->GetElementChildrenCount();
		for (int i = 0; i < count_child; i++)
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
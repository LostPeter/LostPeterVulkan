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

namespace LostPeter
{
#define MATERIAL_DATA_TAG_MATERIAL_CFG								            "cfg_material_data"
    #define	MATERIAL_DATA_TAG_MATERIALS									            "material_datas"
        #define	MATERIAL_DATA_TAG_MATERIAL									            "material_data"
            #define	MATERIAL_DATA_TAG_STATE_COMMON							                "state_common"
                #define	MATERIAL_DATA_TAG_STATE_COMMON_ALPHA_TEST			                    "alpha_test"					//1
                #define	MATERIAL_DATA_TAG_STATE_COMMON_COLOR_WRITE				                "color_write"					//2
                #define	MATERIAL_DATA_TAG_STATE_COMMON_CULLING_TYPE				                "culling_type"					//3
                #define	MATERIAL_DATA_TAG_STATE_COMMON_DEPTH_SETTING				            "depth_setting"					//4
                #define	MATERIAL_DATA_TAG_STATE_COMMON_POINT_SETTING				            "point_setting"					//5
                #define	MATERIAL_DATA_TAG_STATE_COMMON_POLYGON_TYPE				                "polygon_type"					//6
                #define	MATERIAL_DATA_TAG_STATE_COMMON_SCENE_BLENDING_SETTING	                "scene_blending_setting"		//7
                #define	MATERIAL_DATA_TAG_STATE_COMMON_STENCIL_SETTING			                "stencil_setting"				//8
            
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


#define MATERIAL_DATA_TAG_ATTRIBUTE_NAME		            "name"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_TYPE				    "type"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_SOURCE			        "src"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_DEST				    "dst"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_ENABLE			        "enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_R_ENABLE	        "c_r_enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_G_ENABLE	        "c_g_enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_B_ENABLE	        "c_b_enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_COLOR_A_ENABLE	        "c_a_enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_WRITE_ENABLE		    "w_enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_CHECK_ENABLE		    "c_enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_SPRITE_ENABLE	        "s_enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_ATTENU_ENABLE	        "a_enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_TWO_SIDED_ENABLE	    "ts_enable"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_FUNC				    "func"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_VALUE			        "value"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_MASK				    "mask"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_INDEX			        "index"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_BIAS_CONSTANT	        "bias_c"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_BIAS_SLOPE_SCALE	    "bias_s"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_SIZE				    "size"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_MIN_SIZE			    "min_size"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_MAX_SIZE			    "max_size"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_CONSTANT			    "constant"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_LINEAR			        "linear"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_QUADRATIC		        "quadratic"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_FAIL_OP			        "fail_op"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_DEPTH_FAIL_OP	        "depth_fail_op"
#define MATERIAL_DATA_TAG_ATTRIBUTE_SB_TYPE			        "sb_type"
#define MATERIAL_DATA_TAG_ATTRIBUTE_SB_OP_TYPE		        "sb_op_type"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_PASS_OP			        "pass_op"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_AMBIENT			        "ambient"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_DIFFUSE			        "diffuse"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_SPECULAR			    "specular"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_EMISSIVE			    "emissive"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_SHININESS		        "shininess"
#define MATERIAL_DATA_TAG_ATTRIBUTE_FLAG				    "flag"
#define MATERIAL_DATA_TAG_ATTRIBUTE_GROUP			        "group"
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
#define	MATERIAL_DATA_TAG_ATTRIBUTE_DATA_TYPE		        "data_type"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_FLOAT4			        "float4"
#define	MATERIAL_DATA_TAG_ATTRIBUTE_MATRIX44			    "matrix44"


    
    MaterialDataSerializer::MaterialDataSerializer()
        : Base("MaterialDataSerializer")
    {
        
    }
    MaterialDataSerializer::~MaterialDataSerializer()
    {

    }
    
    //File Content XML
    bool MaterialDataSerializer::serializeXML()
    {
        return false;
    }
    bool MaterialDataSerializer::deserializeXML()
    {
        return false;
    }

    //File Content Binary
    bool MaterialDataSerializer::serializeBinary()
    {
        return false;
    }
    bool MaterialDataSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter
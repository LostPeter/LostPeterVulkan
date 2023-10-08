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

#include "../include/MeshSerializer.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    #define MESH_TAG_MESH_CFG								"cfg_mesh"
    #define	MESH_TAG_MESH									"mesh"

#define MESH_TAG_ATTRIBUTE_NAME		            "name"
#define MESH_TAG_ATTRIBUTE_PATH			        "path"
#define MESH_TAG_ATTRIBUTE_TYPE_MESH			"type_mesh"
#define MESH_TAG_ATTRIBUTE_TYPE_MESH_GEOMETRY	"type_mesh_geometry"
#define MESH_TAG_ATTRIBUTE_TYPE_VERTEX	        "type_vertex"
#define MESH_TAG_ATTRIBUTE_IS_FLIP_Y	        "is_flip_y"
#define MESH_TAG_ATTRIBUTE_IS_TRANS_LOCAL	    "is_trans_local"
#define MESH_TAG_ATTRIBUTE_TRANS_LOCAL	        "trans_local"


    MeshSerializer::MeshSerializer()
        : Base("MeshSerializer")
    {
        
    }
    MeshSerializer::~MeshSerializer()
    {

    }
    
    //File Content XML
    bool MeshSerializer::serializeXML()
    {

        return false;
    }
    bool MeshSerializer::deserializeXML()
    {
        
        
        return false;
    }

    //File Content Binary
    bool MeshSerializer::serializeBinary()
    {
        return false;
    }
    bool MeshSerializer::deserializeBinary()
    {
        return false;
    }

}; //LostPeter
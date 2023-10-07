/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _MESH_DATA_SERIALIZER_H_
#define _MESH_DATA_SERIALIZER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport MeshDataSerializer : public FSerializer
                                        , public Base
    {
    public:
        MeshDataSerializer();
        virtual ~MeshDataSerializer();

    public:

    public:

    protected:
        //File Content XML
        virtual bool serializeXML();
        virtual bool deserializeXML();

        //File Content Binary
        virtual bool serializeBinary();
        virtual bool deserializeBinary();
    };

}; //LostPeter

#endif
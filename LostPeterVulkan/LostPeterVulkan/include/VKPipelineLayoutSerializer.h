/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_PIPELINE_LAYOUT_SERIALIZER_H_
#define _VK_PIPELINE_LAYOUT_SERIALIZER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport VKPipelineLayoutSerializer : public FSerializer
                                                , public Base
    {
    public:
        VKPipelineLayoutSerializer();
        virtual ~VKPipelineLayoutSerializer();

    public:
        

    public:
        void Destroy();

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
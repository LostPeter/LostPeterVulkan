/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _TEXTURE_SERIALIZER_H_
#define _TEXTURE_SERIALIZER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport TextureSerializer : public FSerializer
                                       , public Base
    {
    public:
        TextureSerializer();
        virtual ~TextureSerializer();

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
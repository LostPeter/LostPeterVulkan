/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-03-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(input_attachment_index = 1, set = 0, binding = 5) uniform subpassInput redChannel;
layout(input_attachment_index = 2, set = 0, binding = 6) uniform subpassInput greenChannel;
layout(input_attachment_index = 3, set = 0, binding = 7) uniform subpassInput blueChannel;

layout(location = 0) out vec4 combinedChannels;

void main()
{
    combinedChannels = subpassLoad(redChannel) + subpassLoad(greenChannel) + subpassLoad(blueChannel);
}
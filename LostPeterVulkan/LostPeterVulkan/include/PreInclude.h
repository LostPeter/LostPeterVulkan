/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _PRE_INCLUDE_H_
#define _PRE_INCLUDE_H_

#include "FPreInclude.h"

#include "PreDefine.h"
#include "VulkanDefine.h"

#include "App.h"
#include "VulkanBase.h"
#include "VulkanWindow.h"

#include "VKDebug.h"
#include "VKFrameBufferAttachment.h"
#include "VKMultiRenderPass.h"
#include "VKPipelineCompute.h"
#include "VKPipelineComputeCull.h"
#include "VKPipelineComputeTerrain.h"
#include "VKPipelineGraphics.h"
#include "VKPipelineGraphicsCopyBlitFromFrame.h"
#include "VKPipelineGraphicsCopyBlitToFrame.h"
#include "VKPipelineGraphicsDepthShadowMap.h"
#include "VKPipelineGraphicsTerrain.h"
#include "VKRenderPassCopyBlitFromFrame.h"
#include "VKRenderPassCull.h"
#include "VKRenderPassShadowMap.h"
#include "VKRenderPassTerrain.h"

#include "Base.h"
#include "BufferCompute.h"
#include "BufferIndirectCommand.h"
#include "BufferStorage.h"
#include "BufferUniform.h"
#include "CullLodData.h"
#include "CullRenderData.h"
#include "CullManager.h"
#include "CullObjectDynamic.h"
#include "CullObjectStatic.h"
#include "CullTerrainStatic.h"
#include "CullUnit.h"
#include "CullUnitObject.h"
#include "CullUnitTerrain.h"
#include "EditorBase.h"
#include "EditorCameraAxis.h"
#include "EditorCoordinateAxis.h"
#include "EditorGrid.h"
#include "EditorLineFlat2DCollector.h"
#include "EditorLineFlat3DCollector.h"

#include "Mesh.h"
#include "MeshSub.h"
#include "Texture.h"

namespace LostPeterVulkan
{	

}; //LostPeterVulkan

#endif
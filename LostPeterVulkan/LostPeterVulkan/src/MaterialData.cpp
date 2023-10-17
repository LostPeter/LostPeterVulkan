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

#include "../include/MaterialData.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    MaterialData::MaterialData(const String& nameMaterialData)
        : Base(nameMaterialData)
        , m_pStateCommonBuf(nullptr)
		, m_pStateCommonBufSize(0)
        , m_pStateLightingBuf(nullptr)
		, m_pStateLightingBufSize(0)
		, m_pStateTextureBuf(nullptr)
		, m_pStateTextureBufSize(0)
		, m_pShaderStateBuf(nullptr)
		, m_pStateShaderBufSize(0)
    {
        
    }
    MaterialData::~MaterialData()
    {
        Destroy();
    }

    void MaterialData::Destroy()
	{
		F_DELETE_T(m_pStateCommonBuf)
        F_DELETE_T(m_pStateLightingBuf)
		F_DELETE_T(m_pStateTextureBuf)
		F_DELETE_T(m_pShaderStateBuf)
	}

	void MaterialData::SerializerFrom(uint8* pBuffer)
	{
		F_Assert(pBuffer && "MaterialData::SerializerFrom")
		//1> StateCommon

        //2> StateLighting

		//3> StateTexture

		//4> StateShader

	}

	void MaterialData::SerializerFrom(const RenderState* pRS)
	{
		F_Assert(pRS && "MaterialData::SerializerFrom")
		//1> StateCommon
		SerializerFrom(pRS->GetStateCommon());
        //2> StateLighting
		SerializerFrom(pRS->GetStateLighting());
		//3> StateTexture
		SerializerFrom(pRS->GetStateTexture());
		//4> StateShader
		SerializerFrom(pRS->GetStateShader());
	}

	void MaterialData::SerializerFrom(RenderState::StateCommon* pSC)
	{
		F_Assert(pSC && "MaterialData::SerializerFrom: StateCommon")
		uint32 nNewSize = sizeof(RenderState::StateCommon);
		if (nNewSize != m_pStateCommonBufSize)
		{
			F_DELETE_T(m_pStateCommonBuf)
			m_pStateCommonBufSize = nNewSize;
			m_pStateCommonBuf = new uint8[nNewSize];
		}
		pSC->CopyTo((RenderState::StateCommon*)m_pStateCommonBuf);
	}

    void MaterialData::SerializerFrom(RenderState::StateLighting* pSL)
	{
		if (!pSL)
			return;
		F_DELETE_T(m_pStateLightingBuf)

	}

	void MaterialData::SerializerFrom(RenderState::StateTexture* pST)
	{
		F_Assert(pST && "MaterialData::SerializerFrom: StateTexture")
		
	}

	void MaterialData::SerializerFrom(uint8* pBuf, uint32 nTexUnitNum)
	{
		
	}

	void MaterialData::SerializerFrom(RenderState::StateShader* pSS)
	{
		if (!pSS)
			return;
        
	}

	

	void MaterialData::SerializerTo(uint8* pBuffer)
	{
		F_Assert(pBuffer && "MaterialData::SerializerTo")
		//1> StateCommon

        //2> StateLighting

		//3> StateTexture

		//4> StateShader
	}

	void MaterialData::SerializerTo(RenderState* pRS)
	{
		F_Assert(pRS && "MaterialData::SerializerTo")
		//1> StateCommon
		SerializerTo(pRS->GetStateCommon());
        //2> StateLighting
		SerializerTo(pRS->GetStateLighting());
		//3> StateTexture
		SerializerTo(pRS->GetStateTexture());
		//4> StateShader
		SerializerTo(pRS->GetStateShader());
	}

	void MaterialData::SerializerTo(RenderState::StateCommon* pSC)
	{
		F_Assert(pSC && "MaterialData::SerializerTo: StateCommon")
		if (!m_pStateCommonBuf)
			return;
		pSC->CopyFrom((RenderState::StateCommon*)m_pStateCommonBuf);
	}

    void MaterialData::SerializerTo(RenderState::StateLighting* pSL)
	{
		if (!pSL)
			return;

	}

	void MaterialData::SerializerTo(RenderState::StateTexture* pST)
	{
		F_Assert(pST && "MaterialData::SerializerTo: StateTexture")
		if (!m_pStateTextureBuf || m_pStateTextureBufSize <= 0)
			return;
		
	}

	void MaterialData::SerializerTo(RenderState::StateShader* pSS)
	{
		F_Assert(pSS && "MaterialData::SerializerTo: StateShader")
		if (!m_pShaderStateBuf || m_pStateShaderBufSize<=0)
			return;
		
	}
    
}; //LostPeter
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

#ifndef _MATERIAL_DATA_H_
#define _MATERIAL_DATA_H_

#include "Base.h"
#include "RenderState.h"

namespace LostPeter
{
    class utilExport MaterialData : public Base
                                  , public FNonCopyable
    {
    public:
        MaterialData(const String& nameMaterialData);
        virtual ~MaterialData();

    public:
    private:
        uint8* m_pStateCommonBuf;
		uint32 m_pStateCommonBufSize;
        uint8* m_pStateLightingBuf;
		uint32 m_pStateLightingBufSize;
		uint8* m_pStateTextureBuf;
		uint32 m_pStateTextureBufSize;
		uint8* m_pShaderStateBuf;
		uint32 m_pStateShaderBufSize;

    public:
        LP_FORCEINLINE uint8* GetStateCommonBuf() { return m_pStateCommonBuf; }
		LP_FORCEINLINE uint32 GetStateCommonBufSize() { return m_pStateCommonBufSize; }
        LP_FORCEINLINE uint8* GetStateLightingBuf() { return m_pStateLightingBuf; }
		LP_FORCEINLINE uint32 GetStateLightingBufSize() { return m_pStateLightingBufSize; }
		LP_FORCEINLINE uint8* GetStateTextureBuf() { return m_pStateTextureBuf; }
		LP_FORCEINLINE uint32 GetStateTextureBufSize() { return m_pStateTextureBufSize; }
		LP_FORCEINLINE uint8* GetStateShaderBuf() { return m_pShaderStateBuf; }
		LP_FORCEINLINE uint32 GetStateShaderBufSize() { return m_pStateShaderBufSize; }

    public:
		void Destroy();

	public:
		void SerializerFrom(uint8* pBuffer);
		void SerializerFrom(const RenderState* pRS);
		//Common
		void SerializerFrom(RenderState::StateCommon* pSC);
        //Lighting
		void SerializerFrom(RenderState::StateLighting* pSL);
		//Texture
		void SerializerFrom(RenderState::StateTexture* pST);
		void SerializerFrom(uint8* pBuf,uint32 nTexUnitNum);
		//Shader
		void SerializerFrom(RenderState::StateShader* pSS);
		
		void SerializerTo(uint8* pBuffer);
		void SerializerTo(RenderState* pRS);
		//Common
		void SerializerTo(RenderState::StateCommon* pSC);
        //Lighting
		void SerializerTo(RenderState::StateLighting* pSL);
		//Texture
		void SerializerTo(RenderState::StateTexture* pST);
		//Shader
		void SerializerTo(RenderState::StateShader* pSS);
		
    };

}; //LostPeter

#endif
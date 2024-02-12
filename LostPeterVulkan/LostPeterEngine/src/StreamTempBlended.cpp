/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/StreamTempBlended.h"
#include "../include/StreamVertex.h"
#include "../include/StreamManager.h"
#include "../include/DataVertex.h"
#include "../include/VertexElement.h"
#include "../include/VertexDeclaration.h"
#include "../include/StreamVertexBinding.h"

namespace LostPeterEngine
{
     StreamTempBlended::StreamTempBlended()
		: m_pSrcPosStream(nullptr)
		, m_pSrcNormalStream(nullptr)
		, m_pDestPosStream(nullptr)
		, m_pDestNormalStream(nullptr)
		, m_bPosNormalShareStream(false)
		, m_nPosBindIndex(0)
		, m_nNormBindIndex(0)
		, m_bBindPos(false)
		, m_bBindNormals(false)
	{
		
	}

	StreamTempBlended::~StreamTempBlended()
	{	
		Destroy();
	}
	
	void StreamTempBlended::Destroy()
	{
		if (m_pDestPosStream)
		{
			m_pStreamManager->ReleaseStreamVertexCopy(m_pDestPosStream);
			m_pDestPosStream = nullptr;
		}

		if (m_pDestNormalStream)
		{
			m_pStreamManager->ReleaseStreamVertexCopy(m_pDestNormalStream);
			m_pDestNormalStream = nullptr;
		}
	}

	void StreamTempBlended::ExtractFrom(DataVertex* pSrcVD)
	{
		if (m_pDestPosStream)
		{
			m_pStreamManager->ReleaseStreamVertexCopy(m_pDestPosStream);
			m_pDestPosStream = nullptr;
		}

		if (m_pDestNormalStream)
		{
			m_pStreamManager->ReleaseStreamVertexCopy(m_pDestNormalStream);
			m_pDestNormalStream = nullptr;
		}

		VertexDeclaration* pDecl = pSrcVD->GetVertexDeclaration();
		StreamVertexBinding* pBinding = pSrcVD->GetVertexStreamBinding();
		const VertexElement* posElem = pDecl->FindVertexElementBySemantic(F_VertexElementSemantic_Position);
		const VertexElement* normElem = pDecl->FindVertexElementBySemantic(F_VertexElementSemantic_Normal);
		F_Assert(posElem && "StreamTempBlended::ExtractFrom: Positions are required !")

		m_nPosBindIndex	= posElem->GetSourceIndex();
		m_pSrcPosStream	= pBinding->GetStreamVertex(m_nPosBindIndex);

		if (!normElem)
		{
			m_bPosNormalShareStream = false;
			m_pStreamManager->DestroyStreamVertex(m_pSrcNormalStream);
			m_pSrcNormalStream = nullptr;
		}
		else
		{
			m_nNormBindIndex = normElem->GetSourceIndex();
			if (m_nNormBindIndex == m_nPosBindIndex)
			{
				m_bPosNormalShareStream = true;
				m_pStreamManager->DestroyStreamVertex(m_pSrcNormalStream);
				m_pSrcNormalStream = nullptr;
			}
			else
			{
				m_bPosNormalShareStream = false;
				m_pSrcNormalStream = pBinding->GetStreamVertex(m_nNormBindIndex);
			}
		}
	}

	void StreamTempBlended::CheckoutTempCopies(bool bPos /*= true*/, bool bNormals /*= true*/)
	{
		m_bBindPos = bPos;
		m_bBindNormals = bNormals;

		if (bPos && !m_pDestPosStream)
		{
			m_pDestPosStream = m_pStreamManager->AllocateStreamVertexCopy(m_pSrcPosStream, E_StreamRelease_Auto, this);
		}
		if (bNormals && !m_bPosNormalShareStream && m_pSrcNormalStream && !m_pDestNormalStream)
		{
			m_pDestNormalStream = m_pStreamManager->AllocateStreamVertexCopy(m_pSrcNormalStream, E_StreamRelease_Auto, this);
		}
	}

	void StreamTempBlended::BindTempCopies(DataVertex* pTargetVD, bool bSuppressHardwareUpload)
	{
		m_pDestPosStream->SuppressHardwareUpdate(bSuppressHardwareUpload);
		pTargetVD->GetVertexStreamBinding()->SetBinding(m_nPosBindIndex, m_pDestPosStream);
		if (m_bBindNormals && !m_bPosNormalShareStream && m_pDestNormalStream)
		{
			m_pDestNormalStream->SuppressHardwareUpdate(bSuppressHardwareUpload);
			pTargetVD->GetVertexStreamBinding()->SetBinding(m_nNormBindIndex, m_pDestNormalStream);
		}
	}

	bool StreamTempBlended::BuffersCheckedOut(bool bPos /*= true*/, bool bNormals /*= true*/) const
	{
		if (bPos || (bNormals && m_bPosNormalShareStream))
		{
			if (!m_pDestPosStream)
				return false;

			m_pStreamManager->TouchStreamVertexCopy(m_pDestPosStream);
		}

		if (bNormals && !m_bPosNormalShareStream)
		{
			if (!m_pDestNormalStream)
				return false;

			m_pStreamManager->TouchStreamVertexCopy(m_pDestNormalStream);
		}

		return true;
	}

	void StreamTempBlended::DeleteStream(Stream* pStream)
	{
		F_Assert((pStream == m_pDestPosStream || pStream == m_pDestNormalStream) && "StreamTempBlended::DeleteStream")

		if (pStream == m_pDestPosStream)
		{
			//m_pDestPosStream->DelRef();
			m_pDestPosStream = nullptr;
		}
		if (pStream == m_pDestNormalStream)
		{
			//m_pDestNormalStream->DelRef();
			m_pDestNormalStream = nullptr;
		}
	}

}; //LostPeterEngine
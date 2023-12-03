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

#ifndef _STREAM_TEMP_BLENDED_H_
#define _STREAM_TEMP_BLENDED_H_

#include "StreamTemp.h"

namespace LostPeterEngine
{
    class engineExport StreamTempBlended : public StreamTemp
	{
	public:
		StreamTempBlended();
		virtual ~StreamTempBlended();

	public:
	private:
		//PreBlended 
		StreamVertex* m_pSrcPosStream;
		StreamVertex* m_pSrcNormalStream;

		//PostBlended 
		StreamVertex* m_pDestPosStream;
		StreamVertex* m_pDestNormalStream;

		//Both positions and normals are contained in the same buffer
		bool m_bPosNormalShareStream;
		uint16 m_nPosBindIndex;
		uint16 m_nNormBindIndex;
		bool m_bBindPos;
		bool m_bBindNormals;

	public:
		void Destroy();

	public:
		void ExtractFrom(DataVertex* pSrcVD);

		void CheckoutTempCopies(bool bPos = true, bool bNormals = true);

		void BindTempCopies(DataVertex* pTargetVD, bool bSuppressHardwareUpload);

		bool BuffersCheckedOut(bool bPos = true, bool bNormals = true) const;

	public:
		virtual	void DeleteStream(Stream* pStream);
	};

}; //LostPeterEngine

#endif
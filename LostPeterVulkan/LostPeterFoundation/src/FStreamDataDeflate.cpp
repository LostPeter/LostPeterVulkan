/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FStreamDataDeflate.h"
#include "../include/FStreamDataFileStream.h"
#include "zlib.h"

namespace LostPeterFoundation
{
    static void* F_Zip_Alloc(void* opaque, unsigned int items, unsigned int size)
	{
		return malloc(items*size);
	}

	static void F_Zip_Free(void* opaque, void* address)
	{
		free(address);
	}

	FStreamDataDeflate::FStreamDataDeflate(FStreamData* pStreamCompressed)
		: FStreamData(pStreamCompressed->GetAccessMode())
		, m_pStreamCompressed(pStreamCompressed)
		, m_pStreamZip(0)
		, m_nCurrentPos(0)
		, m_pTemp(0)
		, m_bIsCompressedValid(true)
	{
		init();
	}

	FStreamDataDeflate::FStreamDataDeflate(const String& strName, FStreamData* pStreamCompressed)
		: FStreamData(strName,pStreamCompressed->GetAccessMode())
		, m_pStreamCompressed(pStreamCompressed)
		, m_pStreamZip(0)
		, m_nCurrentPos(0)
		, m_pTemp(0)
		, m_bIsCompressedValid(true)
	{	
		init();
	}

	FStreamDataDeflate::~FStreamDataDeflate()
	{	
		Close();
		destroy();
	}

    void FStreamDataDeflate::destroy()
	{
		if (GetAccessMode() == F_StreamAccess_Read)
			inflateEnd(m_pStreamZip);

		delete m_pStreamZip;
		m_pStreamZip = nullptr;
		delete m_pTemp;
		m_pTemp = nullptr;
	}
	void FStreamDataDeflate::init()
	{
		m_pStreamZip = new z_stream;
		m_pStreamZip->zalloc = F_Zip_Alloc;
		m_pStreamZip->zfree = F_Zip_Free;

		if (GetAccessMode() == F_StreamAccess_Read)
		{
			m_pTemp = new uint8[F_C_DEFLATE_SIZE_TEMP];
			size_t restorePoint = m_pStreamCompressed->Tell();
			m_pStreamZip->next_in = m_pTemp;
			m_pStreamZip->avail_in = (uInt)m_pStreamCompressed->Read(m_pTemp, F_C_DEFLATE_SIZE_TEMP);

			if (inflateInit(m_pStreamZip) != Z_OK)
			{
				m_bIsCompressedValid = false;
			}
			else
			{
                m_bIsCompressedValid = true;
            }

			if (m_bIsCompressedValid)
			{
				Bytef testOut[4];
				size_t savedIn = m_pStreamZip->avail_in;
				m_pStreamZip->avail_out = 4;
				m_pStreamZip->next_out = testOut;
				if (inflate(m_pStreamZip, Z_SYNC_FLUSH) != Z_OK)
					m_bIsCompressedValid = false;
				m_pStreamZip->avail_in = (uInt)savedIn;
				m_pStreamZip->next_in  = m_pTemp;

				inflateReset(m_pStreamZip);
			}

			if (!m_bIsCompressedValid)
			{
				destroy();
				m_pStreamCompressed->Seek(restorePoint);
			}				
		}
		else 
		{
			char tmpname[L_tmpnam];
			tmpnam(tmpname);
			m_strTempFileName = tmpname;
			std::fstream* pStream = new std::fstream;
			pStream->open(tmpname,std::ios::binary | std::ios::out);
			m_pStreamWriteTemp = new FStreamDataFileStream(pStream);
		}
	}

	void FStreamDataDeflate::compressFinal()
	{
		m_pStreamWriteTemp->Close();

		int ret, flush;
		char in[F_C_DEFLATE_SIZE_TEMP];
		char out[F_C_DEFLATE_SIZE_TEMP];
		if (deflateInit(m_pStreamZip, Z_DEFAULT_COMPRESSION) != Z_OK)
		{
			destroy();

            F_LogError("*********************** FStreamDataDeflate::compressFinal: Error initialising deflate compressed stream !");
            return;
		}

		std::ifstream inFile;
		inFile.open(m_strTempFileName.c_str(), std::ios::in | std::ios::binary);

		do 
		{
			inFile.read(in, F_C_DEFLATE_SIZE_TEMP);
			m_pStreamZip->avail_in = inFile.gcount();
			if (inFile.bad()) 
			{
				deflateEnd(m_pStreamZip);

                F_LogError("*********************** FStreamDataDeflate::compressFinal: Error reading temp uncompressed stream !");
                return;
			}
			flush = inFile.eof() ? Z_FINISH : Z_NO_FLUSH;
			m_pStreamZip->next_in = (Bytef*)in;

			do 
			{
				m_pStreamZip->avail_out = F_C_DEFLATE_SIZE_TEMP;
				m_pStreamZip->next_out = (Bytef*)out;
				ret = deflate(m_pStreamZip, flush);    
				F_Assert("FStreamDataDeflate::compressFinal" && ret != Z_STREAM_ERROR)  
				size_t compressed = F_C_DEFLATE_SIZE_TEMP - m_pStreamZip->avail_out;
				m_pStreamCompressed->Write(out,compressed);
			} while (m_pStreamZip->avail_out == 0);
			F_Assert("FStreamDataDeflate::compressFinal" && m_pStreamZip->avail_in == 0)

		} while (flush != Z_FINISH);
		F_Assert("FStreamDataDeflate::compressFinal" && ret == Z_STREAM_END)

		deflateEnd(m_pStreamZip);
		remove(m_strTempFileName.c_str());
	}

	size_t FStreamDataDeflate::Read(void* pBuffer, size_t nCount)
	{
		if (!m_bIsCompressedValid)
		{
			return m_pStreamCompressed->Read(pBuffer, nCount);
		}

		if (GetAccessMode() & F_StreamAccess_Write)
		{
			return m_pStreamWriteTemp->Read(pBuffer, nCount);
		}
		else 
		{
			size_t restorePoint = m_pStreamCompressed->Tell();
			size_t cachereads = m_ReadCache.Read(pBuffer, nCount);
			size_t newReadUncompressed = 0;
			if (cachereads < nCount)
			{
				m_pStreamZip->avail_out = (uInt)(nCount - cachereads);
				m_pStreamZip->next_out = (Bytef*)pBuffer + cachereads;

				while (m_pStreamZip->avail_out)
				{
					if (!m_pStreamZip->avail_in && !m_pStreamCompressed->Eof())
					{
						m_pStreamZip->avail_in = (uInt)m_pStreamCompressed->Read(m_pTemp, F_C_DEFLATE_SIZE_TEMP);
						m_pStreamZip->next_in  = m_pTemp;
					}

					if (m_pStreamZip->avail_in)
					{
						int availpre = m_pStreamZip->avail_out;
						int status = inflate(m_pStreamZip, Z_SYNC_FLUSH);
						size_t readUncompressed = availpre - m_pStreamZip->avail_out;
						newReadUncompressed += readUncompressed;
						if (status != Z_OK)
						{
							if (status != Z_STREAM_END)
							{
								m_pStreamCompressed->Seek(restorePoint);

                                F_LogError("*********************** FStreamDataDeflate::Read: Error in compressed stream !");
                                return 0;
							}
							else 
							{												
								long unusedCompressed = m_pStreamZip->avail_in;
								m_pStreamCompressed->Skip(-unusedCompressed);
							}

							break;
						}
					}
				}
			}

			m_ReadCache.CacheData((char*)pBuffer + cachereads,newReadUncompressed);
			m_nCurrentPos += newReadUncompressed + cachereads;
			return newReadUncompressed + cachereads;
		}
	}
	
	size_t FStreamDataDeflate::Write(const void* pBuffer, size_t nCount)
	{
		if ((GetAccessMode() & F_StreamAccess_Write) == 0)
		{
            F_LogError("*********************** FStreamDataDeflate::Write: Not a writable stream !");
            return 0;
		}
		return m_pStreamWriteTemp->Write(pBuffer, nCount);
	}
	
	void FStreamDataDeflate::Skip(long nCount)
	{
		if (!m_bIsCompressedValid)
		{
			m_pStreamCompressed->Skip(nCount);
			return;
		}

		if (GetAccessMode() & F_StreamAccess_Write)
		{
			m_pStreamWriteTemp->Skip(nCount);
		}
		else 
		{
			if (nCount > 0)
			{
				if (!m_ReadCache.Forward(nCount))
				{
                    F_LogError("*********************** FStreamDataDeflate::Skip: 1, Can only skip within the cache range in a deflate stream !");
                    return;
				}
			}
			else if (nCount < 0)
			{
				if (!m_ReadCache.Rewind((size_t)(-nCount)))
				{
                    F_LogError("*********************** FStreamDataDeflate::Skip: 2, Can only skip within the cache range in a deflate stream !");
                    return;
				}
			}
		}		
		m_nCurrentPos = static_cast<size_t>(static_cast<long>(m_nCurrentPos) + nCount);
	}

	void FStreamDataDeflate::Seek(size_t nPos)
	{
		if (!m_bIsCompressedValid)
		{
			m_pStreamCompressed->Seek(nPos);
			return;
		}
		if (GetAccessMode() & F_StreamAccess_Write)
		{
			m_pStreamWriteTemp->Seek(nPos);
		}
		else
		{
			if (nPos == 0)
			{
				m_nCurrentPos = 0;
				m_pStreamZip->next_in = m_pTemp;
				m_pStreamCompressed->Seek(0);
				m_pStreamZip->avail_in = (uInt)m_pStreamCompressed->Read(m_pTemp, F_C_DEFLATE_SIZE_TEMP);			
				inflateReset(m_pStreamZip);
			}
			else 
			{
				Skip((long)(nPos - Tell()));
			}
		}		
	}
	
	size_t FStreamDataDeflate::Tell() const
	{
		if (!m_bIsCompressedValid)
		{
			return m_pStreamCompressed->Tell();
		}
		else if (GetAccessMode() & F_StreamAccess_Write) 
		{
			return m_pStreamWriteTemp->Tell();
		}
		else
		{
			return m_nCurrentPos;
		}
	}
	
	bool FStreamDataDeflate::Eof() const
	{
		if (GetAccessMode() & F_StreamAccess_Write)
			return m_pStreamWriteTemp->Eof();
		else 
		{
			if (!m_bIsCompressedValid)
				return m_pStreamCompressed->Eof();
			else
				return m_pStreamCompressed->Eof() && m_pStreamZip->avail_in == 0;
		}
	}
	
	void FStreamDataDeflate::Close()
	{
		if (GetAccessMode() & F_StreamAccess_Write)
		{
			compressFinal();
		}
	}

}; //LostPeterFoundation
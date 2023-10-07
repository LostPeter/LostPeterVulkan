/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FFileBinary.h"

namespace LostPeterFoundation
{
    static const uint16 s_EndianLittleFlag = 0x1000;
	static const uint16 s_EndianBigFlag    = 0x0010;

	const size_t FFileBinary::ms_nTempBufSize = 16*1024;
	FFileBinary::FFileBinary()
		: m_strFileType("")
		, m_nVersion(0)
		, m_eEndianType_Current(Endian_Little)
		, m_eEndianType_Export(Endian_Little)
		, m_bFlipEndian(false)
		, m_pFile(0)
		, m_nTotalSize(0)
	{
		m_pBufTemp = new uint8[ms_nTempBufSize];
	}

	FFileBinary::~FFileBinary()
	{
        F_DELETE_T(m_pBufTemp)
		Destroy();
	}

	void FFileBinary::Destroy()
	{
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = 0;
		}
	}

	bool FFileBinary::CreateBinary(const char* szFilePath)
	{
		Destroy();
		m_pFile = fopen(szFilePath, "wb");
		if (!m_pFile)
			return false;
		
		return true;
	}

	bool FFileBinary::OpenBinary(const char* szFilePath)
	{
		Destroy();
		m_pFile = fopen(szFilePath, "rb");
		if (!m_pFile)
			return false;

		//total size
		{
			long pos = ftell(m_pFile);
			fseek(m_pFile,0,SEEK_END);
			m_nTotalSize = ftell(m_pFile);
			fseek(m_pFile,pos,SEEK_SET);
		}
		ReadFileHeader();
		return true;
	}

	bool FFileBinary::SaveBinary()
	{
		if (!m_pFile)
			return false;

		fclose(m_pFile);
		return true;
	}
	
	size_t FFileBinary::GetFileHeaderSize()
	{
		return sizeof(uint16) + m_strFileType.size() + sizeof(uint32);
	}

	size_t FFileBinary::GetTotalFileSize()
	{
		return m_nTotalSize;
	}

	size_t FFileBinary::GetFileDataSize()
	{
		return m_nTotalSize - GetFileHeaderSize();
	}

	void FFileBinary::WriteFileHeader()
	{
		uint16 nVal = m_eEndianType_Export==Endian_Little?s_EndianLittleFlag:s_EndianBigFlag;
		WriteShorts(&nVal,1);
		WriteString(m_strFileType);
		WriteInts(&m_nVersion,1);
	}

	void FFileBinary::WriteBools(const bool* const pBools, size_t nCount)
	{
		if (m_bFlipEndian && nCount > 1)
		{
			bool* pBoolToWrite = 0;
			bool bDel = false;
			if (sizeof(bool)*nCount > ms_nTempBufSize)
			{
				pBoolToWrite = new bool[nCount];
				bDel = true;
			}
			else
			{
				pBoolToWrite = (bool*)m_pBufTemp;
			}

			memcpy(pBoolToWrite,pBools,sizeof(bool)*nCount);
			flipEndian(pBoolToWrite,sizeof(bool),nCount);
			WriteData(pBoolToWrite,sizeof(bool),nCount);

			if (bDel)
			{
				delete[] pBoolToWrite;
			}
		}
		else
		{
			WriteData(pBools, sizeof(bool), nCount);
		}
	}

	void FFileBinary::WriteBytes(const uint8* const pBytes, size_t nCount)
	{
		if (m_bFlipEndian && nCount > 1)
		{
			uint8* pByteToWrite = 0;
			uint8 bDel = false;
			if (sizeof(uint8)*nCount > ms_nTempBufSize)
			{
				pByteToWrite = new uint8[nCount];
				bDel = true;
			}
			else
			{
				pByteToWrite = (uint8*)m_pBufTemp;
			}

			memcpy(pByteToWrite,pBytes,sizeof(uint8)*nCount);
			flipEndian(pByteToWrite,sizeof(uint8),nCount);
			WriteData(pByteToWrite,sizeof(uint8),nCount);

			if (bDel)
			{
				delete[] pByteToWrite;
			}
		}
		else
		{
			WriteData(pBytes,sizeof(uint8),nCount);
		}
	}

	void FFileBinary::WriteShorts(const uint16* const pShorts, size_t nCount)
	{
		if (m_bFlipEndian)
		{
			uint16* pShortToWrite = 0;
			bool bDel = false;
			if (sizeof(uint16)*nCount > ms_nTempBufSize)
			{
				pShortToWrite = new uint16[nCount];
				bDel = true;
			}
			else
			{
				pShortToWrite = (uint16*)m_pBufTemp;
			}

			memcpy(pShortToWrite,pShorts,sizeof(uint16)*nCount);
			flipEndian(pShortToWrite,sizeof(uint16),nCount);
			WriteData(pShortToWrite,sizeof(uint16),nCount);

			if (bDel)
			{
				delete[] pShortToWrite;
			}
		}
		else
		{
			WriteData(pShorts, sizeof(uint16), nCount);
		}
	}

	void FFileBinary::WriteInts(const uint32* const pInts, size_t nCount)
	{
		if (m_bFlipEndian)
		{
			uint32* pIntToWrite = 0;
			bool bDel = false;
			if (sizeof(uint32)*nCount > ms_nTempBufSize)
			{
				pIntToWrite = new uint32[nCount];
				bDel = true;
			}
			else
			{
				pIntToWrite = (uint32*)m_pBufTemp;
			}

			memcpy(pIntToWrite,pInts,sizeof(uint32)*nCount);
			flipEndian(pIntToWrite,sizeof(uint32),nCount);
			WriteData(pIntToWrite,sizeof(uint32),nCount);

			if (bDel)
			{
				delete[] pIntToWrite;
			}
		}
		else
		{
			WriteData(pInts,sizeof(uint32),nCount);
		}
	}

	void FFileBinary::WriteFloats(const float* const pFloats, size_t nCount)
	{
		if (m_bFlipEndian)
		{
			float* pFloatToWrite = 0;
			bool bDel = false;
			if (sizeof(float)*nCount > ms_nTempBufSize)
			{
				pFloatToWrite = new float[nCount];
				bDel = true;
			}
			else
			{
				pFloatToWrite = (float*)m_pBufTemp;
			}

			memcpy(pFloatToWrite,pFloats,sizeof(float)*nCount);
			flipEndian(pFloatToWrite,sizeof(float),nCount);
			WriteData(pFloatToWrite,sizeof(float),nCount);

			if (bDel)
			{
				delete[] pFloatToWrite;
			}
		}
		else
		{
			WriteData(pFloats,sizeof(float),nCount);
		}
	}

	void FFileBinary::WriteDoubles(const double* const pDoubles, size_t nCount)
	{
		if (m_bFlipEndian)
		{
			double* pDoubleToWrite = 0;
			bool bDel = false;
			if (sizeof(double)*nCount > ms_nTempBufSize)
			{
				pDoubleToWrite = new double[nCount];
				bDel = true;
			}
			else
			{
				pDoubleToWrite = (double*)m_pBufTemp;
			}

			memcpy(pDoubleToWrite,pDoubles,sizeof(double)*nCount);
			flipEndian(pDoubleToWrite,sizeof(double),nCount);
			WriteData(pDoubleToWrite,sizeof(double),nCount);

			if (bDel)
			{
				delete[] pDoubleToWrite;
			}
		}
		else
		{
			WriteData(pDoubles,sizeof(double),nCount);
		}
	}

	void FFileBinary::WriteString(const String& str)
	{
		F_Assert(m_pFile && "FFileBinary::WriteString")
		fputs(str.c_str(),m_pFile);
		fputc('\0',m_pFile);
	}

	void FFileBinary::WriteData(const void* const pBuf, size_t nSize, size_t nCount)
	{
		F_Assert(m_pFile && "FFileBinary::WriteData")
		fwrite((void* const)pBuf,nSize,nCount,m_pFile);
	}

	void FFileBinary::ReadFileHeader()
	{
		uint16 nVal;
		ReadShorts(&nVal, 1);
		if (nVal == s_EndianLittleFlag)
			m_eEndianType_Export = Endian_Little;
		else if (nVal == s_EndianBigFlag)
			m_eEndianType_Export = Endian_Big;
		else
		{
			F_Assert(false && "FFileBinary::ReadFileHeader: Unknown file Type!")
		}
		if (m_eEndianType_Current != m_eEndianType_Export)
			m_bFlipEndian = true;

		ReadString(m_strFileType);
		ReadInts(&m_nVersion,1);
	}

	void FFileBinary::ReadBools(bool* pDest, size_t nCount)
	{
		ReadData(pDest, sizeof(bool)*nCount);
		if (m_bFlipEndian && nCount > 1)
		{
			flipEndian(pDest, sizeof(bool)*nCount);
		}
	}

	void FFileBinary::ReadBytes(uint8* pDest, size_t nCount)
	{
		ReadData(pDest, sizeof(uint8)*nCount);
		if (m_bFlipEndian && nCount > 1)
		{
			flipEndian(pDest, sizeof(uint8)*nCount);
		}
	}

	void FFileBinary::ReadShorts(uint16* pDest, size_t nCount)
	{
		ReadData(pDest, sizeof(uint16)*nCount);
		if (m_bFlipEndian)
		{
			flipEndian(pDest, sizeof(uint16)*nCount);
		}
	}

	void FFileBinary::ReadInts(uint32* pDest, size_t nCount)
	{
		ReadData(pDest, sizeof(uint32)*nCount);
		if (m_bFlipEndian)
		{
			flipEndian(pDest, sizeof(uint32)*nCount);
		}
	}

	void FFileBinary::ReadFloats(float* pDest, size_t nCount)
	{
		ReadData(pDest, sizeof(float)*nCount);
		if (m_bFlipEndian)
		{
			flipEndian(pDest, sizeof(float)*nCount);
		}
	}

	void FFileBinary::ReadDoubles(double* pDest, size_t nCount)
	{
		ReadData(pDest, sizeof(double)*nCount);
		if (m_bFlipEndian)
		{
			flipEndian(pDest, sizeof(double)*nCount);
		}
	}

	void FFileBinary::ReadString(String& str)
	{
		F_Assert(m_pFile && "FFileBinary::ReadString")
		char c = (char)getc(m_pFile);
		while (c != '\0')
		{
			str.push_back(c);
			c = (char)getc(m_pFile);
		}
	}

	void FFileBinary::ReadData(void* pBuf, size_t nSize)
	{
		F_Assert(m_pFile && "FFileBinary::ReadData")
		fread(pBuf,1,nSize,m_pFile);
	}

	void FFileBinary::flipEndian(void* pData, size_t nSize, size_t nCount)
	{
		for (size_t index = 0; index < nCount; index++)
		{
			flipEndian((void*)((long)pData + (index * nSize)), nSize);
		}
	}

	void FFileBinary::flipEndian(void* pData, size_t nSize)
	{
		char swapByte;
		for (size_t byteIndex = 0; byteIndex < nSize / 2; byteIndex++)
		{
			swapByte = *(char *)((long)pData + byteIndex);
			*(char *)((long)pData + byteIndex) = *(char *)((long)pData + nSize - byteIndex - 1);
			*(char *)((long)pData + nSize - byteIndex - 1) = swapByte;
		}
	}
    
}; //LostPeterFoundation
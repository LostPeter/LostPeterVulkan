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

#ifndef _F_FILE_BINARY_H_
#define _F_FILE_BINARY_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FFileBinary
	{
	public:
		FFileBinary();
		~FFileBinary();

	public:
		enum EndianType
		{
			Endian_Little = 0,	//use little endian (0x1000 is serialized as 0x00 0x10)
			Endian_Big,			//use big endian (0x1000 is serialized as 0x10 0x00)
		};
		
		static const size_t ms_nTempBufSize;

	protected:
		String m_strFileType;
		uint32 m_nVersion;
		EndianType m_eEndianType_Current;
		EndianType m_eEndianType_Export;
		bool m_bFlipEndian;
		FILE* m_pFile;
		size_t m_nTotalSize;

	private:
		uint8* m_pBufTemp;

	public:
		LP_FORCEINLINE const String& GetFileType() const { return m_strFileType; }
		LP_FORCEINLINE void SetFileType(const String& str) { m_strFileType=str; }
		LP_FORCEINLINE uint32 GetVersion() const { return m_nVersion; }
		LP_FORCEINLINE void SetVersion(uint32 n) { m_nVersion=n; }
		LP_FORCEINLINE void SetEndianType_Current(EndianType e) { m_eEndianType_Current=e; }
		LP_FORCEINLINE EndianType GetEndianType_Export() const { return m_eEndianType_Export; }
		LP_FORCEINLINE void SetEndianType_Export(EndianType e)
		{
			m_eEndianType_Export=e;
			m_bFlipEndian = (m_eEndianType_Current==m_eEndianType_Export)?false:true;
		}
		LP_FORCEINLINE bool GetFlipEndian() const { return m_bFlipEndian; }

	public:
		void Destroy();

		bool CreateBinary(const char* szFilePath);
		bool OpenBinary(const char* szFilePath);
		bool SaveBinary();
		
		size_t GetFileHeaderSize();
		size_t GetTotalFileSize();
		size_t GetFileDataSize();

	public:
		void WriteFileHeader();
			
		void WriteBools(const bool* const pBools, size_t nCount);
		void WriteBytes(const uint8* const pBytes, size_t nCount);
		void WriteShorts(const uint16* const pShorts, size_t nCount);
		void WriteInts(const uint32* const pInts, size_t nCount); 
		void WriteFloats(const float* const pFloats, size_t nCount);
		void WriteDoubles(const double* const pDoubles, size_t nCount);
		void WriteString(const String& str);
		void WriteData(const void* const pBuf, size_t nSize, size_t nCount);
		

		void ReadFileHeader();

		void ReadBools(bool* pDest, size_t nCount);
		void ReadBytes(uint8* pDest, size_t nCount);
		void ReadShorts(uint16* pDest, size_t nCount);
		void ReadInts(uint32* pDest, size_t nCount);
		void ReadFloats(float* pDest, size_t nCount);
		void ReadDoubles(double* pDest, size_t nCount);
		void ReadString(String& str);
		void ReadData(void* pBuf, size_t nSize);

	protected:
		void flipEndian(void* pData, size_t nSize, size_t nCount);
		void flipEndian(void* pData, size_t nSize);
	};

}; //LostPeterFoundation

#endif
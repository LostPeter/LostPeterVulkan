/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_BITWISE_H_
#define _F_BITWISE_H_

#include "FPreDefine.h"

#ifndef __has_builtin
    // Compatibility with non-clang compilers
    #define __has_builtin(x) 0
#endif

namespace LostPeterFoundation
{
    class LPF_Export FBitwise
    {
    public:
        static LP_FORCEINLINE uint16 Swap16(uint16 arg)
        {
#if LP_COMPILER == LP_COMPILER_MSVC && LP_COMPILER_VERSION >= 1310
            return _byteswap_ushort(arg);
#elif (LP_COMPILER == LP_COMPILER_CLANG && __has_builtin(__builtin_bswap16)) || (LP_COMPILER == LP_COMPILER_GNUC && LP_COMPILER_VERSION >= 480)
            return __builtin_bswap16(arg);
#else
            return ((arg << 8) & 0xFF00) | ((arg >> 8) & 0x00FF);
#endif
        }
      
        static LP_FORCEINLINE uint32 Swap32(uint32 arg)
        {
#if LP_COMPILER == LP_COMPILER_MSVC && LP_COMPILER_VERSION >= 1310
            return _byteswap_ulong(arg);
#elif (LP_COMPILER == LP_COMPILER_CLANG && __has_builtin(__builtin_bswap32)) || (LP_COMPILER == LP_COMPILER_GNUC && LP_COMPILER_VERSION >= 430)
            return __builtin_bswap32(arg);
#else
            return ((arg & 0x000000FF) << 24) | ((arg & 0x0000FF00) << 8) | ((arg >> 8) & 0x0000FF00) | ((arg >> 24) & 0x000000FF);
#endif
        }
      
        static LP_FORCEINLINE uint64 Swap64(uint64 arg)
        {
#if LP_COMPILER == LP_COMPILER_MSVC && LP_COMPILER_VERSION >= 1310
            return _byteswap_uint64(arg);
#elif (LP_COMPILER == LP_COMPILER_CLANG && __has_builtin(__builtin_bswap64)) || (LP_COMPILER == LP_COMPILER_GNUC && LP_COMPILER_VERSION >= 430)
            return __builtin_bswap64(arg);
#else
            union { 
                uint64 sv;
                uint32 ul[2];
            } tmp, result;
            tmp.sv = arg;
            result.ul[0] = Swap32(tmp.ul[1]);
            result.ul[1] = Swap32(tmp.ul[0]);
            return result.sv; 
#endif
        }

        static LP_FORCEINLINE void SwapBuffer(void * pData, size_t size)
        {
            char swapByte;
            for (char *p0 = (char*)pData, *p1 = p0 + size - 1; p0 < p1; ++p0, --p1)
            {
                swapByte = *p0;
                *p0 = *p1;
                *p1 = swapByte;
            }
        }

        static LP_FORCEINLINE void SwapChunks(void* pData, size_t size, size_t count)
        {
            for (size_t c = 0; c < count; ++c)
            {
                char swapByte;
                for (char *p0 = (char*)pData + c * size, *p1 = p0 + size - 1; p0 < p1; ++p0, --p1)
                {
                    swapByte = *p0;
                    *p0 = *p1;
                    *p1 = swapByte;
                }
            }
        }

		static uint32 MostSignificantBitSet(uint32 nValue);								
		static uint32 FirstPow2From(uint32 n);					

		template<typename T>																											
		static LP_FORCEINLINE bool IsPow2(T n)		
		{
			return (n & (n-1)) == 0;
		}
							
		template<typename T>													
		static LP_FORCEINLINE uint32 GetBitShift(T mask)								
		{
			if (mask == 0)
				return 0;

			uint32 result = 0;
			while ((mask & 1) == 0) 
			{
				++result;
				mask >>= 1;
			}
			return result;
		}

		template<typename SrcT,typename DestT>																						//			
		static LP_FORCEINLINE DestT	ConvertBitPattern(SrcT srcValue, SrcT srcBitMask, DestT destBitMask)
		{
			// Mask off irrelevant source value bits (if any)
			srcValue = srcValue & srcBitMask;

			// Shift source down to bottom of DWORD
			const uint32 srcBitShift = GetBitShift(srcBitMask);
			srcValue >>= srcBitShift;

			// Get max value possible in source from srcMask
			const SrcT srcMax = srcBitMask >> srcBitShift;

			// Get max available in dest
			const uint32 destBitShift = GetBitShift(destBitMask);
			const DestT destMax = destBitMask >> destBitShift;

			// Scale source value into destination, and shift back
			DestT destValue = (srcValue * destMax) / srcMax;

			return (destValue << destBitShift);
		}

		static uint32 FixedToFixed(uint32 value, uint32 n, uint32 p);		
		static uint32 FloatToFixed(const float value, const uint32 bits);				
		static float FixedToFloat(unsigned value, uint32 bits);							
		static void	IntWrite(void* dest, const int32 n, const uint32 value);			
		static uint32 IntRead(const void* src, int32 n);										

		static uint16 FloatToHalf(float i);													
		static uint16 FloatToHalfI(uint32 i);											

		static float HalfToFloat(uint16 y);										
		static uint32 HalfToFloatI(uint16 y);
    };

}; //LostPeterFoundation

#endif
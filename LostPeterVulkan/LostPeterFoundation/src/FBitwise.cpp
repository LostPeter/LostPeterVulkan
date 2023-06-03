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

#include "../include/FBitwise.h"

namespace LostPeterFoundation
{
    uint32 FBitwise::MostSignificantBitSet(uint32 nValue)
	{
		uint32 result = 0;	
		while (nValue != 0) 
		{
			++result;
			nValue >>= 1;
		}
		return result-1;
	}

	uint32 FBitwise::FirstPow2From(uint32 n)									
	{
		--n;            
		n |= n >> 16;
		n |= n >> 8;
		n |= n >> 4;	
		n |= n >> 2;
		n |= n >> 1;
		++n;

		return n;
	}

	uint32 FBitwise::FixedToFixed(uint32 value, uint32 n, uint32 p)
	{
		if(n > p) 
		{
			// Less bits required than available; this is easy
			value >>= n-p;
		} 
		else if(n < p)
		{
			// More bits required than are there, do the fill
			// Use old fashioned division, probably better than a loop
			if(value == 0)
				value = 0;
			else if(value == (static_cast<uint32>(1)<<n)-1)
				value = (1<<p)-1;
			else   
				value = value*(1<<p)/((1<<n)-1);
		}
		return value; 
	}

	uint32 FBitwise::FloatToFixed(const float value, const uint32 bits)	
	{
		if(value <= 0.0f) 
			return 0;
		else if (value >= 1.0f) 
			return (1<<bits)-1;
		else 
			return (uint32)(value * (1<<bits));   
	}

	float FBitwise::FixedToFloat(unsigned value, uint32 bits)				
	{
		return (float)value/(float)((1<<bits)-1);
	}

	void FBitwise::IntWrite(void* dest, const int32 n, const uint32 value)	
	{
		switch(n) 
		{
		case 1:
			((uint8*)dest)[0] = (uint8)value;
			break;
		case 2:
			((uint16*)dest)[0] = (uint16)value;
			break;
		case 3:
#if LP_ENDIAN == LP_ENDIAN_BIG
			((uint8*)dest)[0] = (uint8)((value >> 16) & 0xFF);
			((uint8*)dest)[1] = (uint8)((value >> 8) & 0xFF);
			((uint8*)dest)[2] = (uint8)(value & 0xFF);
#else
			((uint8*)dest)[2] = (uint8)((value >> 16) & 0xFF);
			((uint8*)dest)[1] = (uint8)((value >> 8) & 0xFF);
			((uint8*)dest)[0] = (uint8)(value & 0xFF);
#endif
			break;
		case 4:
			((uint32*)dest)[0] = (uint32)value;                
			break;                
		}        
	}

	uint32 FBitwise::IntRead(const void* src,int32 n)							
	{
		switch(n) 
		{
		case 1:
			return ((uint8*)src)[0];
		case 2:
			return ((uint16*)src)[0];
		case 3:
#if LP_ENDIAN == LP_ENDIAN_BIG      
			return ((uint32)((uint8*)src)[0]<<16) |
				((uint32)((uint8*)src)[1]<<8) |
				((uint32)((uint8*)src)[2]);
#else
			return ((uint32)((uint8*)src)[0]) |
				((uint32)((uint8*)src)[1]<<8) |
				((uint32)((uint8*)src)[2]<<16);
#endif
		case 4:
			return ((uint32*)src)[0];
		} 
		return 0; 
	}

	uint16 FBitwise::FloatToHalf(float i)									
	{
		union { float f; uint32 i; } v;
		v.f = i;
		return FloatToHalfI(v.i);
	}

	uint16 FBitwise::FloatToHalfI(uint32 i)							
	{
		int32 s =  (i >> 16) & 0x00008000;
		int32 e =  ((i >> 23) & 0x000000ff) - (127 - 15);
		int32 m =  i & 0x007fffff;

		if (e <= 0)
		{
			if (e < -10)
			{
				return 0;
			}
			m = (m | 0x00800000) >> (1 - e);

			return uint16(s | (m >> 13));
		}
		else if (e == 0xff - (127 - 15))
		{
			if (m == 0)		// Inf
			{
				return uint16(s | 0x7c00);
			} 
			else			// NAN
			{
				m >>= 13;
				return uint16(s | 0x7c00 | m | (m == 0));
			}
		}
		else
		{
			if (e > 30)		// Overflow
			{
				return uint16(s | 0x7c00);
			}

			return uint16(s | (e << 10) | (m >> 13));
		}
	}

	float FBitwise::HalfToFloat(uint16 y)								
	{
		union { float f; uint32 i; } v;
		v.i = HalfToFloatI(y);
		return v.f;
	}

	uint32 FBitwise::HalfToFloatI(uint16 y)				
	{
		int32 s = (y >> 15) & 0x00000001;
		int32 e = (y >> 10) & 0x0000001f;
		int32 m =  y        & 0x000003ff;

		if (e == 0)
		{
			if (m == 0)				// Plus or minus zero
			{
				return s << 31;
			}
			else					// Denormalized number -- renormalize it
			{
				while (!(m & 0x00000400))
				{
					m <<= 1;
					e -=  1;
				}

				e += 1;
				m &= ~0x00000400;
			}
		}
		else if (e == 31)
		{
			if (m == 0)				// Inf
			{
				return (s << 31) | 0x7f800000;
			}
			else					// NaN
			{
				return (s << 31) | 0x7f800000 | (m << 13);
			}
		}

		e = e + (127 - 15);
		m = m << 13;

		return (s << 31) | (e << 23) | m;
	}

}; //LostPeterFoundation
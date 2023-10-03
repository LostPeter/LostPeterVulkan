/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_COMPLEX_H_
#define _F_COMPLEX_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FComplex
    {
    public:
        FComplex();
        FComplex(double real);
        FComplex(double real, double imaginary);
        ~FComplex() 
        {

        }

    public:
        static FComplex I;      //0.0 + 1.0i
        static FComplex NaN;    //NaN + NaNi
        static FComplex INF;    //+INF + INFi
        static FComplex ONE;    //1.0 + 0.0i
        static FComplex ZERO;   //0.0 + 0.0i

        static FComplex CreateFComplex(const double realPart, const double imaginaryPart);
        static bool Equals(const FComplex& x, const FComplex& y, double eps);

    public:
        double real;
        double imaginary;
        bool IsNaN;         //is equal to NaN
        bool IsInfinite;    //is infinite

    public:
        LP_FORCEINLINE double GetImaginary() const { return imaginary; }
        LP_FORCEINLINE double GetReal() const { return real; }

    public:
        String ToString();

        FComplex operator= (const FComplex& src);
        FComplex operator+ (const FComplex& addend);
        FComplex operator+ (double addend);  
        FComplex operator- ();
        FComplex operator- (const FComplex& subtrahend);
        FComplex operator- (double subtrahend);
        FComplex operator* (const FComplex& factor);
        FComplex operator* (int factor);
        FComplex operator* (double factor);
        FComplex operator/ (const FComplex& divisor);
        FComplex operator/ (double divisor);
        
    public:
        double Abs();
        FComplex Conjugate();
        FComplex Reciprocal();
        bool Equals(const FComplex& c) const;

        FComplex Log();
        FComplex ACos();
        FComplex ASin();
        FComplex ATan();
        FComplex Cos();
        FComplex Cosh();
        FComplex Exp();
        FComplex Pow(const FComplex& x);
        FComplex Pow(double x);
        FComplex Sin();
        FComplex Sinh();
        FComplex Sqrt();
        FComplex Sqrt1z();
        FComplex Tan();
        FComplex Tanh();
        double GetArgument();
        FComplex NTHRoot(int n);
        std::vector<FComplex> NTHRoots(int n);
    };
    
}; //LostPeterFoundation

#endif
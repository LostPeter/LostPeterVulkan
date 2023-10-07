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

#include "../include/FComplex.h"
#include "../include/FUtilString.h"
#include "../include/FMath.h"

namespace LostPeterFoundation
{
    FComplex FComplex::I(0.0, 1.0);
    FComplex FComplex::NaN(NAN, NAN);
    FComplex FComplex::INF(INFINITY, INFINITY);
    FComplex FComplex::ONE(1.0, 0.0);
    FComplex FComplex::ZERO(0.0, 0.0);

    FComplex FComplex::CreateFComplex(const double realPart, const double imaginaryPart)
    {
        FComplex c(realPart, imaginaryPart);
        return c;
    }

    bool FComplex::Equals(const FComplex& x, const FComplex& y, double eps)
    {
        return fabs(x.real - y.real) <= eps && fabs(x.imaginary - y.imaginary) <= eps;
    }

    FComplex::FComplex() 
        : FComplex(0.0, 0.0)
    { 

    }

    FComplex::FComplex(double real) 
        : FComplex(real, 0.0)
    { 
        
    }

    FComplex::FComplex(double real, double imaginary)
    {
        this->real = real;
        this->imaginary = imaginary;

        IsNaN = isnan(real) || isnan(imaginary);
        IsInfinite = !IsNaN && (isinf(real) || isinf(imaginary));
    }

    String FComplex::ToString()
    {
        return "(" + FUtilString::SaveDouble(real) + ", " + FUtilString::SaveDouble(imaginary) + ")";
    }

    FComplex FComplex::operator= (const FComplex& src)
    {
        this->real = src.real;
        this->imaginary = src.imaginary;
        this->IsNaN = src.IsNaN;
        this->IsInfinite = src.IsInfinite;
        return *this;
    }

    FComplex FComplex::operator+ (const FComplex& addend)
    {
        if (IsNaN || addend.IsNaN)
        {
            return NaN;
        }
        return CreateFComplex(real + addend.GetReal(),
            imaginary + addend.GetImaginary());
    }
    FComplex FComplex::operator+ (double addend)
    {
        if (IsNaN || isnan(addend))
        {
            return NaN;
        }
        return CreateFComplex(real + addend, imaginary);
    }

    FComplex FComplex::operator- ()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return CreateFComplex(-real, -imaginary);
    }
    FComplex FComplex::operator- (const FComplex& subtrahend)
    {
        if (IsNaN || subtrahend.IsNaN)
        {
            return NaN;
        }
        return CreateFComplex(real - subtrahend.GetReal(),
            imaginary - subtrahend.GetImaginary());
    }    
    FComplex FComplex::operator- (double subtrahend)
    {
        if (IsNaN || isnan(subtrahend))
        {
            return NaN;
        }
        return CreateFComplex(real - subtrahend, imaginary);
    }

    FComplex FComplex::operator* (const FComplex& factor)
    {
        if (IsNaN || factor.IsNaN)
        {
            return NaN;
        }
        if (isinf(real) ||
            isinf(imaginary) ||
            isinf(factor.real) ||
            isinf(factor.imaginary))
        {
            return INF;
        }
        return CreateFComplex(real * factor.real - imaginary * factor.imaginary,
            real * factor.imaginary + imaginary * factor.real);
    }
    FComplex FComplex::operator* (int factor)
    {
        if (IsNaN)
        {
            return NaN;
        }
        if (isinf(real) ||
            isinf(imaginary))
        {
            return INF;
        }
        return CreateFComplex(real * factor, imaginary * factor);
    }
    FComplex FComplex::operator* (double factor)
    {
        if (IsNaN || isnan(factor))
        {
            return NaN;
        }
        if (isinf(real) ||
            isinf(imaginary) ||
            isinf(factor))
        {
            return INF;
        }
        return CreateFComplex(real * factor, imaginary * factor);
    }

    FComplex FComplex::operator/ (const FComplex& divisor)
    {
        if (IsNaN || divisor.IsNaN)
        {
            return NaN;
        }

        double c = divisor.GetReal();
        double d = divisor.GetImaginary();
        if (c == 0.0 && d == 0.0)
        {
            return NaN;
        }

        if (divisor.IsInfinite && !IsInfinite)
        {
            return ZERO;
        }

        if (fabs(c) < fabs(d))
        {
            double q = c / d;
            double denominator = c * q + d;
            return CreateFComplex((real * q + imaginary) / denominator,
                (imaginary * q - real) / denominator);
        }
        else
        {
            double q = d / c;
            double denominator = d * q + c;
            return CreateFComplex((imaginary * q + real) / denominator,
                (imaginary - real * q) / denominator);
        }
    }
    FComplex FComplex::operator/ (double divisor)
    {
        if (IsNaN || isnan(divisor))
        {
            return NaN;
        }
        if (divisor == 0)
        {
            return NaN;
        }
        if (isinf(divisor))
        {
            return !IsInfinite ? ZERO : NaN;
        }
        return CreateFComplex(real / divisor,
            imaginary / divisor);
    }


    double FComplex::Abs()
    {
        if (IsNaN)
        {
            return NAN;
        }
        if (IsInfinite)
        {
            return INFINITY;
        }
        if (fabs(real) < fabs(imaginary))
        {
            if (imaginary == 0.0)
            {
                return fabs(real);
            }
            double q = real / imaginary;
            return fabs(imaginary) * sqrt(1 + q * q);
        }
        else
        {
            if (real == 0.0)
            {
                return fabs(imaginary);
            }
            double q = imaginary / real;
            return fabs(real) * sqrt(1 + q * q);
        }
    }

    FComplex FComplex::Conjugate()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return CreateFComplex(real, -imaginary);
    }

    FComplex FComplex::Reciprocal()
    {
        if (IsNaN)
        {
            return NaN;
        }

        if (real == 0.0 && imaginary == 0.0)
        {
            return INF;
        }

        if (IsInfinite)
        {
            return ZERO;
        }

        if (fabs(real) < fabs(imaginary))
        {
            double q = real / imaginary;
            double scale = 1.0 / (real * q + imaginary);
            return CreateFComplex(scale * q, -scale);
        }
        else
        {
            double q = imaginary / real;
            double scale = 1.0 / (imaginary * q + real);
            return CreateFComplex(scale, -scale * q);
        }
    }

    bool FComplex::Equals(const FComplex& c) const
    {
        if (c.IsNaN)
        {
            return IsNaN;
        }
        else
        {
            return real == c.real &&
                imaginary == c.imaginary;
        }
    }

    FComplex FComplex::Log()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return CreateFComplex(log(Abs()),
            ::atan2(imaginary, real));
    }

    FComplex FComplex::ACos()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return - (((*this) + (*this).Sqrt1z() * I).Log())*I;
    }

    FComplex FComplex::ASin()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return (this->Sqrt1z() + ((*this)*I)).Log() * (-I);
    }

    FComplex FComplex::ATan()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return ((I + (*this)) / (I - (*this))).Log() * I *0.5;
    }
   
    FComplex FComplex::Cos()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return CreateFComplex(cos(real) * cosh(imaginary), -sin(real) * sinh(imaginary));
    }

    FComplex FComplex::Cosh()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return CreateFComplex(cosh(real) * cos(imaginary), sinh(real) * sin(imaginary));
    }

    FComplex FComplex::Exp()
    {
        if (IsNaN)
        {
            return NaN;
        }
        double expReal = exp(real);
        return CreateFComplex(expReal * cos(imaginary), expReal * sin(imaginary));
    }

    FComplex FComplex::Pow(const FComplex& x)
    {
        return (this->Log() * x).Exp();
    }

    FComplex FComplex::Pow(double x)
    {
        return (this->Log()* x).Exp();
    }

    FComplex FComplex::Sin()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return CreateFComplex(sin(real) * cosh(imaginary), cos(real) * sinh(imaginary));
    }

    FComplex FComplex::Sinh()
    {
        if (IsNaN)
        {
            return NaN;
        }
        return CreateFComplex(sinh(real) * cos(imaginary), cosh(real) * sin(imaginary));
    }

    FComplex FComplex::Sqrt()
    {
        if (IsNaN)
        {
            return NaN;
        }

        if (real == 0.0 && imaginary == 0.0)
        {
            return CreateFComplex(0.0, 0.0);
        }

        double t = sqrt((fabs(real) + Abs()) / 2.0);
        if (real >= 0.0)
        {
            return CreateFComplex(t, imaginary / (2.0 * t));
        }
        else
        {
            double signedT = imaginary < 0 ? -t : t;
            return CreateFComplex(fabs(imaginary) / (2.0 * t),
                signedT);
        }
    }

    FComplex FComplex::Sqrt1z()
    {
        return (ONE - ((*this) * (*this))).Sqrt();
    }
    
    FComplex FComplex::Tan()
    {
        if (IsNaN || isinf(real))
        {
            return NaN;
        }
        if (imaginary > 20.0)
        {
            return CreateFComplex(0.0, 1.0);
        }
        if (imaginary < -20.0)
        {
            return CreateFComplex(0.0, -1.0);
        }

        double real2 = 2.0 * real;
        double imaginary2 = 2.0 * imaginary;
        double d = cos(real2) + cosh(imaginary2);

        return CreateFComplex(sin(real2) / d, sinh(imaginary2) / d);
    }

    FComplex FComplex::Tanh()
    {
        if (IsNaN || isinf(imaginary))
        {
            return NaN;
        }
        if (real > 20.0)
        {
            return CreateFComplex(1.0, 0.0);
        }
        if (real < -20.0)
        {
            return CreateFComplex(-1.0, 0.0);
        }
        double real2 = 2.0 * real;
        double imaginary2 = 2.0 * imaginary;
        double d = cosh(real2) + cos(imaginary2);

        return CreateFComplex(sinh(real2) / d, sin(imaginary2) / d);
    }

    double FComplex::GetArgument()
    {
        return atan2(GetImaginary(), GetReal());
    }

    FComplex FComplex::NTHRoot(int n)
    {
        if (n <= 0)
        {
            throw "FComplex::NTHRoot: Can not compute nth root for negative n !";
        }
        if (IsNaN) return NaN;
        if (IsInfinite) return INF;

        // nth root of Abs -- faster / more accurate to use a solver here?
        double nthRootOfAbs = pow(Abs(), 1.0 / n);

        // Compute nth roots of FComplex number with k = 0, 1, ... n-1
        double nthPhi = GetArgument() / n;
        double innerPart = nthPhi;
        double realPart = nthRootOfAbs * cos(innerPart);
        double imaginaryPart = nthRootOfAbs * sin(innerPart);
        return CreateFComplex(realPart, imaginaryPart);
    }

    std::vector<FComplex> FComplex::NTHRoots(int n)
    {
        if (n <= 0)
        {
            throw "FComplex::NTHRoots: Can not compute nth root for negative n !";
        }
        std::vector<FComplex> result;

        if (IsNaN)
        {
            for (int k = 0; k < n; k++) result[k] = NaN;
            return result;
        }
        if (IsInfinite)
        {
            for (int k = 0; k < n; k++) result[k] = INF;
            return result;
        }

        // nth root of Abs -- faster / more accurate to use a solver here?
        double nthRootOfAbs = pow(Abs(), 1.0 / n);

        // Compute nth roots of FComplex number with k = 0, 1, ... n-1
        double nthPhi = GetArgument() / n;
        double slice = FMath::ms_dPI_Two / n;
        double innerPart = nthPhi;
        for (int k = 0; k < n; k++)
        {
            // inner part
            double realPart = nthRootOfAbs * cos(innerPart);
            double imaginaryPart = nthRootOfAbs * sin(innerPart);
            result.push_back(CreateFComplex(realPart, imaginaryPart));
            innerPart += slice;
        }
        return result;
    }

}; //LostPeterFoundation
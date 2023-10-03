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

#include "../include/FEquationQuartic.h"
#include "../include/FEquationQuadratic.h"
#include "../include/FEquationCubic.h"

namespace LostPeterFoundation
{
    FEquationQuartic::FEquationQuartic(FComplex& a, FComplex& b, FComplex& c, FComplex& d, FComplex& e, double epsilon /*= 1e-6*/)
        : A(a)
        , B(b)
        , C(c)
        , D(d)
        , E(e)
        , dEpsilon(epsilon)
    {
        if (a.Equals(FComplex::ZERO)) 
        {
            F_LogError("FEquationQuartic::FEquationQuartic: a must be != 0 !");
        }
    }

    FEquationQuartic::~FEquationQuartic()
    {

    }

    FComplex FEquationQuartic::findMaxDelta(FComplex* cubicRoots)
    {
        int k = 0;
        double maxModulus = 0;
        for (int i = 0; i < 3; i++)
        {
            FComplex y = cubicRoots[i];
            FComplex Delta = B*B - A*y*4;
            double modulus = Delta.Abs();
            if (modulus > maxModulus)
            {
                maxModulus = modulus;
                k = i;
            }
        }
        return cubicRoots[k];
    }

    void FEquationQuartic::Solve()
    {
        if (B.Abs() < this->dEpsilon && D.Abs() < this->dEpsilon)
        {
            FEquationQuadratic eQuadratic(A, C, E);
            eQuadratic.Solve();
            x[0] = eQuadratic.x[0].Sqrt();
            x[1] = -x[0];
            x[2] = eQuadratic.x[1].Sqrt();
            x[3] = -x[2];
        }
        else
        {
            FEquationCubic eCubic(FComplex::ONE,
                                  C*(-2),
                                  C*C + B*D - A*E*4,
                                  -(B*C*D - B*B*E - A*D*D),
                                  this->dEpsilon);
            eCubic.Solve();
            FComplex y = findMaxDelta(eCubic.x);
            FComplex DeltaRoot = (B*B - A*y*4).Sqrt();
            FComplex G, g, H, h;
            if (DeltaRoot.Abs() < this->dEpsilon)
            {
                G = B*0.5;
                g = G;
                H = (C - y)* 0.5;
                h = H;
            }
            else
            {
                G = (B + DeltaRoot)* 0.5;
                g = (B - DeltaRoot)* 0.5;
                FComplex part = (B* (C-y) - A*D*2)/(DeltaRoot*2);
                H = (C - y)* 0.5 + part;
                h = (C - y)* 0.5 - part;
            }
            FEquationQuadratic eQuadraticA(A, G, H);
            eQuadraticA.Solve();
            FEquationQuadratic eQuadraticB(A, g, h);
            eQuadraticB.Solve();
            x[0] = eQuadraticA.x[0];
            x[1] = eQuadraticA.x[1];
            x[2] = eQuadraticB.x[0];
            x[3] = eQuadraticB.x[1];
        }
    }   

    void FEquationQuartic::Check()
    {
        for (int i = 0; i < 4; i++)
        {
            FComplex xi2 = x[i]* x[i];
            FComplex z = A*xi2*xi2 + B*xi2*x[i] + C*xi2 + D*x[i] + E;
            if (z.Abs() > this->dEpsilon)
            {
                F_LogError("FEquationQuartic::Check: Bad root !");
            }
        }
    }

    void FEquationQuartic::Print()
    {
        F_LogInfo("%sx^4 + %sx^3 + %sx^2 + %sx + %s = 0", A.ToString().c_str(), B.ToString().c_str(), C.ToString().c_str(), D.ToString().c_str(), E.ToString().c_str());
	    F_LogInfo("x0 = %s, x1 = %s, x2 = %s, x3 = %s", x[0].ToString().c_str(), x[1].ToString().c_str(), x[2].ToString().c_str(), x[3].ToString().c_str());
    }

}; //LostPeterFoundation
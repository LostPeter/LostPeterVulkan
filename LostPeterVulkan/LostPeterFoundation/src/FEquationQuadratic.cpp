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

#include "../include/FEquationQuadratic.h"


namespace LostPeterFoundation
{
    FEquationQuadratic::FEquationQuadratic(FComplex& a, FComplex& b, FComplex& c)
        : A(a)
        , B(b)
        , C(c)
    {
        if (a.Equals(FComplex::ZERO)) 
        {
            F_LogError("*********************** FEquationQuadratic::FEquationQuadratic: a must be != 0 !");
        }
    }

    FEquationQuadratic::~FEquationQuadratic()
    {

    }

    void FEquationQuadratic::Solve()
    {
        FComplex discriminant = B*B - A*C*4;
        x[0] = (-B - discriminant.Sqrt()) / (A*2);
        x[1] = (-B + discriminant.Sqrt()) / (A*2);
    }

    void FEquationQuadratic::Print()
    {
        F_LogInfo("%sx^2 + %sx + %s = 0", A.ToString().c_str(), B.ToString().c_str(), C.ToString().c_str());
	    F_LogInfo("x0 = %s, x1 = %s", x[0].ToString().c_str(), x[1].ToString().c_str());
    }

}; //LostPeterFoundation
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

#ifndef _F_EQUATION_QUARTIC_H_
#define _F_EQUATION_QUARTIC_H_

#include "FPreDefine.h"
#include "FComplex.h"

namespace LostPeterFoundation
{
    class foundationExport FEquationQuartic
    {
    public:
        FEquationQuartic(FComplex& a, FComplex& b, FComplex& c, FComplex& d, FComplex& e, double epsilon = 1e-6);
        ~FEquationQuartic();

    public:
        FComplex A, B, C, D, E;
        double dEpsilon;
	    FComplex x[4];

    public:
        void Solve();   
        void Check();
        void Print();

    private:
        FComplex findMaxDelta(FComplex* cubicRoots);
    };

}; //LostPeterFoundation

#endif
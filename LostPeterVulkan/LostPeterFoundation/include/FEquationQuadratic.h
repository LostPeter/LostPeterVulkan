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

#ifndef _F_EQUATION_QUADRATIC_H_
#define _F_EQUATION_QUADRATIC_H_

#include "FPreDefine.h"
#include "FComplex.h"

namespace LostPeterFoundation
{
    class foundationExport FEquationQuadratic
    {
    public:
        FEquationQuadratic(FComplex& a, FComplex& b, FComplex& c);
        ~FEquationQuadratic();

    public:
        FComplex A, B, C;
	    FComplex x[2];

    public:
        void Solve();   
        void Print();
    };

}; //LostPeterFoundation

#endif
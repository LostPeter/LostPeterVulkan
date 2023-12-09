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

#ifndef _F_EQUATION_CUBIC_H_
#define _F_EQUATION_CUBIC_H_

#include "FPreDefine.h"
#include "FComplex.h"

namespace LostPeterFoundation
{
    class foundationExport FEquationCubic
    {
    public:
        FEquationCubic(const FComplex& a, const FComplex& b, const FComplex& c, const FComplex& d, double epsilon = 1e-6);
        ~FEquationCubic();

    public:
        FComplex A, B, C, D;
        double dEpsilon;
	    FComplex x[3];

    public:
        double MaxDist(FComplex* roots);

        void Solve();   
        void Check();
        void Print();

    private:
        FComplex p, q;
        void canonicalForm();
        FComplex ytox(FComplex y);
        int find_m_from_e(FComplex z, FComplex* eps);
        int find_l_for_m(int m);
    };

}; //LostPeterFoundation

#endif
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

#include "../include/FEquationCubic.h"


namespace LostPeterFoundation
{
    FEquationCubic::FEquationCubic(const FComplex& a, const FComplex& b, const FComplex& c, const FComplex& d, double epsilon /*= 1e-6*/)
        : A(a)
        , B(b)
        , C(c)
        , D(d)
        , dEpsilon(epsilon)
    {
        if (a.Equals(FComplex::ZERO)) 
        {
            F_LogError("*********************** FEquationCubic::FEquationCubic: a must be != 0 !");
        }
    }

    FEquationCubic::~FEquationCubic()
    {

    }

    void FEquationCubic::canonicalForm()
    {
        p = C/A - B*B/(A*A*3);
	    q = (B*B*B*2)/(A*A*A*27) + D/A - B*C/(A*A*3);
    }
    FComplex FEquationCubic::ytox(FComplex y)
    {
        return y - B/(A*3);
    }
    int FEquationCubic::find_m_from_e(FComplex z, FComplex* e)
    {
        for (int i = 0; i < 3; i++)
        {
            FComplex r = z - e[i];
            if (r.Abs() < 1e-7)
            {
                return i;
            }
        }
        return 0;
    }
    int FEquationCubic::find_l_for_m(int m)
    {
        switch (m)
        {
        case 0: return 0;
        case 1: return 2;
        case 2: return 1;
        default: F_LogError("*********************** FEquationCubic::find_l_for_m: Is not possible !");
        }
        return 0;
    }

    double FEquationCubic::MaxDist(FComplex* roots)
    {
        double maxDist = 0;
        for (int i = 0; i < 3; i++)
        {
            FComplex xi = x[i];
            double minDist = 1;
            for (int j = 0; j<3; j++)
            {
                double dist = (xi-roots[j]).Abs();
                minDist = fmin(minDist, dist);
            }
            maxDist = fmax(maxDist, minDist);
        }
        return maxDist;
    }

    void FEquationCubic::Solve()
    {
        canonicalForm();
        if (p.Abs() < this->dEpsilon)
        {
            std::vector<FComplex> roots = (-q).NTHRoots(3);
            for (int i = 0; i < 3; i++)
                x[i] = ytox(roots[i]);
        }
        else
        {
            FComplex t2 = (q* q + p*p*p* 4.0 / 27.0);
            if (t2.Abs() < this->dEpsilon) 
                t2 = FComplex::ZERO; //double root
            FComplex temp = t2.Sqrt();
            FComplex z0 = (temp-q)/2;
            FComplex v0 = z0.NTHRoot(3);
            FComplex u = (-q-z0).NTHRoot(3);
            temp = v0 * u / p* (-3);
            FComplex e[3];
            e[0] = FComplex::CreateFComplex(1, 0);
            e[1] = FComplex::CreateFComplex(-1.0 / 2, sqrt(3) / 2);
            e[2] = FComplex::CreateFComplex(-1.0 / 2, -sqrt(3) / 2);
            int m = find_m_from_e(temp, e);
            int n = find_l_for_m(m);
            FComplex u0 = e[n]* u;
            FComplex y[3];
            y[0] = v0 + u0;
            y[1] = e[1]* v0 + e[1]* e[1]* u0;
            y[2] = e[1]* e[1]* v0 + e[1]* u0;
            for (int i = 0; i < 3; i++)
            {
                x[i] = ytox(y[i]);
            }
        }
    }

    void FEquationCubic::Check()
    {
        for (int i = 0; i < 3; i++)
        {
            FComplex xi2 = x[i]* x[i];
            FComplex z = A* xi2* x[i] + B * xi2 + C * x[i] + D;
            if (z.Abs() > this->dEpsilon)
            {
                F_LogError("*********************** FEquationCubic::Check: Bad root !");
            }
        }
    }

    void FEquationCubic::Print()
    {
        F_LogInfo("%sx^3 + %sx^2 + %sx + %s = 0", A.ToString().c_str(), B.ToString().c_str(), C.ToString().c_str(), D.ToString().c_str());
	    F_LogInfo("x0 = %s, x1 = %s, x2 = %s", x[0].ToString().c_str(), x[1].ToString().c_str(), x[2].ToString().c_str());
    }

}; //LostPeterFoundation
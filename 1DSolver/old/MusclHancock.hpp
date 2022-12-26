#ifndef MUSCLHANCOCK_HPP
#define MUSCLHANCOCK_HPP

#include <cmath>
#include <vector>
#include "Vector3.hpp"
#include "Euler1D.hpp"

using namespace Euler1D;

namespace MusclHancock
{
    std::vector<Vector3> musclHcalcSlopes(std::vector<Vector3> w, double omega, Vector3 wIn, Vector3 wOut)
    {
        std::vector<Vector3> slopes;

        int n = w.size();

        slopes.push_back(0.5*(1.0 + omega)*(w[0] - wIn) + 0.5*(1.0 - omega)*(w[1] - w[0]));

        for (int i = 1; i < n-1; i++)
        {
            slopes.push_back(0.5*(1.0 + omega)*(w[i] - w[i-1]) + 0.5*(1.0 - omega)*(w[i+1] - w[i]));
        }

        slopes.push_back(0.5*(1.0 + omega)*(w[n-1] - w[n-2]) + 0.5*(1.0 - omega)*(wOut - w[n-1]));

        return slopes;
    }

    std::vector<Vector3> musclHReconstruction(const std::vector<Vector3>& w, const std::vector<Vector3>& slopes, double dx)
    {
        std::vector<Vector3> wk;

        for (int i = 0; i < w.size(); i++)
        {
            wk.push_back(w[i] - 0.5*slopes[i]);
        }
        
        return wk;
    }

    std::vector<Vector3> musclEvolution(const std::vector<Vector3>& w, const std::vector<Vector3>& wk, const std::vector<Vector3>& wl, const std::vector<Vector3>& wr, double dx, double dt)
    {
        std::vector<Vector3> wkEvo;
        
        for (int i = 0; i < w.size(); i++)
        {
            wkEvo.push_back(wk[i]+0.5*(dt/dx)*(flux(wl[i])-flux(wr[i])));
        }
        return wkEvo;
    }
}

#endif
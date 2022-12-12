#ifndef HLLCFLUX_HPP
#define HLLCFLUX_HPP

#include <cmath>
#include "Vector3.hpp"

using namespace Euler1D;

namespace HLLCFlux
{
    Vector3 waveSpeedsEstimate(const Vector3& wl, const Vector3& wr)
    {
        //PVRS
        double pl = pressure(wl);
        double pr = pressure(wr);
        double ul = velocity(wl);
        double ur = velocity(wr);
        double rhol = density(wl);
        double rhor = density(wr);
        double al = soundSpeed(wl);
        double ar = soundSpeed(wr);

        double pStar = std::fmax(0, 0.5*((pl + pr) - 0.25*(ul + ur)*(rhol + rhor)*(al + ar)));
        double ql;
        double qr;

        if(pStar > pl)
        {
            ql = pow(1 + ((GAMMA + 1)/(2*GAMMA))*((pStar/pl) - 1), 0.5);
        }
        else
        {
            ql = 1;
        }
        double sl = ul - al*ql;

        if(pStar > pr)
        {
            qr = pow(1 + ((GAMMA + 1)/(2*GAMMA))*((pStar/pr) - 1), 0.5);
        }
        else
        {
            qr = 1;
        }
        double sr = ur + ar*qr;

        double ss = (pr - pl + rhol*ul*(sl - ul) - rhor*ur*(sr - ur))/(rhol*sl - rhol*ul - rhor*sr + rhor*ur);

        return Vector3({sl, ss, sr});
    }


    Vector3 HLLC(const Vector3& wl, const Vector3& wr)
    {
        enum {sl, ss, sr};
        Vector3 wSpeed = waveSpeedsEstimate(wl, wr);
        

        if (0 <= wSpeed[sl])
        {
            //FL
            return flux(wl);
        }
        else if(0 <= wSpeed[ss])
        {
            //F*L
            return (wSpeed[ss]*(wSpeed[sl]*wl - flux(wl)) + wSpeed[sl]*(pressure(wl) + density(wl)*(wSpeed[sl] - velocity(wl))*(wSpeed[ss] - velocity(wl)))*Vector3({0, 1, wSpeed[ss]}))/(wSpeed[sl] - wSpeed[ss]);
        }
        else if(0 <= wSpeed[sr])
        {
            //F*R
            return (wSpeed[ss]*(wSpeed[sr]*wr - flux(wr)) + wSpeed[sr]*(pressure(wr) + density(wr)*(wSpeed[sr] - velocity(wr))*(wSpeed[ss] - velocity(wr)))*Vector3({0, 1, wSpeed[ss]}))/(wSpeed[sr] - wSpeed[ss]);
        }
        else
        {
            //FR
            return flux(wr);
        }
    }
}

#endif
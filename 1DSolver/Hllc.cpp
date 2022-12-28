#include <cmath>
#include "RiemannSolver.hpp"
#include "Hllc.hpp"

Hllc::Hllc()
{

}

Hllc::Hllc(std::shared_ptr<EulerEquations> eqnIn)
{
    setEquationModel(eqnIn);
}

void Hllc::setEquationModel(std::shared_ptr<EulerEquations> eqnIn)
{
    eulerEqn = eqnIn;
}

Vector3 Hllc::waveSpeedsEstimate(const Vector3& wl, const Vector3& wr) const
{
    //PVRS
    double pl = eulerEqn->pressure(wl);
    double pr = eulerEqn->pressure(wr);
    double ul = eulerEqn->velocity(wl);
    double ur = eulerEqn->velocity(wr);
    double rhol = eulerEqn->density(wl);
    double rhor = eulerEqn->density(wr);
    double al = eulerEqn->soundSpeed(wl);
    double ar = eulerEqn->soundSpeed(wr);
    double Gamma = eulerEqn->getGamma();

    double pStar = std::fmax(0, 0.5*((pl + pr) - 0.25*(ul + ur)*(rhol + rhor)*(al + ar)));
    double ql;
    double qr;

    if(pStar > pl)
    {
        ql = pow(1 + ((Gamma + 1)/(2*Gamma))*((pStar/pl) - 1), 0.5);
    }
    else
    {
        ql = 1;
    }
    double sl = ul - al*ql;

    if(pStar > pr)
    {
        qr = pow(1 + ((Gamma + 1)/(2*Gamma))*((pStar/pr) - 1), 0.5);
    }
    else
    {
        qr = 1;
    }
    double sr = ur + ar*qr;

    double ss = (pr - pl + rhol*ul*(sl - ul) - rhor*ur*(sr - ur))/(rhol*sl - rhol*ul - rhor*sr + rhor*ur);

    return Vector3({sl, ss, sr});
    
}

Vector3 Hllc::calculateFlux(const Vector3& wl, const Vector3& wr) const
{
    enum {sl, ss, sr};
    Vector3 wSpeed = waveSpeedsEstimate(wl, wr);
        

    if (0 <= wSpeed[sl])
    {
        //FL
        return eulerEqn->flux(wl);
    }
    else if(0 <= wSpeed[ss])
    {
        //F*L
        return (wSpeed[ss]*(wSpeed[sl]*wl - eulerEqn->flux(wl)) + wSpeed[sl]*(eulerEqn->pressure(wl) + eulerEqn->density(wl)*(wSpeed[sl] - eulerEqn->velocity(wl))*(wSpeed[ss] - eulerEqn->velocity(wl)))*Vector3({0, 1, wSpeed[ss]}))/(wSpeed[sl] - wSpeed[ss]);
    }
    else if(0 <= wSpeed[sr])
    {
        //F*R
        return (wSpeed[ss]*(wSpeed[sr]*wr - eulerEqn->flux(wr)) + wSpeed[sr]*(eulerEqn->pressure(wr) + eulerEqn->density(wr)*(wSpeed[sr] - eulerEqn->velocity(wr))*(wSpeed[ss] - eulerEqn->velocity(wr)))*Vector3({0, 1, wSpeed[ss]}))/(wSpeed[sr] - wSpeed[ss]);
    }
    else
    {
        //FR
        return eulerEqn->flux(wr);
    }
}

std::vector<Vector3> Hllc::calculateFluxes(const std::vector<Vector3>& wl, const std::vector<Vector3>& wr) const
{
    std::vector<Vector3> out;

    for (int i = 0; i < wr.size()-1; i++)
    {
        out.push_back(calculateFlux(wl[i], wr[i+1]));
    }
    
    return out;
}
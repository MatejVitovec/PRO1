#include <cmath>
#include "RiemannSolver.hpp"
#include "Hll.hpp"

Hll::Hll()
{

}

Hll::Hll(std::shared_ptr<EulerEquations> eqnIn)
{
    setEquationModel(eqnIn);
}

void Hll::setEquationModel(std::shared_ptr<EulerEquations> eqnIn)
{
    eulerEqn = eqnIn;
}

Vector3 Hll::waveSpeedsEstimate(const Vector3& wl, const Vector3& wr) const
{
    double ul = eulerEqn->velocity(wl);
    double ur = eulerEqn->velocity(wr);
    double al = eulerEqn->soundSpeed(wl);
    double ar = eulerEqn->soundSpeed(wr);

    double sl = std::min(ul - al, ur - ar);
    double sr = std::max(ul + al, ur + ar);
    double ss = 0.0; //nepotrebuju

    return Vector3({sl, ss, sr});    
}

Vector3 Hll::calculateFlux(const Vector3& wl, const Vector3& wr) const
{
    enum {sl, ss, sr};
    Vector3 wSpeed = waveSpeedsEstimate(wl, wr);        

    if (0 <= wSpeed[sl])
    {
        //FL
        return eulerEqn->flux(wl);
    }
    else if(0 < wSpeed[sr])
    {
        //F*
        return (wSpeed[sr]*eulerEqn->flux(wl) - wSpeed[sl]*eulerEqn->flux(wr) + wSpeed[sl]*wSpeed[sr]*(wr - wl)) / (wSpeed[sr] - wSpeed[sl]);
    }
    else
    {
        //FR
        return eulerEqn->flux(wr);
    }
}

std::vector<Vector3> Hll::calculateFluxes(const std::vector<Vector3>& wl, const std::vector<Vector3>& wr) const
{
    std::vector<Vector3> out;

    for (int i = 0; i < wl.size()-1; i++)
    {
        out.push_back(calculateFlux(wl[i], wr[i+1]));
    }
    
    return out;
}
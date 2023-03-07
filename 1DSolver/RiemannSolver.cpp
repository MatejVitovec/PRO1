#include "RiemannSolver.hpp"

void RiemannSolver::setEquationModel(std::shared_ptr<EulerEquations> eulerEqnIn)
{
    eulerEqn = eulerEqnIn;
}

std::vector<Vector3> RiemannSolver::calculateFluxes(const std::vector<Vector3>& wl, const std::vector<Vector3>& wr) const
{
    std::vector<Vector3> out;

    for (int i = 0; i < wl.size()-1; i++)
    {
        out.push_back(calculateFlux(wl[i], wr[i+1]));
    }
    
    return out;
}
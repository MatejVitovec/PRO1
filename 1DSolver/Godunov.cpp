#include <cmath>
#include "SpatialScheme.hpp"
#include "Godunov.hpp"

Godunov::Godunov()
{

}

Godunov::Godunov(std::shared_ptr<RiemannSolver> riemann)
{
    setRiemannSolver(riemann);
}

void Godunov::setRiemannSolver(std::shared_ptr<RiemannSolver> riemann)
{
    riemannSolver = riemann;
}


std::vector<Vector3> Godunov::calculateResidues(const std::vector<Vector3>& w, double dx) const
{
    std::vector<Vector3> res;

    std::vector<Vector3> f = riemannSolver->calculateFluxes(w, w);

    res.push_back(Vector3({0.0, 0.0, 0.0})); // inlet void cell residue

    for (int i = 0; i < w.size()-2; i++)
    {
        res.push_back((f[i] - f[i+1])/dx);
    }
    
    res.push_back(Vector3({0.0, 0.0, 0.0})); // outlet void cell residue

    return res;
}
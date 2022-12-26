#include <vector>
#include <cmath>
#include "TemporalScheme.hpp"
#include "ExplicitEuler.hpp"

ExplicitEuler::ExplicitEuler()
{

}

ExplicitEuler::ExplicitEuler(std::shared_ptr<EulerEquations> eulerEqn, std::shared_ptr<SpatialScheme> spaceScheme)
{
    setEquationModel(eulerEqn);
    setSpatialScheme(spaceScheme);
}

std::vector<Vector3> ExplicitEuler::solve(const std::vector<Vector3>& w, double dt, double dx) const
{
    std::vector<Vector3> wn;
    
    std::vector<Vector3> res = spaceScheme->calculateResidues(w, dx);

    wn.push_back(Vector3({0.0, 0.0, 0.0}));

    for (int i = 1; i < w.size()-1; i++)
    {
        wn.push_back(w[i] - dt*res[i]);
    }
    
    wn.push_back(Vector3({0.0, 0.0, 0.0}));

    return wn;
}
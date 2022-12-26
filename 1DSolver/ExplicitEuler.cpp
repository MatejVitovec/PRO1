#include <vector>
#include <cmath>
#include "TemporalScheme.hpp"
#include "ExplicitEuler.hpp"

ExplicitEuler::ExplicitEuler()
{

}

ExplicitEuler::ExplicitEuler(std::shared_ptr<EulerEquations> eulerEqn, std::shared_ptr<SpatialScheme> spaceScheme, double cfl)
{
    setEquationModel(eulerEqn);
    setSpatialScheme(spaceScheme);
    setCfl(cfl);
}

std::vector<Vector3> ExplicitEuler::Solve(const std::vector<Vector3>& w) const
{
    std::vector<Vector3> wn;

    double dx = 0.1;

    std::vector<Vector3> res = spaceScheme->calculateResidues(w);

    wn.push_back(Vector3({0.0, 0.0, 0.0}));

    double dt = timeStep(w, dx);

    for (int i = 1; i < w.size()-1; i++)
    {
        wn.push_back(w[i] - dt*res[i]);
    }
    
    wn.push_back(Vector3({0.0, 0.0, 0.0}));

    return wn;
}
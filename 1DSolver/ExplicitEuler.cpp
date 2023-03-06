#include <vector>
#include <cmath>
#include "TemporalScheme.hpp"
#include "ExplicitEuler.hpp"

ExplicitEuler::ExplicitEuler()
{

}

ExplicitEuler::ExplicitEuler(std::shared_ptr<SpatialScheme> spaceScheme)
{
    setSpatialScheme(spaceScheme);
}

std::vector<Vector3> ExplicitEuler::solve(const std::vector<Vector3>& w, const double& dt, const double& dx) const
{
    std::vector<Vector3> wn;
    
    std::vector<Vector3> res = spaceScheme->calculateResidues(w, dx);

    for (int i = 0; i < w.size(); i++)
    {
        wn.push_back(w[i] + dt*res[i]);
    }

    return wn;
}

std::vector<Vector3> ExplicitEuler::solve(const std::vector<Vector3>& w, const double& dt, std::shared_ptr<Nozzle> mesh) const
{
    std::vector<Vector3> wn;
    
    std::vector<Vector3> res = spaceScheme->calculateResidues(w, mesh);

    for (int i = 0; i < w.size(); i++)
    {
        wn.push_back(w[i] + dt*(res[i]));
    }

    return wn;
}
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

std::vector<Vector3> ExplicitEuler::solve(const std::vector<Vector3>& w, double dt, double dx) const
{
    std::vector<Vector3> wn;
    
    std::vector<Vector3> res = spaceScheme->calculateResidues(w, dx);

    wn.push_back(w[0]);

    for (int i = 1; i < w.size()-1; i++)
    {
        wn.push_back(w[i] + dt*res[i]);
    }
    
    wn.push_back(w[w.size()-1]);

    return wn;
}

std::vector<Vector3> ExplicitEuler::solve(const std::vector<Vector3>& w, std::shared_ptr<SourceTerm> sourceTerm, double dt, double dx) const
{
    std::vector<Vector3> wn;
    
    std::vector<Vector3> res = spaceScheme->calculateResidues(w, dx);
    std::vector<Vector3> sourceRes = sourceTerm->calculateSourceResidues(w);

    wn.push_back(w[0]);

    for (int i = 1; i < w.size()-1; i++)
    {
        wn.push_back(w[i] + dt*(res[i] + sourceRes[i]));
    }
    
    wn.push_back(w[w.size()-1]);

    return wn;
}
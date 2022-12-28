#include <vector>
#include <cmath>
#include "TemporalScheme.hpp"
#include "ExplicitRK2.hpp"

ExplicitRK2::ExplicitRK2()
{

}

ExplicitRK2::ExplicitRK2(std::shared_ptr<SpatialScheme> spaceScheme)
{
    setSpatialScheme(spaceScheme);
}

std::vector<Vector3> ExplicitRK2::solve(const std::vector<Vector3>& w, double dt, double dx) const
{
    std::vector<Vector3> wk;
    std::vector<Vector3> wn;
    
    //first stage
    std::vector<Vector3> k1 = spaceScheme->calculateResidues(w, dx);
    for (int i = 0; i < w.size(); i++)
    {
        wk.push_back(w[i] + (dt/2.0)*k1[i]);
    }    

    //second stage
    std::vector<Vector3> k2 = spaceScheme->calculateResidues(wk, dx);
    for (int i = 0; i < w.size(); i++)
    {
        wn.push_back(w[i] + dt*k2[i]);
    }

    return wn;
}

std::vector<Vector3> ExplicitRK2::solve(const std::vector<Vector3>& w, std::shared_ptr<SourceTerm> sourceTerm, double dt, double dx) const
{
    //TODO (zatim euler)

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
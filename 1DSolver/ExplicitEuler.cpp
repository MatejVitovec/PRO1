#include <vector>
#include <cmath>
#include "TemporalScheme.hpp"
#include "ExplicitEuler.hpp"

ExplicitEuler::ExplicitEuler()
{

}

ExplicitEuler::ExplicitEuler(std::shared_ptr<SpatialScheme> spaceScheme, std::shared_ptr<SourceTerm> srcTerm)
{
    setSpatialScheme(spaceScheme);
    setSourceTerm(srcTerm);
}

std::vector<Vector3> ExplicitEuler::solve(const std::vector<Vector3>& w, double dt, double dx) const
{
    std::vector<Vector3> wn;
    
    std::vector<Vector3> res = spaceScheme->calculateResidues(w, dx);
    std::vector<Vector3> sourceRes = sourceTerm->calculateSourceResidues(w);

    wn.push_back(w[0]);

    for (int i = 1; i < w.size()-1; i++)
    {
        wn.push_back(w[i] - dt*(res[i] + sourceRes[i]));
    }
    
    wn.push_back(w[w.size()-1]);

    return wn;
}
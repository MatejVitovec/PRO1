#include <vector>
#include <cmath>
#include "TemporalScheme.hpp"
#include "ExplicitHeun.hpp"

ExplicitHeun::ExplicitHeun()
{

}

ExplicitHeun::ExplicitHeun(std::shared_ptr<SpatialScheme> spaceScheme)
{
    setSpatialScheme(spaceScheme);
}

std::vector<Vector3> ExplicitHeun::solve(const std::vector<Vector3>& w, const double& dt, const double& dx) const
{
    std::vector<Vector3> wp;
    std::vector<Vector3> wn;

    //first stage
    std::vector<Vector3> k1 = spaceScheme->calculateResidues(w, dx);
    for (int i = 0; i < w.size(); i++)
    {
        wp.push_back(w[i] + dt*(k1[i]));
    }    

    //second stage
    std::vector<Vector3> k2 = spaceScheme->calculateResidues(wp, dx);
    for (int i = 0; i < w.size(); i++)
    {
        wn.push_back(w[i] + (dt/2.0)*(k1[i] + k2[i]));
    }

    return wn;
}

std::vector<Vector3> ExplicitHeun::solve(const std::vector<Vector3>& w, const double& dt, std::shared_ptr<Nozzle> mesh) const
{
    std::vector<Vector3> wp;
    std::vector<Vector3> wn;

    //first stage
    std::vector<Vector3> k1 = spaceScheme->calculateResidues(w, mesh);
    for (int i = 0; i < w.size(); i++)
    {
        wp.push_back(w[i] + dt*(k1[i]));
    }    

    //second stage
    std::vector<Vector3> k2 = spaceScheme->calculateResidues(wp, mesh);
    for (int i = 0; i < w.size(); i++)
    {
        wn.push_back(w[i] + (dt/2.0)*(k1[i] + k2[i]));
    }

    return wn;
}
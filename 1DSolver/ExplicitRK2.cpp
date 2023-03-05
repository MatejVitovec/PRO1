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

std::vector<Vector3> ExplicitRK2::solve(const std::vector<Vector3>& w, const double& dt, const double& dx) const
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

std::vector<Vector3> ExplicitRK2::solve(const std::vector<Vector3>& w, const double& dt, std::shared_ptr<NozzleGeometry> geometry) const
{
    /*std::vector<Vector3> wk;
    std::vector<Vector3> wn;
    
    //first stage
    std::vector<Vector3> k1 = spaceScheme->calculateResidues(w, geometry);
    for (int i = 0; i < w.size(); i++)
    {
        wk.push_back(w[i] + (dt/2.0)*(k1[i]));
    }    

    //second stage
    std::vector<Vector3> k2 = spaceScheme->calculateResidues(wk, geometry);
    for (int i = 0; i < w.size(); i++)
    {
        wn.push_back(w[i] + dt*(k2[i]));
    }*/

    std::vector<Vector3> wp;
    std::vector<Vector3> wn;


    //first stage
    std::vector<Vector3> k1 = spaceScheme->calculateResidues(w, geometry);
    for (int i = 0; i < w.size(); i++)
    {
        wp.push_back(w[i] + dt*(k1[i]));
    }    

    //second stage
    std::vector<Vector3> k2 = spaceScheme->calculateResidues(wp, geometry);
    for (int i = 0; i < w.size(); i++)
    {
        wn.push_back(w[i] + (dt/2.0)*(k1[i] + k2[i]));
    }

    return wn;
}
#include <cmath>
#include "Solver.hpp"

Solver::Solver()
{

}

Solver::Solver(std::shared_ptr<EulerEquations> euler, std::shared_ptr<Mesh> mesh, std::shared_ptr<SpatialScheme> spcScheme, std::shared_ptr<TemporalScheme> tmpScheme)
{
    setEquationModel(euler);
    setMesh(mesh);
    setSpatialScheme(spcScheme);
    setTemporalScheme(tmpScheme);
}

void Solver::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    Solver::eulerEqn = euler;
}

void Solver::setMesh(std::shared_ptr<Mesh> mesh)
{
    Solver::mesh = mesh;
}

void Solver::setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme)
{
    Solver::spaceScheme = spcScheme;
}

void Solver::setTemporalScheme(std::shared_ptr<TemporalScheme> tmpScheme)
{
    Solver::temporalScheme = tmpScheme;
}

double Solver::timeStep(std::vector<Vector3> w, double dx, double cfl) const
{
    double maxdt = 10e+100;

    for (int i = 0; i < w.size(); i++)
    {
        //min(dt, dx/spectral radius)
        maxdt = std::min(maxdt, dx/(fabs(eulerEqn->velocity(w[i])) + eulerEqn->soundSpeed(w[i])));
    }

    return maxdt*cfl;
}

std::vector<Vector3> Solver::solveOneIter(const std::vector<Vector3>& w, double cfl) const
{
    double dx = mesh->getDx();
    double dt = timeStep(w, dx, cfl);

    std::vector<Vector3> wn = temporalScheme->solve(w, dt, dx);

    return wn;
}

std::vector<Vector3> Solver::stream(const std::vector<Vector3>& w, Vector3 inlet, Vector3 outlet) const
{
    std::vector<Vector3> wn;

    wn.push_back(inlet);

    for (int i = 1; i < w.size()-1; i++)
    {
        wn.push_back(w[i]);
    }
    
    wn.push_back(outlet);

    return wn;
}

std::vector<Vector3> Solver::solve(std::vector<Vector3> w, const int& iter, const double& cfl) const
{
    std::vector<Vector3> wn;

    w = stream(wn, Vector3(), Vector3());

    for (int i = 0; i < iter; i++)
    {
        wn = solveOneIter(w, cfl);
        
        //vypocet op TODO
        w = stream(wn, Vector3(), Vector3());

        //save()
    }
    
    return w;
}
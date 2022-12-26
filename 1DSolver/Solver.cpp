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

std::vector<Vector3> Solver::overwriteBC(std::vector<Vector3> w, std::shared_ptr<BoundaryCondition> inlet, std::shared_ptr<BoundaryCondition> outlet) const
{
    w[0] = inlet->calcBoundaryState(w);    
    w[w.size()-1] = outlet->calcBoundaryState(w);

    return w;
}

std::vector<Vector3> Solver::solve(std::vector<Vector3> w, std::shared_ptr<BoundaryCondition> inlet, std::shared_ptr<BoundaryCondition> outlet, const int& iter, const double& cfl) const
{

    //nutne pred prvni iteraci
    w = overwriteBC(w, inlet, outlet);

    for (int i = 0; i < iter; i++)
    {
        double dx = mesh->getDx();
        double dt = timeStep(w, dx, cfl);

        std::vector<Vector3> wn = temporalScheme->solve(w, dt, dx);
        
        w = overwriteBC(wn, inlet, outlet);

        //save()
    }
    
    return w;
}
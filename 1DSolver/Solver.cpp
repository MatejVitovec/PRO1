#include <cmath>
#include <iostream>
#include "Solver.hpp"

Solver::Solver()
{

}

Solver::Solver(std::shared_ptr<EulerEquations> euler, std::shared_ptr<SpatialScheme> spcScheme, std::shared_ptr<TemporalScheme> tmpScheme)
{
    spcScheme->setEquationModel(euler);
    tmpScheme->setSpatialScheme(spcScheme);

    setEquationModel(euler);
    setSpatialScheme(spcScheme);
    setTemporalScheme(tmpScheme);
}

void Solver::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    Solver::eulerEqn = euler;
}

void Solver::setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme)
{
    Solver::spaceScheme = spcScheme;
}

void Solver::setTemporalScheme(std::shared_ptr<TemporalScheme> tmpScheme)
{
    Solver::temporalScheme = tmpScheme;
}

std::vector<double> Solver::getDensityResidues()
{
    return densityResidue;
}

std::vector<Vector3> Solver::calcInitialCondition(Vector3 wInit, std::shared_ptr<Mesh> mesh)
{
    std::vector<Vector3> w;

    for (int i = 0; i < mesh->getCells(); i++)
    {
        w.push_back(wInit);
    }

    return w;
}

std::vector<Vector3> Solver::calcInitialCondition(Vector3 wl, Vector3 wr, std::shared_ptr<Mesh> mesh)
{
    std::vector<Vector3> out;

    double firstX = mesh->getFirstX();
    double dx = mesh->getDx();
    double initDiscontinuityPos = firstX + (mesh->getDomain()/2.0);

    for (int i = 0; i < mesh->getCells(); i++)
    {
        double x = firstX + i*dx;
        if (x < initDiscontinuityPos)
        {
            out.push_back(wl);
        }
        else
        {
            out.push_back(wr);
        }
    }

    return out;
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

double Solver::timeStep(std::vector<Vector3> w, double dx, double cfl, double time, double targetTime) const
{
    double maxdt = 10e+100;

    for (int i = 0; i < w.size(); i++)
    {
        //min(dt, dx/spectral radius)
        maxdt = std::min(maxdt, dx/(fabs(eulerEqn->velocity(w[i])) + eulerEqn->soundSpeed(w[i])));
    }

    double dt = maxdt*cfl;
      
    if(targetTime - time < maxdt)
    {
        dt = targetTime - time;
        //exitCalcualtion = true;
    }
    time += dt;

    return dt;
}


std::vector<Vector3> Solver::solve(std::vector<Vector3> w, const double& dx, const int& maxIter, const double& targetTime, const double& cfl)
{
    double t = 0;
    std::vector<Vector3> wn;

    int iter = 0;
    while (iter < maxIter)
    {
        double dt = timeStep(w, dx, cfl, t, targetTime);
        t += dt;

        wn = temporalScheme->solve(w, dt, dx);
        w = wn;

        if (t >= targetTime)
        {
            break;
        }
        
        ++iter;
    }

    std::cout << "vypocet probehl uspesne s " << iter << " iteracemi\n";
    
    return w;
}


std::vector<Vector3> Solver::solve(std::vector<Vector3> w, std::shared_ptr<Nozzle> mesh, const int& iter, const double& cfl)
{
    double dx = mesh->getDx();
    densityResidue.clear();

    for (int i = 0; i < iter; i++)
    {
        double dt = timeStep(w, dx, cfl);

        if(i == 15000)
        {
            int a = 5;
        }

        std::vector<Vector3> wn = temporalScheme->solve(w, dt, mesh);

        densityResidue.push_back(calcDensityResidue(w, wn, dt));

        w = wn; //streaming
    }
    
    std::cout << "vypocet probehl uspesne s " << iter << " iteracemi\n";

    return w;
}


double Solver::calcDensityResidue(std::vector<Vector3> w, std::vector<Vector3> wn, double dt)
{
    double sumRes = 0;

    for (int i = 0; i < w.size(); i++)
    {
        sumRes += pow((eulerEqn->density(w[i]) - eulerEqn->density(wn[i]))/dt, 2.0);
    }

    return sqrt(sumRes);
    //return sqrt(sumRes/n);
}


#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <memory>

#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "Solver.hpp"
#include "SlopeLimiter.hpp"
#include "VanAlbada.hpp"
#include "Superbee.hpp"
#include "Minmod.hpp"
#include "MonotonizedCentral.hpp"
#include "SpatialScheme.hpp"
#include "Godunov.hpp"
#include "Muscl.hpp"
#include "RiemannSolver.hpp"
#include "Hllc.hpp"
#include "Hll.hpp"
#include "TemporalScheme.hpp"
#include "ExplicitEuler.hpp"
#include "ExplicitHeun.hpp"
#include "ExplicitRK2.hpp"
#include "Mesh.hpp"
#include "Nozzle.hpp"
#include "BoundaryCondition.hpp"
#include "PressureTemperatureInlet.hpp"
#include "PressureDensityInlet.hpp"
#include "PressureOutlet.hpp"

void saveRiemann(std::string fileName, std::vector<Vector3> w, std::shared_ptr<EulerEquations> eulerEqn, std::shared_ptr<Mesh> mesh, double time)
{
    std::ofstream writeToFile(fileName);
    writeToFile << mesh->getDomain() << std::endl;
    writeToFile << mesh->getCells() << std::endl;
    writeToFile << time << std::endl;

    for (int i = 0; i < w.size(); i++)
    {
        writeToFile << eulerEqn->density(w[i]) << ","<< eulerEqn->velocity(w[i]) << "," << eulerEqn->pressure(w[i]) << "," << eulerEqn->internalEnergy(w[i]) << std::endl;
    }
        
    writeToFile.close();
}

void saveNozzle(std::string fileName, std::vector<Vector3> w, std::shared_ptr<EulerEquations> eulerEqn, std::shared_ptr<Mesh> mesh, double iter)
{
    std::ofstream writeToFile(fileName);
    writeToFile << mesh->getFirstX() << std::endl;
    writeToFile << mesh->getDx() << std::endl;
    writeToFile << mesh->getCells() << std::endl;
    writeToFile << iter << std::endl;

    for (int i = 0; i < w.size(); i++)
    {
        writeToFile << eulerEqn->density(w[i]) << ","<< eulerEqn->velocity(w[i]) << "," << eulerEqn->pressure(w[i]) << "," << (eulerEqn->velocity(w[i])/eulerEqn->soundSpeed(w[i])) << std::endl;
    }
        
    writeToFile.close();
}

int main(int argc, char** argv)
{
    std::shared_ptr<Nozzle> mesh = std::make_shared<Nozzle>();

    std::shared_ptr<EulerEquations> eulerEqn = std::make_shared<EulerEquations>(1.4, 287.05);
    std::shared_ptr<RiemannSolver> riemannSolver = std::make_shared<Hll>(eulerEqn);

    //FVM schemes
    std::shared_ptr<SlopeLimiter> limiter = std::make_shared<Minmod>();
    std::shared_ptr<SpatialScheme> spcScheme = std::make_shared<Muscl>(riemannSolver, limiter, eulerEqn);
    std::shared_ptr<TemporalScheme> tmpScheme = std::make_shared<ExplicitHeun>(spcScheme);
    /*std::shared_ptr<SpatialScheme> spcScheme = std::make_shared<Godunov>(riemannSolver, eulerEqn);
    std::shared_ptr<TemporalScheme> tmpScheme = std::make_shared<ExplicitEuler>(spcScheme);*/

    //boundary condition
    mesh->setInlet(std::make_shared<PressureDensityInlet>(100000.0, 1.2, eulerEqn));
    mesh->setOutlet(std::make_shared<PressureOutlet>(80000.0, eulerEqn));
    

    Solver mySolver = Solver(eulerEqn, mesh, spcScheme, tmpScheme);

    //initial condition    
    //Vector3 init = eulerEqn->tempVeloPressToConservative(Vector3({293.15, 0.0, 100000.0}));
    Vector3 init = Vector3({1.0, 0.0, 250000.0});

    std::vector<Vector3> w;

    //solver init
    w = mySolver.calcInitialCondition(init);

    //cfl 0.8
    std::vector<Vector3> wn = mySolver.solve(w, mesh, 20000, 0.8, "residueMinmod.txt");

    saveNozzle("nozzleMinmod.txt", wn, eulerEqn, mesh, 20000);

    return 0;
}
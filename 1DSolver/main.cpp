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
#include "TemporalScheme.hpp"
#include "ExplicitEuler.hpp"
#include "ExplicitRK2.hpp"
#include "SourceTerm.hpp"
#include "NozzleSourceTerm.hpp"
#include "Mesh.hpp"
#include "NozzleGeometry.hpp"
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
    writeToFile << mesh->getCells() + 2 << std::endl;
    writeToFile << iter << std::endl;

    for (int i = 1; i < w.size()-1; i++)
    {
        writeToFile << eulerEqn->density(w[i]) << ","<< eulerEqn->velocity(w[i]) << "," << eulerEqn->pressure(w[i]) << "," << (eulerEqn->velocity(w[i])/eulerEqn->soundSpeed(w[i])) << std::endl;
    }
        
    writeToFile.close();
}

int main(int argc, char** argv)
{
    std::shared_ptr<NozzleGeometry> geometry = std::make_shared<NozzleGeometry>();

    std::shared_ptr<EulerEquations> eulerEqn = std::make_shared<EulerEquations>(1.4, 287.05);
    std::shared_ptr<RiemannSolver> riemannSolver = std::make_shared<Hllc>(eulerEqn);
    std::shared_ptr<SourceTerm> sourceTerm = std::make_shared<NozzleSourceTerm>(eulerEqn, geometry);

    //FVM schemes
    std::shared_ptr<SlopeLimiter> limiter = std::make_shared<MonotonizedCentral>();
    std::shared_ptr<SpatialScheme> spcScheme = std::make_shared<Muscl>(riemannSolver, limiter);
    std::shared_ptr<TemporalScheme> tmpScheme = std::make_shared<ExplicitRK2>(spcScheme);
    /*std::shared_ptr<SpatialScheme> spcScheme = std::make_shared<Godunov>(riemannSolver);
    std::shared_ptr<TemporalScheme> tmpScheme = std::make_shared<ExplicitEuler>(spcScheme);*/

    //boundary condition
    std::shared_ptr<BoundaryCondition> inlet = std::make_shared<PressureDensityInlet>(100000.0, 1.2, eulerEqn);
    std::shared_ptr<BoundaryCondition> outlet = std::make_shared<PressureOutlet>(80000.0, eulerEqn);

    Solver mySolver = Solver(eulerEqn, geometry, spcScheme, tmpScheme);

    //initial condition    
    //Vector3 init = eulerEqn->tempVeloPressToConservative(Vector3({293.15, 0.0, 100000.0}));
    Vector3 init = Vector3({1.0, 0.0, 250000.0});

    std::vector<Vector3> w;

    //solver init
    w = mySolver.calcInitialCondition(init, inlet, outlet);

    //cfl 0.8
    std::vector<Vector3> wn = mySolver.solve(w, sourceTerm, inlet, outlet, 30000, 0.8);

    saveNozzle("nozzle.txt", wn, eulerEqn, geometry, 30000);

    return 0;
}
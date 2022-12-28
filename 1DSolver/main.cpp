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
#include "SpatialScheme.hpp"
#include "Godunov.hpp"
#include "Muscl.hpp"
#include "RiemannSolver.hpp"
#include "Hllc.hpp"
#include "TemporalScheme.hpp"
#include "ExplicitEuler.hpp"
//#include "SourceTerm.hpp"
#include "Mesh.hpp"

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

int main(int argc, char** argv)
{    
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(1.0, 100, 0.0);

    std::shared_ptr<EulerEquations> eulerEqn = std::make_shared<EulerEquations>(1.4, 286);
    std::shared_ptr<RiemannSolver> riemannSolver = std::make_shared<Hllc>(eulerEqn);
    std::shared_ptr<SlopeLimiter> limiter = std::make_shared<VanAlbada>();
    std::shared_ptr<SpatialScheme> spcScheme = std::make_shared<Muscl>(riemannSolver, limiter);
    std::shared_ptr<TemporalScheme> tmpScheme = std::make_shared<ExplicitEuler>(spcScheme);

    Solver mySolver = Solver(eulerEqn, mesh, spcScheme, tmpScheme);

    std::vector<Vector3> w;
    

    Vector3 wl = eulerEqn->primitiveToConservative(Vector3({1.0, 0.0, 1.0}));
    Vector3 wr = eulerEqn->primitiveToConservative(Vector3({0.125, 0, 0.1}));
    
    w = mySolver.calcRiemannInitialCondition(wl, wr);

    std::vector<Vector3> wn = mySolver.solve(w, 500, 0.25, 0.4);

    saveRiemann("results.txt", wn, eulerEqn, mesh, 0.25);

    return 0;
}
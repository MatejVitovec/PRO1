#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <memory>

#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "Solver.hpp"
#include "SpatialScheme.hpp"
#include "Godunov.hpp"
#include "RiemannSolver.hpp"
#include "Hllc.hpp"
#include "TemporalScheme.hpp"
#include "ExplicitEuler.hpp"
#include "SourceTerm.hpp"
#include "Mesh.hpp"



int main(int argc, char** argv)
{
    
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(1.0, 400, 0.0);

    std::shared_ptr<EulerEquations> eulerEqn = std::make_shared<EulerEquations>(1.4, 286);
    std::shared_ptr<RiemannSolver> riemannSolver = std::make_shared<Hllc>(eulerEqn);
    std::shared_ptr<SpatialScheme> spcScheme = std::make_shared<Godunov>(riemannSolver);
    std::shared_ptr<SourceTerm> sourceTerm = std::make_shared<SourceTerm>();
    std::shared_ptr<TemporalScheme> tmpScheme = std::make_shared<ExplicitEuler>(spcScheme, sourceTerm);

    Solver mySolver = Solver(eulerEqn, mesh, spcScheme, tmpScheme);

    std::vector<Vector3> w;
    

    Vector3 wl = eulerEqn->primitiveToConservative(Vector3({1.0, 0.75, 1.0}));
    Vector3 wr = eulerEqn->primitiveToConservative(Vector3({0.125, 0, 0.1}));
    
    w = mySolver.calcRiemannInitialCondition(wl, wr);

    std::vector<Vector3> wn = mySolver.solve(w, 200, 0.8);

    int a = 5;

    return 0;
}
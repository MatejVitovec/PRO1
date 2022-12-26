#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <memory>

#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "RiemannSolver.hpp"
#include "Hllc.hpp"

#include "Euler1D.hpp"
#include "HLLCFlux.hpp"


using namespace Euler1D;
using namespace HLLCFlux;

int main(int argc, char** argv)
{
    GAMMA = 1.4;

    std::shared_ptr<EulerEquations> eulerEqn =  std::make_shared<EulerEquations>(1.4, 286);
    std::shared_ptr<RiemannSolver> riemannSolver =  std::make_shared<Hllc>(eulerEqn);

    Vector3 ul = Vector3({1.0, 0.75, 1.0});
    Vector3 ur = Vector3({0.125, 0.0, 0.1});

    Vector3 wl = primitiveToConservative(ul);
    Vector3 wr = primitiveToConservative(ur);

    Vector3 out = riemannSolver->calculateFlux(wl, wr);
    Vector3 out2 = HLLC(wl, wr);

    std::cout << out[0] << ", " << out[1] << ", " << out[2] << std::endl;
    std::cout << out2[0] << ", " << out2[1] << ", " << out2[2] << std::endl;

    return 0;
}
#include <cmath>
#include "PressureDensityInlet.hpp"


PressureDensityInlet::PressureDensityInlet()
{

}

PressureDensityInlet::PressureDensityInlet(double totPress, double totRho)
{
    setTotalPressure(totPress);
    setTotalDensity(totRho);
}

PressureDensityInlet::PressureDensityInlet(double totPress, double totRho, std::shared_ptr<EulerEquations> euler)
{
    setTotalPressure(totPress);
    setTotalDensity(totRho);
    setEquationModel(euler);
}

void PressureDensityInlet::setTotalPressure(double totPress)
{
    totalPressure = totPress;
}

void PressureDensityInlet::setTotalDensity(double totRho)
{
    totalDensity = totRho;
}

Vector3 PressureDensityInlet::calcBoundaryState(const std::vector<Vector3>& w) const
{
    double gamma = eulerEqn->getGamma();
    double r = eulerEqn->getR();

    double p = std::min(eulerEqn->pressure(w[0]), totalPressure);
    double m2 = (2.0/(gamma - 1.0))*(pow((p/totalPressure), ((1.0 - gamma)/gamma)) - 1.0);
    double rho = totalDensity*pow((1.0 + ((gamma - 1.0)/2.0)*m2), (1.0/(1.0 - gamma)));
    double u = sqrt(m2*((gamma*p)/rho));

    return eulerEqn->primitiveToConservative(Vector3({rho, u, p}));
}
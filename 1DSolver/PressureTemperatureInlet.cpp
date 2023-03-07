#include <cmath>
#include "PressureTemperatureInlet.hpp"


PressureTemperatureInlet::PressureTemperatureInlet()
{

}

PressureTemperatureInlet::PressureTemperatureInlet(double totPress, double totTemp)
{
    setTotalPressure(totPress);
    setTotalTemperature(totTemp);
}


PressureTemperatureInlet::PressureTemperatureInlet(double totPress, double totTemp, std::shared_ptr<EulerEquations> euler)
{
    setTotalPressure(totPress);
    setTotalTemperature(totTemp);
    setEquationModel(euler);
}

void PressureTemperatureInlet::setTotalPressure(double totPress)
{
    totalPressure = totPress;
}

void PressureTemperatureInlet::setTotalTemperature(double totTemp)
{
    totalTemperature = totTemp;
}

Vector3 PressureTemperatureInlet::calcBoundaryState(const std::vector<Vector3>& w) const
{
    double gamma = eulerEqn->getGamma();
    double r = eulerEqn->getR();

    double p = std::min(eulerEqn->pressure(w[0]), totalPressure);
    double m2 = (2.0/(gamma - 1.0))*(pow((p/totalPressure), ((1.0 - gamma)/gamma)) - 1.0);
    double temp = totalTemperature/(1.0 + ((gamma - 1.0)/2.0)*m2);
    double rho = p/(temp*r);
    double u = sqrt(m2*((gamma*p)/rho));

    return eulerEqn->primitiveToConservative(Vector3({rho, u, p}));
}
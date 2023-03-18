#include <cmath>
#include "nonLinearPressureTemperatureInlet.hpp"


nonLinearPressureTemperatureInlet::nonLinearPressureTemperatureInlet()
{

}

nonLinearPressureTemperatureInlet::nonLinearPressureTemperatureInlet(double totPress, double totTemp)
{
    setTotalPressure(totPress);
    setTotalTemperature(totTemp);
}


nonLinearPressureTemperatureInlet::nonLinearPressureTemperatureInlet(double totPress, double totTemp, std::shared_ptr<EulerEquations> euler)
{
    setTotalPressure(totPress);
    setTotalTemperature(totTemp);
    setEquationModel(euler);
}

void nonLinearPressureTemperatureInlet::setTotalPressure(double totPress)
{
    totalPressure = totPress;
}

void nonLinearPressureTemperatureInlet::setTotalTemperature(double totTemp)
{
    totalTemperature = totTemp;
}

Vector3 nonLinearPressureTemperatureInlet::calcBoundaryState(const std::vector<Vector3>& w) const
{
    double gamma = eulerEqn->getGamma();
    double r = eulerEqn->getR();

    double rho1 = eulerEqn->density(w[0]);
    double p1 = eulerEqn->pressure(w[0]);
    double u1 = eulerEqn->velocity(w[0]);
    double c1 = sqrt((gamma*p1)/rho1);
    double cTot = sqrt(gamma*r*totalTemperature);

    double invar = u1 - 2.0*c1/(gamma - 1.0);
    double c = ((gamma - 1.0)/(gamma + 1.0))*(-invar + sqrt(((gamma + 1.0)/(gamma - 1.0))*cTot*cTot - ((gamma - 1.0)/2.0)*invar*invar));
    double u = invar + (2.0*c)/(gamma - 1.0);
    double M = u/c;
    double p = totalPressure*pow(1.0 + ((gamma - 1.0)/2)*M*M, gamma/(1.0 - gamma));
    double rho = (gamma*p)/(c*c);

    return eulerEqn->primitiveToConservative(Vector3({rho, u, p}));
}
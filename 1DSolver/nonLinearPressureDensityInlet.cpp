#include <cmath>
#include "nonLinearPressureDensityInlet.hpp"


nonLinearPressureDensityInlet::nonLinearPressureDensityInlet()
{

}

nonLinearPressureDensityInlet::nonLinearPressureDensityInlet(double totPress, double totRho)
{
    setTotalPressure(totPress);
    setTotalDensity(totRho);
}

nonLinearPressureDensityInlet::nonLinearPressureDensityInlet(double totPress, double totRho, std::shared_ptr<EulerEquations> euler)
{
    setTotalPressure(totPress);
    setTotalDensity(totRho);
    setEquationModel(euler);
}

void nonLinearPressureDensityInlet::setTotalPressure(double totPress)
{
    totalPressure = totPress;
}

void nonLinearPressureDensityInlet::setTotalDensity(double totRho)
{
    totalDensity = totRho;
}

Vector3 nonLinearPressureDensityInlet::calcBoundaryState(const std::vector<Vector3>& w) const
{
    double gamma = eulerEqn->getGamma();
    double r = eulerEqn->getR();

    double rho1 = eulerEqn->density(w[0]);
    double p1 = eulerEqn->pressure(w[0]);
    double u1 = eulerEqn->velocity(w[0]);
    double c1 = sqrt((gamma*p1)/rho1);
    double cTot = sqrt(gamma*(totalPressure/totalDensity));

    double invar = u1 - 2.0*c1/(gamma - 1.0);
    double c = ((gamma - 1.0)/(gamma + 1.0))*(-invar + sqrt(((gamma + 1.0)/(gamma - 1.0))*cTot*cTot - ((gamma - 1.0)/2.0)*invar*invar));
    double u = invar + (2.0*c)/(gamma - 1.0);
    double M = u/c;
    double p = totalPressure*pow(1.0 + ((gamma - 1.0)/2)*M*M, gamma/(1.0 - gamma));
    double rho = (gamma*p)/(c*c);

    return eulerEqn->primitiveToConservative(Vector3({rho, u, p}));
}
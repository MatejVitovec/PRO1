#include <cmath>
#include "BoundaryCondition.hpp"
#include "nonLinearPressureOutlet.hpp"


nonLinearPressureOutlet::nonLinearPressureOutlet()
{

}

nonLinearPressureOutlet::nonLinearPressureOutlet(double press)
{
    setPressure(press);
}

nonLinearPressureOutlet::nonLinearPressureOutlet(double press, std::shared_ptr<EulerEquations> euler)
{
    setPressure(press);
    setEquationModel(euler);
}

void nonLinearPressureOutlet::setPressure(double press)
{
    pressure = press;
}


Vector3 nonLinearPressureOutlet::calcBoundaryState(const std::vector<Vector3>& w) const
{
    double gamma = eulerEqn->getGamma();

    Vector3 wn = w[w.size()-1];
    double un = eulerEqn->velocity(wn);    

    if ((un/eulerEqn->soundSpeed(wn)) < 1)
    {
        double rhon = eulerEqn->density(wn);
        double pn = eulerEqn->pressure(wn);
        double cn = sqrt((gamma*pn)/rhon);

        double rho = rhon*pow(pressure/pn, 1.0/gamma);
        double c = sqrt((gamma*pressure)/rho);
        double u = un + 2.0*(cn - c)/(gamma - 1.0);

        return eulerEqn->primitiveToConservative(Vector3({rho, u, pressure}));
    }
    else
    {
        return wn;
    }
}
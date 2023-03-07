#include <cmath>
#include "BoundaryCondition.hpp"
#include "PressureOutlet.hpp"


PressureOutlet::PressureOutlet()
{

}

PressureOutlet::PressureOutlet(double press)
{
    setPressure(press);
}

PressureOutlet::PressureOutlet(double press, std::shared_ptr<EulerEquations> euler)
{
    setPressure(press);
    setEquationModel(euler);
}

void PressureOutlet::setPressure(double press)
{
    pressure = press;
}


Vector3 PressureOutlet::calcBoundaryState(const std::vector<Vector3>& w) const
{
    double gamma = eulerEqn->getGamma();

    Vector3 wn = w[w.size()-1];

    if ((eulerEqn->velocity(wn)/eulerEqn->soundSpeed(wn)) < 1)
    {            
        return eulerEqn->primitiveToConservative(Vector3({eulerEqn->density(wn), eulerEqn->velocity(wn), pressure}));
    }
    else
    {
        return wn;
    }
}
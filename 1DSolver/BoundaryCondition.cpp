#include "BoundaryCondition.hpp"

void BoundaryCondition::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    eulerEqn = euler;
}
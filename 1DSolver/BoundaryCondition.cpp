#include "BoundaryCondition.hpp"

BoundaryCondition::BoundaryCondition()
{

}

void BoundaryCondition::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    eulerEqn = euler;
}
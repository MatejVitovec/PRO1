#include "SpatialScheme.hpp"

void SpatialScheme::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    SpatialScheme::eulerEqn = euler;
}
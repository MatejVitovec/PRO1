#include "SpatialScheme.hpp"

void SpatialScheme::setEquationModel(std::shared_ptr<EulerEquations> eqnIn)
{
    eulerEqn = eqnIn;
}
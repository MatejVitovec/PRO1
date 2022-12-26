#include <cmath>
#include "TemporalScheme.hpp"



void TemporalScheme::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    eulerEqn = euler;
}

void TemporalScheme::setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme)
{
    spaceScheme = spcScheme;
}

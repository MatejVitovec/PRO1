#include <cmath>
#include "TemporalScheme.hpp"



void TemporalScheme::setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme)
{
    spaceScheme = spcScheme;
}

#include <cmath>
#include "TemporalScheme.hpp"



void TemporalScheme::setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme)
{
    spaceScheme = spcScheme;
}

void TemporalScheme::setSourceTerm(std::shared_ptr<SourceTerm> srcTerm)
{
    sourceTerm = srcTerm;
}


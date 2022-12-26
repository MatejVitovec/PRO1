#include <cmath>
#include "TemporalScheme.hpp"


void TemporalScheme::setCfl(double cfl)
{
    TemporalScheme::cfl = cfl;
}

void TemporalScheme::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    eulerEqn = euler;
}

void TemporalScheme::setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme)
{
    spaceScheme = spcScheme;
}

double TemporalScheme::timeStep(std::vector<Vector3> w, double dx) const
{
    double dt = 10e+100;

    for (int i = 0; i < w.size(); i++)
    {
        //min(dt, dx/spectral radius)
        dt = std::min(dt, dx/(fabs(eulerEqn->velocity(w[i])) + eulerEqn->soundSpeed(w[i])));
    }

    return dt;
}
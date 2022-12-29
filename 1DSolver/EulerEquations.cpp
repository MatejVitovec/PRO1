#include <string>
#include <cmath>
#include <vector>
#include "EulerEquations.hpp"

EulerEquations::EulerEquations()
{

}

EulerEquations::EulerEquations(double gamma, double r)
{
    setGamma(gamma);
    setR(r);
}

void EulerEquations::setGamma(double g)
{
    GAMMA = g;
}

void EulerEquations::setR(double r)
{
    R = r;
}

double EulerEquations::getGamma()
{
    return GAMMA;
}

double EulerEquations::getR()
{
    return R;
}

double EulerEquations::density(const Vector3& w) const
{
    return w[RHO];
}

double EulerEquations::velocity(const Vector3& w) const
{
    return w[RHO_U] / w[RHO];
}


double EulerEquations::totalEnergy(const Vector3& w) const
{
    return w[RHO_E] / w[RHO];
}

double EulerEquations::pressure(const Vector3& w) const
{
    return (GAMMA - 1.0)*density(w)*(totalEnergy(w) - 0.5*velocity(w)*velocity(w));
}


double EulerEquations::internalEnergy(const Vector3& w) const
{
    return pressure(w)/((GAMMA - 1.0)*density(w));
}


double EulerEquations::soundSpeed(const Vector3& w) const
{
    return sqrt((GAMMA*pressure(w))/density(w));
}


Vector3 EulerEquations::flux(const Vector3& w) const
{
    double u = velocity(w);
    double p = pressure(w);

    return u * w + Vector3({0.0, p, p*u});
}


Vector3 EulerEquations::primitiveToConservative(const Vector3& v) const
{
    return Vector3({v[0], v[0]*v[1], 0.5*v[0]*v[1]*v[1] + (v[2])/(GAMMA - 1)});
}

Vector3 EulerEquations::tempVeloPressToConservative(const Vector3& in) const
{
    Vector3 w;

    w[0] = in[2]/(R*in[0]);
    w[1] = in[1]*w[0];
    w[2] = in[2]/(GAMMA - 1);

    return w;
}
#ifndef EULEREQUATIONS_H
#define EULEREQUATIONS_H

#include "Vector3.hpp"

class EulerEquations
{
    public:
        enum {RHO, RHO_U, RHO_E};

        EulerEquations();
        EulerEquations(double gamma, double r);

        void setGamma(double g);
        void setR(double r);
        double getGamma();
        double getR();

        double density(const Vector3& w) const;
        double velocity(const Vector3& w) const;
        double totalEnergy(const Vector3& w) const;
        double pressure(const Vector3& w) const;
        double internalEnergy(const Vector3& w) const;
        double soundSpeed(const Vector3& w) const;
        Vector3 flux(const Vector3& w) const;
        Vector3 primitiveToConservative(const Vector3& v) const;
        Vector3 tempVeloPressToConservative(const Vector3& in) const;

    protected:

        double GAMMA;
        double R;
};

#endif // EULEREQUATIONS_H
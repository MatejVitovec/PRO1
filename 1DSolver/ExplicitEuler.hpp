#ifndef EXPLICITEULER_H
#define EXPLICITEULER_H

#include "Vector3.hpp"
#include "EulerEquations.hpp"

class ExplicitEuler : public TemporalScheme
{
    public:
        ExplicitEuler();
        ExplicitEuler(std::shared_ptr<EulerEquations> eulerEqn, std::shared_ptr<SpatialScheme> spaceScheme, double cfl);

        virtual std::vector<Vector3> Solve(const std::vector<Vector3>& w) const;

    private:

};

#endif // EXPLICITEULER_H
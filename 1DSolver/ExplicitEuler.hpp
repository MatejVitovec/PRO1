#ifndef EXPLICITEULER_H
#define EXPLICITEULER_H

#include "Vector3.hpp"
#include "EulerEquations.hpp"

class ExplicitEuler : public TemporalScheme
{
    public:
        ExplicitEuler();
        ExplicitEuler(std::shared_ptr<SpatialScheme> spaceScheme, std::shared_ptr<SourceTerm> srcTerm);

        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, double dt, double dx) const;

    private:

};

#endif // EXPLICITEULER_H
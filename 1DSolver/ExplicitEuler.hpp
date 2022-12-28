#ifndef EXPLICITEULER_H
#define EXPLICITEULER_H

#include "Vector3.hpp"
#include "EulerEquations.hpp"

class ExplicitEuler : public TemporalScheme
{
    public:
        ExplicitEuler();
        ExplicitEuler(std::shared_ptr<SpatialScheme> spaceScheme);

        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, double dt, double dx) const;
        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, std::shared_ptr<SourceTerm> sourceTerm, double dt, double dx) const;

    private:

};

#endif // EXPLICITEULER_H
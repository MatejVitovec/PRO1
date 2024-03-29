#ifndef EXPLICITEULER_H
#define EXPLICITEULER_H

#include "Vector3.hpp"
#include "EulerEquations.hpp"

class ExplicitEuler : public TemporalScheme
{
    public:
        ExplicitEuler();
        ExplicitEuler(std::shared_ptr<SpatialScheme> spaceScheme);

        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, const double& dt, const double& dx) const;
        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, const double& dt, std::shared_ptr<NozzleGeometry> geometry) const;

    private:

};

#endif // EXPLICITEULER_H
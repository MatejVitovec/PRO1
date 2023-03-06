#ifndef TEMPORALSCHEME_H
#define TEMPORALSCHEME_H

#include <vector>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "SpatialScheme.hpp"
#include "Nozzle.hpp"


class TemporalScheme
{
    public:
        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, const double& dt, const double& dx) const = 0;
        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, const double& dt, std::shared_ptr<Nozzle> mesh) const = 0;
        void setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme);

    protected:
        std::shared_ptr<SpatialScheme> spaceScheme;


};

#endif // TEMPORALSCHEME_H
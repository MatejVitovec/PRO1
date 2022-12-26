#ifndef TEMPORALSCHEME_H
#define TEMPORALSCHEME_H

#include <vector>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "SpatialScheme.hpp"


class TemporalScheme
{
    public:
        virtual std::vector<Vector3> Solve(const std::vector<Vector3>& w) const = 0;
        void setCfl(double cfl);
        void setEquationModel(std::shared_ptr<EulerEquations> euler);
        void setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme);

    protected:
        std::shared_ptr<EulerEquations> eulerEqn;
        std::shared_ptr<SpatialScheme> spaceScheme;

        double cfl;

        virtual double timeStep(std::vector<Vector3> w, double dx) const;
    
};

#endif // TEMPORALSCHEME_H
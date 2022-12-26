#ifndef TEMPORALSCHEME_H
#define TEMPORALSCHEME_H

#include <vector>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "SpatialScheme.hpp"


class TemporalScheme
{
    public:
        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, double dt, double dx) const = 0;
        void setEquationModel(std::shared_ptr<EulerEquations> euler);
        void setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme);

    protected:
        std::shared_ptr<EulerEquations> eulerEqn;
        std::shared_ptr<SpatialScheme> spaceScheme;


        
    
};

#endif // TEMPORALSCHEME_H
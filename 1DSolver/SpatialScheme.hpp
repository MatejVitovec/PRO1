#ifndef SPATIALSCHEME_H
#define SPATIALSCHEME_H

#include <memory>
#include <vector>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "RiemannSolver.hpp"

class SpatialScheme
{
    public:
        virtual std::vector<Vector3> calculateResidues(const std::vector<Vector3>& w) const = 0;
        void setEquationModel(std::shared_ptr<EulerEquations> euler);

    protected:
        std::shared_ptr<EulerEquations> eulerEqn;
        
        virtual Vector3 calculateRes() const = 0;
    
};

#endif // SPATIALSCHEME_H
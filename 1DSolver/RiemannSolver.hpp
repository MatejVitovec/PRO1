#ifndef RIEMANNSOLVER_H
#define RIEMANNSOLVER_H

#include <memory>
#include <vector>
#include "Vector3.hpp"
#include "EulerEquations.hpp"

class RiemannSolver
{
    public:
        void setEquationModel(std::shared_ptr<EulerEquations> eulerEqnIn);
        virtual Vector3 calculateFlux(const Vector3& wl, const Vector3& wr) const = 0;
        std::vector<Vector3> calculateFluxes(const std::vector<Vector3>& wl, const std::vector<Vector3>& wr) const;

    protected:
        std::shared_ptr<EulerEquations> eulerEqn;
    
};

#endif // RIEMANNSOLVER_H
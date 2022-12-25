#ifndef RIEMANNSOLVER_H
#define RIEMANNSOLVER_H

#include <memory>
#include "Vector3.hpp"
#include "EulerEquations.hpp"

class RiemannSolver
{
    public:
        virtual Vector3 calculateFlux(const Vector3& wl, const Vector3& wr) const = 0;

    protected:
        std::shared_ptr<EulerEquations> eulerEqn;
    
};

#endif // RIEMANNSOLVER_H
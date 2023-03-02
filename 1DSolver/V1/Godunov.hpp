#ifndef GODUNOV_H
#define GODUNOV_H

#include <vector>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "RiemannSolver.hpp"

class Godunov : public SpatialScheme
{
    public:
        Godunov();
        Godunov(std::shared_ptr<RiemannSolver> riemann);
        
        void setRiemannSolver(std::shared_ptr<RiemannSolver> riemann);

        virtual std::vector<Vector3> calculateResidues(const std::vector<Vector3>& w, double dx) const;

    private:
        std::shared_ptr<RiemannSolver> riemannSolver;

};

#endif // GODUNOV_H
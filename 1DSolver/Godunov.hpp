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
        Godunov(std::shared_ptr<RiemannSolver> riemann, std::shared_ptr<EulerEquations> equationModel);
        
        void setRiemannSolver(std::shared_ptr<RiemannSolver> riemann);
        virtual void setEquationModel(std::shared_ptr<EulerEquations> euler);

        virtual std::vector<Vector3> calculateResidues(const std::vector<Vector3>& w, double dx) const;
        virtual std::vector<Vector3> calculateResidues(const std::vector<Vector3>& w, std::shared_ptr<Nozzle> mesh) const;

    private:
        std::shared_ptr<RiemannSolver> riemannSolver;

};

#endif // GODUNOV_H
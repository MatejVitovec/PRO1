#ifndef MUSCL_H
#define MUSCL_H

#include <vector>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "RiemannSolver.hpp"
#include "SlopeLimiter.hpp"

class Muscl : public SpatialScheme
{
    public:
        Muscl();
        Muscl(std::shared_ptr<RiemannSolver> riemann, std::shared_ptr<SlopeLimiter> limiter);
        Muscl(std::shared_ptr<RiemannSolver> riemann, std::shared_ptr<SlopeLimiter> limiter, std::shared_ptr<EulerEquations> equationModel);
        
        void setRiemannSolver(std::shared_ptr<RiemannSolver> riemann);
        void setLimiter(std::shared_ptr<SlopeLimiter> limiter);

        virtual std::vector<Vector3> calculateResidues(const std::vector<Vector3>& w, double dx) const;
        virtual std::vector<Vector3> calculateResidues(const std::vector<Vector3>& w, std::shared_ptr<NozzleGeometry> geometry) const;

    private:
        double dx;
        std::shared_ptr<RiemannSolver> riemannSolver;
        std::shared_ptr<SlopeLimiter> limiter;

        Vector3 r(const Vector3& wl, const Vector3& wc, const Vector3& wr) const;

        std::vector<Vector3> calcLimitedSlopes(const std::vector<Vector3>& w) const;
        std::vector<Vector3> calcLStates(const std::vector<Vector3>& w, const std::vector<Vector3>& slopes) const;
        std::vector<Vector3> calcRStates(const std::vector<Vector3>& w, const std::vector<Vector3>& slopes) const;

};

#endif // MUSCL_H
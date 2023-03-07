#ifndef HLL_H
#define HLL_H

#include <vector>
#include "Vector3.hpp"
#include "EulerEquations.hpp"

class Hll : public RiemannSolver
{
    public:
        Hll();
        Hll(std::shared_ptr<EulerEquations> euler);
        
        void setEquationModel(std::shared_ptr<EulerEquations> euler);

        Vector3 calculateFlux(const Vector3& wl, const Vector3& wr) const;

    private:
        Vector3 waveSpeedsEstimate(const Vector3& wl, const Vector3& wr) const;

};

#endif // HLL_H
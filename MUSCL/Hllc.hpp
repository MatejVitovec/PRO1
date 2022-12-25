#ifndef HLLC_H
#define HLLC_H

#include "Vector3.hpp"
#include "EulerEquations.hpp"

class Hllc : public RiemannSolver
{
    public:
        Hllc();
        Hllc(std::shared_ptr<EulerEquations> euler);
        
        void setEquationModel(std::shared_ptr<EulerEquations> euler);

        virtual Vector3 calculateFlux(const Vector3& wl, const Vector3& wr) const;

    private:


        Vector3 waveSpeedsEstimate(const Vector3& wl, const Vector3& wr) const;

};

#endif // HLLC_H
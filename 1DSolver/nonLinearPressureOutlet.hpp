#ifndef NONLINEARPRESSUREOUTLET_H
#define NONLINEARPRESSUREOUTLET_H

#include "Vector3.hpp"
#include "EulerEquations.hpp"

class nonLinearPressureOutlet : public BoundaryCondition
{
    public:
        nonLinearPressureOutlet();
        nonLinearPressureOutlet(double press);
        nonLinearPressureOutlet(double press, std::shared_ptr<EulerEquations> euler);

        void setPressure(double press);

        virtual Vector3 calcBoundaryState(const std::vector<Vector3>& w) const;

    private:
        double pressure;

};

#endif // NONLINEARPRESSUREOUTLET_H
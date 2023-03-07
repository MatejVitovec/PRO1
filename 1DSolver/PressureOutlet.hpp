#ifndef PRESSUREOUTLET_H
#define PRESSUREOUTLET_H

#include "Vector3.hpp"
#include "EulerEquations.hpp"

class PressureOutlet : public BoundaryCondition
{
    public:
        PressureOutlet();
        PressureOutlet(double press);
        PressureOutlet(double press, std::shared_ptr<EulerEquations> euler);

        void setPressure(double press);

        virtual Vector3 calcBoundaryState(const std::vector<Vector3>& w) const;

    private:
        double pressure;

};

#endif // PRESSUREOUTLET_H
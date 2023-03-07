#ifndef PRESSURETEMPERATUREINLET_H
#define PRESSURETEMPERATUREINLET_H

#include "Vector3.hpp"
#include "BoundaryCondition.hpp"

class PressureTemperatureInlet : public BoundaryCondition
{
    public:
        PressureTemperatureInlet();
        PressureTemperatureInlet(double totPress, double totTemp);
        PressureTemperatureInlet(double totPress, double totTemp, std::shared_ptr<EulerEquations> euler);

        void setTotalPressure(double totPress);
        void setTotalTemperature(double totTemp);

        virtual Vector3 calcBoundaryState(const std::vector<Vector3>& w) const;

    private:
        double totalPressure;
        double totalTemperature;

};

#endif // PRESSURETEMPERATUREINLET_H
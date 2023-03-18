#ifndef NONLINEARPRESSURETEMPERATUREINLET_H
#define NONLINEARPRESSURETEMPERATUREINLET_H

#include "Vector3.hpp"
#include "BoundaryCondition.hpp"

class nonLinearPressureTemperatureInlet : public BoundaryCondition
{
    public:
        nonLinearPressureTemperatureInlet();
        nonLinearPressureTemperatureInlet(double totPress, double totTemp);
        nonLinearPressureTemperatureInlet(double totPress, double totTemp, std::shared_ptr<EulerEquations> euler);

        void setTotalPressure(double totPress);
        void setTotalTemperature(double totTemp);

        virtual Vector3 calcBoundaryState(const std::vector<Vector3>& w) const;

    private:
        double totalPressure;
        double totalTemperature;

};

#endif // NONLINEARPRESSURETEMPERATUREINLET_H
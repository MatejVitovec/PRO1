#ifndef PRESSUREDENSITYINLET_H
#define PRESSUREDENSITYINLET_H

#include "Vector3.hpp"
#include "BoundaryCondition.hpp"

class PressureDensityInlet : public BoundaryCondition
{
    public:
        PressureDensityInlet();
        PressureDensityInlet(double totPress, double totRho);
        PressureDensityInlet(double totPress, double totRho, std::shared_ptr<EulerEquations> euler);

        void setTotalPressure(double totPress);
        void setTotalDensity(double totRho);

        virtual Vector3 calcBoundaryState(const std::vector<Vector3>& w) const;

    private:
        double totalPressure;
        double totalDensity;

};

#endif // PRESSUREDENSITYINLET_H
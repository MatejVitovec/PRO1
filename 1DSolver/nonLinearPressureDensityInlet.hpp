#ifndef NONLINEARPRESSUREDENSITYINLET_H
#define NONLINEARPRESSUREDENSITYINLET_H

#include "Vector3.hpp"
#include "BoundaryCondition.hpp"

class nonLinearPressureDensityInlet : public BoundaryCondition
{
    public:
        nonLinearPressureDensityInlet();
        nonLinearPressureDensityInlet(double totPress, double totRho);
        nonLinearPressureDensityInlet(double totPress, double totRho, std::shared_ptr<EulerEquations> euler);

        void setTotalPressure(double totPress);
        void setTotalDensity(double totRho);

        virtual Vector3 calcBoundaryState(const std::vector<Vector3>& w) const;

    private:
        double totalPressure;
        double totalDensity;

};

#endif // NONLINEARPRESSUREDENSITYINLET_H
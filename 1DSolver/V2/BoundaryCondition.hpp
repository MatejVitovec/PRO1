#ifndef BOUNDARYCONDITION_H
#define BOUNDARYCONDITION_H

#include <vector>
#include <memory>
#include "Vector3.hpp"
#include "EulerEquations.hpp"


class BoundaryCondition
{
    public:
        BoundaryCondition();

        void setEquationModel(std::shared_ptr<EulerEquations> euler);

        virtual Vector3 calcBoundaryState(const std::vector<Vector3>& w) const = 0;

    protected:
        std::shared_ptr<EulerEquations> eulerEqn;

};

#endif // BOUNDARYCONDITION_H
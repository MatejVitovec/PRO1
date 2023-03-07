#ifndef SPATIALSCHEME_H
#define SPATIALSCHEME_H

#include <memory>
#include <vector>
#include "Vector3.hpp"
#include "Nozzle.hpp"
#include "EulerEquations.hpp"

class SpatialScheme
{
    public:
        virtual std::vector<Vector3> calculateResidues(const std::vector<Vector3>& w, double dx) const = 0;
        virtual std::vector<Vector3> calculateResidues(const std::vector<Vector3>& w, std::shared_ptr<Nozzle> nozzle) const = 0;

        virtual void setEquationModel(std::shared_ptr<EulerEquations> euler);

    protected:
        std::shared_ptr<EulerEquations> eulerEqn;
};

#endif // SPATIALSCHEME_H
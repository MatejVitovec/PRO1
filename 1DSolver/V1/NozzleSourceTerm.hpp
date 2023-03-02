#ifndef NOZZLESOURCETERM_H
#define NOZZLESOURCETERM_H

#include <vector>
#include <memory>
#include "Vector3.hpp"
#include "SourceTerm.hpp"
#include "EulerEquations.hpp"
#include "NozzleGeometry.hpp"

class NozzleSourceTerm : public SourceTerm
{
    public:
        NozzleSourceTerm();
        NozzleSourceTerm(std::shared_ptr<EulerEquations> euler, std::shared_ptr<NozzleGeometry> geom);

        void setEquationModel(std::shared_ptr<EulerEquations> euler);
        void setGeometry(std::shared_ptr<NozzleGeometry> geom);

        virtual std::vector<Vector3> calculateSourceResidues(const std::vector<Vector3>& w);

    private:
        std::shared_ptr<EulerEquations> eulerEqn;
        std::shared_ptr<NozzleGeometry> geometry;
};

#endif // SOURCETERM_H
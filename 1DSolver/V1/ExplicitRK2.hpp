#ifndef EXPLICITRK2_H
#define EXPLICITRK2_H

#include "Vector3.hpp"
#include "EulerEquations.hpp"

//Collatz, midpoint runge kutta

class ExplicitRK2 : public TemporalScheme
{
    public:
        ExplicitRK2();
        ExplicitRK2(std::shared_ptr<SpatialScheme> spaceScheme);

        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, const double& dt, const double& dx) const;
        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, const std::shared_ptr<SourceTerm>& sourceTerm, const double& dt, const double& dx) const;

    private:

};

#endif // EXPLICITRK2_H
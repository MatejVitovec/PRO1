#ifndef EXPLICITHEUN_H
#define EXPLICITHEUN_H

#include "Vector3.hpp"
#include "EulerEquations.hpp"

class ExplicitHeun : public TemporalScheme
{
    public:
        ExplicitHeun();
        ExplicitHeun(std::shared_ptr<SpatialScheme> spaceScheme);

        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, const double& dt, const double& dx) const;
        virtual std::vector<Vector3> solve(const std::vector<Vector3>& w, const double& dt, std::shared_ptr<Nozzle> mesh) const;

    private:

};

#endif // EXPLICITHEUN_H
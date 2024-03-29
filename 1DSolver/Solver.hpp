#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <memory>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "Mesh.hpp"
#include "Nozzle.hpp"
#include "SpatialScheme.hpp"
#include "TemporalScheme.hpp"
#include "BoundaryCondition.hpp"


class Solver
{
    public:
        Solver();
        Solver(std::shared_ptr<EulerEquations> euler, std::shared_ptr<SpatialScheme> spcScheme, std::shared_ptr<TemporalScheme> tmpScheme);
        
        void setEquationModel(std::shared_ptr<EulerEquations> euler);
        void setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme);
        void setTemporalScheme(std::shared_ptr<TemporalScheme> tmpScheme);
        std::vector<double> getDensityResidues();

        std::vector<Vector3> calcInitialCondition(Vector3 wInit, std::shared_ptr<Mesh> mesh);
        std::vector<Vector3> calcInitialCondition(Vector3 wl, Vector3 wr, std::shared_ptr<Mesh> mesh);

        std::vector<Vector3> solve(std::vector<Vector3> w, const double& dx, const int& iter, const double& targetTime, const double& cfl);
        std::vector<Vector3> solve(std::vector<Vector3> w, std::shared_ptr<Nozzle> mesh, const int& iter, const double& cfl);


    protected:
        std::shared_ptr<EulerEquations> eulerEqn;
        std::shared_ptr<SpatialScheme> spaceScheme;
        std::shared_ptr<TemporalScheme> temporalScheme;

        double timeStep(std::vector<Vector3> w, double dx, double cfl) const;
        double timeStep(std::vector<Vector3> w, double dx, double cfl, double time, double targetTime) const;

        double calcDensityResidue(std::vector<Vector3> w, std::vector<Vector3> wn, double dt);

        std::vector<double> densityResidue;

};

#endif // SOLVER_H
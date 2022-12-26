#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <memory>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "Mesh.hpp"
#include "SpatialScheme.hpp"
#include "TemporalScheme.hpp"


class Solver
{
    public:
        Solver();
        Solver(std::shared_ptr<EulerEquations> euler, std::shared_ptr<Mesh> mesh, std::shared_ptr<SpatialScheme> spcScheme, std::shared_ptr<TemporalScheme> tmpScheme);

        
        void setEquationModel(std::shared_ptr<EulerEquations> euler);
        void setMesh(std::shared_ptr<Mesh> mesh);
        void setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme);
        void setTemporalScheme(std::shared_ptr<TemporalScheme> tmpScheme);

        std::vector<Vector3> solveOneIter(const std::vector<Vector3>& w, double cfl) const;
        std::vector<Vector3> stream(const std::vector<Vector3>& w, Vector3 inlet, Vector3 outlet) const;
        std::vector<Vector3> solve(std::vector<Vector3> w, const int& iter, const double& cfl) const;


    protected:
        std::shared_ptr<EulerEquations> eulerEqn;
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<SpatialScheme> spaceScheme;
        std::shared_ptr<TemporalScheme> temporalScheme;

        double timeStep(std::vector<Vector3> w, double dx, double cfl) const;

};

#endif // SOLVER_H
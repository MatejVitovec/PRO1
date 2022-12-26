#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <memory>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "Mesh.hpp"
#include "SpatialScheme.hpp"
#include "TemporalScheme.hpp"
#include "BoundaryCondition.hpp"


class Solver
{
    public:
        Solver();
        Solver(std::shared_ptr<EulerEquations> euler, std::shared_ptr<Mesh> mesh, std::shared_ptr<SpatialScheme> spcScheme, std::shared_ptr<TemporalScheme> tmpScheme);

        
        void setEquationModel(std::shared_ptr<EulerEquations> euler);
        void setMesh(std::shared_ptr<Mesh> mesh);
        void setSpatialScheme(std::shared_ptr<SpatialScheme> spcScheme);
        void setTemporalScheme(std::shared_ptr<TemporalScheme> tmpScheme);

        std::vector<Vector3> overwriteBC(std::vector<Vector3> w, std::shared_ptr<BoundaryCondition> inlet, std::shared_ptr<BoundaryCondition> outlet) const;
        std::vector<Vector3> solve(std::vector<Vector3> w, std::shared_ptr<BoundaryCondition> inlet, std::shared_ptr<BoundaryCondition> outlet, const int& iter, const double& cfl) const;


    protected:
        std::shared_ptr<EulerEquations> eulerEqn;
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<SpatialScheme> spaceScheme;
        std::shared_ptr<TemporalScheme> temporalScheme;

        double timeStep(std::vector<Vector3> w, double dx, double cfl) const;

};

#endif // SOLVER_H
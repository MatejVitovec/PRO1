#include <cmath>
#include "SpatialScheme.hpp"
#include "Godunov.hpp"
#include "BoundaryCondition.hpp"

Godunov::Godunov()
{

}

Godunov::Godunov(std::shared_ptr<RiemannSolver> riemann)
{
    setRiemannSolver(riemann);
}

Godunov::Godunov(std::shared_ptr<RiemannSolver> riemann, std::shared_ptr<EulerEquations> equationModel)
{
    setRiemannSolver(riemann);
    setEquationModel(equationModel);    
}

void Godunov::setRiemannSolver(std::shared_ptr<RiemannSolver> riemann)
{
    Godunov::riemannSolver = riemann;
}

void Godunov::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    eulerEqn = euler;
    Godunov::riemannSolver->setEquationModel(eulerEqn);
}


std::vector<Vector3> Godunov::calculateResidues(const std::vector<Vector3>& w, double dx) const
{
    std::vector<Vector3> res;

    std::vector<Vector3> f = riemannSolver->calculateFluxes(w, w);
    
    f.insert(f.begin(), riemannSolver->calculateFlux(w[0], w[0]));
    f.push_back(riemannSolver->calculateFlux(w[w.size()-1], w[w.size()-1]));;

    for (int i = 0; i < w.size(); i++)
    {
        res.push_back((f[i] - f[i+1])/dx);
    }

    return res;
}

std::vector<Vector3> Godunov::calculateResidues(const std::vector<Vector3>& w, std::shared_ptr<Nozzle> nozzle) const
{
    double dx = nozzle->getDx();
    std::vector<double> area = nozzle->getArea();
    std::vector<double> areaFaces = nozzle->getAreaFaces();
    std::vector<double> areaDiff = nozzle->getAreaDiff();

    std::shared_ptr<BoundaryCondition> inlet = nozzle->getInlet();
    std::shared_ptr<BoundaryCondition> outlet = nozzle->getOutlet();
    inlet->setEquationModel(eulerEqn);
    outlet->setEquationModel(eulerEqn);

    std::vector<Vector3> res;

    std::vector<Vector3> wp = w;
    wp.insert(wp.begin(), inlet->calcBoundaryState(w));
    wp.push_back(outlet->calcBoundaryState(w));

    std::vector<Vector3> f = riemannSolver->calculateFluxes(wp, wp);

    for (int i = 0; i < w.size(); i++)
    {
        res.push_back(((areaFaces[i]*f[i] - areaFaces[i+1]*f[i+1])/dx + Vector3({0.0, (eulerEqn->pressure(w[i]))*areaDiff[i], 0.0}))/area[i]);
    }

    return res;
}
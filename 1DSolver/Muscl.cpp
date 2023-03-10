#include <cmath>
#include "SpatialScheme.hpp"
#include "Muscl.hpp"

Muscl::Muscl()
{

}

Muscl::Muscl(std::shared_ptr<RiemannSolver> riemann, std::shared_ptr<SlopeLimiter> limiter)
{
    setRiemannSolver(riemann);
    setLimiter(limiter);
}

Muscl::Muscl(std::shared_ptr<RiemannSolver> riemann, std::shared_ptr<SlopeLimiter> limiter, std::shared_ptr<EulerEquations> equationModel)
{    
    setLimiter(limiter);
    setRiemannSolver(riemann);
    setEquationModel(equationModel);
}

void Muscl::setRiemannSolver(std::shared_ptr<RiemannSolver> riemann)
{
    Muscl::riemannSolver = riemann;
}

void Muscl::setLimiter(std::shared_ptr<SlopeLimiter> limiter)
{
    Muscl::limiter = limiter;
}

void Muscl::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    eulerEqn = euler;    
    Muscl::riemannSolver->setEquationModel(eulerEqn);
}


Vector3 Muscl::r(const Vector3& wl, const Vector3& wc, const Vector3& wr) const
{
    Vector3 out;

    if((wc[0] - wl[0]) == 0.0)
    {
        out[0] = 0;
    }
    else
    {
        out[0] = (wc[0] - wl[0])/(wr[0] - wc[0]);
    }

    if((wc[1] - wl[1]) == 0.0)
    {
        out[1] = 0;
    }
    else
    {
        out[1] = (wc[1] - wl[1])/(wr[1] - wc[1]);
    }

    if((wc[2] - wl[2]) == 0.0)
    {
        out[2] = 0;
    }
    else
    {
        out[2] = (wc[2] - wl[2])/(wr[2] - wc[2]);
    }

    return out;
}


std::vector<Vector3> Muscl::calcLimitedSlopes(const std::vector<Vector3>& w) const
{
    int n = w.size();
    std::vector<Vector3> out;    

    out.push_back(w[1] - w[0]);

    for (int i = 1; i < n-1; i++)
    {
        Vector3 lim = limiter->calc(r(w[i-1], w[i], w[i+1]));

        out.push_back(lim*(w[i+1] - w[i]));
    }

    out.push_back(w[n-1] - w[n-2]);
    
    return out;
}


std::vector<Vector3> Muscl::calcLStates(const std::vector<Vector3>& w, const std::vector<Vector3>& slopes) const
{
    std::vector<Vector3> out;

    for (int i = 0; i < w.size(); i++)
    {
        out.push_back(w[i] + 0.5*slopes[i]);
    }
    
    return out;
}


std::vector<Vector3> Muscl::calcRStates(const std::vector<Vector3>& w, const std::vector<Vector3>& slopes) const
{
    std::vector<Vector3> out;

    for (int i = 0; i < w.size(); i++)
    {
        out.push_back(w[i] - 0.5*slopes[i]);
    }
    
    return out;
}


std::vector<Vector3> Muscl::calculateResidues(const std::vector<Vector3>& w, double dx) const
{
    std::vector<Vector3> res;

    std::vector<Vector3> slopes = calcLimitedSlopes(w);
    std::vector<Vector3> wl = calcLStates(w, slopes);
    std::vector<Vector3> wr = calcRStates(w, slopes);

    std::vector<Vector3> f = riemannSolver->calculateFluxes(wl, wr);
    
    f.insert(f.begin(), riemannSolver->calculateFlux(w[0], w[0]));
    f.push_back(riemannSolver->calculateFlux(w[w.size()-1], w[w.size()-1]));

    for (int i = 0; i < w.size(); i++)
    {
        res.push_back((f[i] - f[i+1])/dx);
    }

    return res;
}

std::vector<Vector3> Muscl::calculateResidues(const std::vector<Vector3>& w, std::shared_ptr<Nozzle> nozzle) const
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

    std::vector<Vector3> slopes = calcLimitedSlopes(w);
    std::vector<Vector3> wl = calcLStates(w, slopes);
    std::vector<Vector3> wr = calcRStates(w, slopes);

    std::vector<Vector3> f = riemannSolver->calculateFluxes(wl, wr);
    f.insert(f.begin(), riemannSolver->calculateFlux(inlet->calcBoundaryState(w), w[0]));
    f.push_back(riemannSolver->calculateFlux(w[w.size()-1], outlet->calcBoundaryState(w)));

    /*std::vector<Vector3> wPom = w;
    wPom[0] = w[0] - 1.0*slopes[0];
    wPom[wPom.size()-1] = w[wPom.size()-1] + 1.0*slopes[wPom.size()-1];

    std::vector<Vector3> f = riemannSolver->calculateFluxes(wl, wr);
    f.insert(f.begin(), riemannSolver->calculateFlux(inlet->calcBoundaryState(wPom), w[0]));
    f.push_back(riemannSolver->calculateFlux(w[w.size()-1], outlet->calcBoundaryState(wPom)));*/

    for (int i = 0; i < w.size(); i++)
    {
        res.push_back(((areaFaces[i]*f[i] - areaFaces[i+1]*f[i+1])/dx + Vector3({0.0, (eulerEqn->pressure(w[i]))*areaDiff[i], 0.0}))/area[i]);
    }

    return res;
}
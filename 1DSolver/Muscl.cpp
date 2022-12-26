#include <vector>
#include <cmath>
#include "SpatialScheme.hpp"
#include "Muscl.hpp"

Muscl::Muscl()
{

}

Muscl::Muscl(std::shared_ptr<EulerEquations> euler, std::shared_ptr<RiemannSolver> riemann, std::shared_ptr<SlopeLimiter> limiter)
{
    setEquationModel(euler);
    setRiemannSolver(riemann);
    setLimiter(limiter);
}

void Muscl::setRiemannSolver(std::shared_ptr<RiemannSolver> riemann)
{
    Muscl::riemannSolver = riemann;
}

void Muscl::setLimiter(std::shared_ptr<SlopeLimiter> limiter)
{
    Muscl::limiter = limiter;
}


Vector3 Muscl::r(const Vector3& wl, const Vector3& wc, const Vector3& wr) const
{
    Vector3 out;

    out[0] = (wc[0] - wl[0])/(wr[0] - wc[0]);
    out[1] = (wc[1] - wl[1])/(wr[1] - wc[1]);
    out[1] = (wc[2] - wl[2])/(wr[2] - wc[2]);

    return out;
}


std::vector<Vector3> Muscl::calcLimitedSlopes(const std::vector<Vector3>& w) const
{
    std::vector<Vector3> out;

    out.push_back(Vector3({0.0, 0.0, 0.0}));

    for (int i = 1; i < w.size()-1; i++)
    {
        Vector3 lim = limiter->calc(r(w[i-1], w[i], w[i+1]));

        //(omega = -1)
        out.push_back(lim*(w[i+1] - w[i]));
    }

    out.push_back(Vector3({0.0, 0.0, 0.0}));
    
    return out;
}


std::vector<Vector3> Muscl::calcLStates(const std::vector<Vector3>& w, const std::vector<Vector3>& slopes) const
{
    std::vector<Vector3> out;
    out.push_back(w[0]);
    out.push_back(w[1]);

    for (int i = 2; i < w.size(); i++)
    {
        out.push_back(w[i] - 0.5*slopes[i]);
    }
    
    return out;
}


std::vector<Vector3> Muscl::calcRStates(const std::vector<Vector3>& w, const std::vector<Vector3>& slopes) const
{
    std::vector<Vector3> out;
    int n = w.size();

    for (int i = 0; i < n-2; i++)
    {
        out.push_back(w[i] + 0.5*slopes[i]);
    }
    out.push_back(w[n-2]);
    out.push_back(w[n-1]);
    
    return out;
}


std::vector<Vector3> Muscl::calculateResidues(const std::vector<Vector3> w, double dx) const
{
    std::vector<Vector3> res;

    std::vector<Vector3> slopes = calcLimitedSlopes(w);
    std::vector<Vector3> wl = calcLStates(w, slopes);
    std::vector<Vector3> wr = calcRStates(w, slopes);

    std::vector<Vector3> f = riemannSolver->calculateFluxes(wl, wr);

    res.push_back(Vector3({0.0, 0.0, 0.0})); // inlet void cell

    for (int i = 0; i < w.size()-2; i++)
    {
        res.push_back((f[i] - f[i+1])/(Muscl::dx));
    }
    
    res.push_back(Vector3({0.0, 0.0, 0.0})); // outlet void cell

    return res;
}
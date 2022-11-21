#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "Vector3.hpp"
#include "Euler1D.hpp"
#include "ApproximateFlux.hpp"

using namespace Euler1D;
using namespace ApproximateFlux;

Vector3 riemannInitialCondition(double x, Vector3 wl, Vector3 wr, double initDiscontinuityPos)
{
    if (x < initDiscontinuityPos)
    {
        return wl;
    }
    else
    {
        return wr;
    }
}


double maxTimeStep(const std::vector<Vector3>& w, double dx)
{
    double dt = 10e+100;

    for (int i = 0; i < w.size(); i++)
    {
        dt = std::min(dt, dx/(fabs(velocity(w[i])) + soundSpeed(w[i])));
    }

    return dt;
}


int main(int argc, char** argv)
{
    //TODO


    std::string inputFileName = "cases/case1.txt";
    std::string outputFileName = "results/HLLC.txt";

    if(argc == 3)
    {
        inputFileName = argv[1];
        outputFileName = argv[2];
    }

    double domainLenght;
    double initDiscontinuityPos;
    double n;
    double setTime;
    Vector3 stateL;
    Vector3 stateR;

    loadData(inputFileName, domainLenght, initDiscontinuityPos, n, GAMMA, setTime, stateL, stateR);


    double dx = domainLenght/n;
    double time = 0;

    Vector3 wl = primitiveToConservative(stateL);
    Vector3 wr = primitiveToConservative(stateR);

    std::vector<Vector3> w(n);
    std::vector<Vector3> wn(n);
    std::vector<Vector3> f(n + 1);

    for (int i = 0; i < w.size(); i++)
    {
        w[i] = riemannInitialCondition((i + 0.5)*dx, wl, wr, initDiscontinuityPos);
    }

    bool exitCalcualtion = false;
    int iter = 0;


    while (1)
    {        
        f[0] = HLLC(w[0], w[0]);
        f[n] = HLLC(w[n-1], w[n-1]);

        for (int i = 1; i < n; i++)
        {
            f[i] = HLLC(w[i-1], w[i]);
        }

        //CFL = 0.9
        double dt = 0.9 * maxTimeStep(w, dx);        
        if(setTime - time < dt)
        {
            dt = setTime - time;
            exitCalcualtion = true;
        }
        time += dt;

        for (int i = 0; i < n; i++)
        {
            wn[i] = w[i] - dt/dx * (f[i+1] - f[i]);
        }

        w = wn;

        iter++;

        if(exitCalcualtion)
        {
            break;
        }
    }

    saveData(outputFileName, w, domainLenght, n, setTime);

    std::cout << "Výpočet v čase t = " << time <<" proběhl úspěšně s " << iter << " iteracemi." << std::endl;

    return 0;
}
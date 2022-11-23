#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "Vector3.hpp"
#include "Euler1D.hpp"
#include "HLLCFlux.hpp"

using namespace Euler1D;
using namespace HLLCFlux;

void loadInitBoundaryCondition(std::string fileName, double& lGamma, double& lR, double& time, Vector3& initc, Vector3& boundaryCondition)
{
    std::ifstream file(fileName);
    std::string line;

    std::getline(file, line);
    lGamma = std::stod(line);
    std::getline(file, line);
    lR = std::stod(line);
    std::getline(file, line);
    time = std::stod(line);

    std::getline(file, line);
    initc[0] = std::stod(line);
    std::getline(file, line);
    initc[1] = std::stod(line);
    std::getline(file, line);
    initc[2] = std::stod(line);

    std::getline(file, line);
    boundaryCondition[0] = std::stod(line);
    std::getline(file, line);
    boundaryCondition[1] = std::stod(line);
    std::getline(file, line);
    boundaryCondition[2] = std::stod(line);

    file.close();
}


void saveData(std::string fileName, std::vector<Vector3> w, double firstX, double dx, double cells, double time)
{
    std::ofstream writeToFile(fileName);
    writeToFile << firstX << std::endl;
    writeToFile << dx << std::endl;
    writeToFile << cells << std::endl;
    writeToFile << time << std::endl;

    for (int i = 0; i < w.size(); i++)
    {
        writeToFile << density(w[i]) << ","<< velocity(w[i]) << "," << pressure(w[i]) << "," << (velocity(w[i])/soundSpeed(w[i])) << std::endl;
    }
        
    writeToFile.close();
}


void saveRes(std::string fileName, std::vector<double> history, int dIter)
{
    std::ofstream writeToFile(fileName);
    writeToFile << dIter << std::endl;

    for (int i = 0; i < history.size(); i++)
    {
        writeToFile << history[i] << std::endl;
    }
        
    writeToFile.close();
}


std::vector<double> loadNozzleGeometry(std::string fileName, double& dx, double& firstX, int& cells)
{
    std::vector<double> out;

    std::ifstream inFile(fileName);
    std::string xx;
    std::string yy;
    double lastX;

    inFile >> xx >> yy;
    firstX = std::stod(xx);
    out.push_back(std::stod(yy));

    inFile >> xx >> yy;
    lastX = std::stod(xx);
    out.push_back(std::stod(yy));

    dx = fabs(fabs(lastX) - fabs(firstX));

    while (inFile >> xx >> yy)
    {
        //0.22 = y*2*d(110 mm)
        out.push_back(std::stod(yy)/**0.22*/);        
        double x = std::stod(xx);

        if (ceil(lastX) != ceil(x - dx))
        {
            std::cout << "Vstupni data geometrie nemají stejny krok dx" << std::endl;
        }
        lastX = x;
    }
    inFile.close();

    cells = out.size();

    return out;
}


std::vector<double> loadNozzleGeometryTest(std::string fileName, double& dx, double& firstX, int& cells)
{
    std::vector<double> out;    
    cells = 400;
    dx = 1.0/cells;
    firstX = 0.0;

    double x = firstX;
    for (int i = 0; i < cells; i++)
    {
        out.push_back(1.0 + (x - 0.5)*(x - 0.5));
        x += dx;
    }

    return out;
}


std::vector<double> areaDiff(std::vector<double> area, double dx)
{
    std::vector<double> diff;

    diff.push_back((area[1] - area[0])/dx);

    double vectorSize = area.size();
    for (int i = 1; i < vectorSize - 1; i++)
    {
        diff.push_back((area[i+1] - area[i-1])/(2*dx));
    }
    
    diff.push_back((area[vectorSize - 1] - area[vectorSize - 2])/dx);

    return diff;
}


Vector3 inletPressureTemperature(Vector3 w0, double pTot, double tempTot)
{
    double p = std::min(pressure(w0), pTot);
    double m2 = (2.0/(GAMMA - 1.0))*(pow((p/pTot), ((1.0 - GAMMA)/GAMMA)) - 1.0);
    double temp = tempTot/(1.0 + ((GAMMA - 1.0)/2.0)*m2);
    double rho = p/(temp*R);
    double u = sqrt(m2*((GAMMA*p)/rho));

    return primitiveToConservative(Vector3({rho, u, p}));
}


Vector3 inletPressureDensity(Vector3 w0, double pTot, double rhoTot)
{
    double p = std::min(pressure(w0), pTot);
    if (pressure(w0) < pTot)
    {
        int a = 0;
    }
    p = pTot;
    
    double m2 = (2.0/(GAMMA - 1.0))*(pow((p/pTot), ((1.0 - GAMMA)/GAMMA)) - 1.0);
    double rho = pow(rhoTot*(1.0 + ((GAMMA - 1.0)/2)*m2), 1.0/(1.0 - GAMMA));
    double u = sqrt(m2*GAMMA*(p/rho));

    return primitiveToConservative(Vector3({rho, u, p}));
}


Vector3 outlet(Vector3 wn, double pOut)
{
    if ((velocity(wn)/soundSpeed(wn)) < 1)
    {            
        return primitiveToConservative(Vector3({density(wn), velocity(wn), pOut}));
    }
    else
    {
        return wn;
    }
}


double maxTimeStep(const std::vector<Vector3>& w, double dx)
{
    double dt = 10e+100;

    for (int i = 0; i < w.size(); i++)
    {
        //min(dt, dx/spectral radius)
        dt = std::min(dt, dx/(fabs(velocity(w[i])) + soundSpeed(w[i])));
    }

    return dt;
}


double calcResidueDensity(const std::vector<Vector3> w)
{
    double res = 0;

    int n = w.size();

    for (int i = 1; i < n; i++)
    {
        res += (density(w[i-1]) - density(w[i]));
    }

    return res/n;
}


int main(int argc, char** argv)
{
    std::string inputGeometryName = "cases/profile.txt";
    std::string inputConditionName = "cases/case1.txt";
    std::string outputFileName = "results/case1.txt";

    if(argc == 4)
    {
        inputGeometryName = argv[1];
        inputConditionName = argv[2];
        outputFileName = argv[3];
    }

    double time = 0;
    double setTime;
    double cfl = 0.8;
    Vector3 initC;
    Vector3 boundaryC;
    loadInitBoundaryCondition(inputConditionName, GAMMA, R, setTime, initC, boundaryC);

    double pTotIn = boundaryC[0];
    double tTotIn = boundaryC[1];
    double pOut = boundaryC[2];

    int n;
    double dx;
    double firstX;
    std::vector<double> A = loadNozzleGeometry(inputGeometryName, dx, firstX, n);
    std::vector<double> dA = areaDiff(A, dx);


    std::vector<Vector3> w(n);
    std::vector<Vector3> wn(n);
    std::vector<Vector3> f(n + 1);

    std::vector<double> history;

    Vector3 wInit = tempVeloPressToConservative(initC);
    for (int i = 0; i < w.size(); i++)
    {
        w[i] = wInit;
    }

    bool exitCalculation = false;
    int iter = 0;
    int dIter = 50;

    while (1)
    {
        Vector3 wIn = inletPressureTemperature(w[0], pTotIn, tTotIn);
        Vector3 wOut = outlet(w[n-1], pOut);

        f[0] = HLLC(wIn, w[0]);

        for (int i = 1; i < n; i++)
        {
            f[i] = HLLC(w[i-1], w[i]);
        }

        f[n] = HLLC(w[n-1], wOut);

        double dt = cfl * maxTimeStep(w, dx);
        if(setTime - time < dt)
        {
            dt = setTime - time;
            exitCalculation = true;
        }
        time += dt;

        for (int i = 0; i < n; i++)
        {
            wn[i] = w[i] - (dt/A[i])*dA[i]*(flux(w[i]) - Vector3({0, pressure(w[i]), 0})) - (dt/dx)*(f[i+1] - f[i]);
        }
        
        w = wn;

        if(iter % dIter == 0)
        {
            history.push_back(calcResidueDensity(w));
        }

        ++iter;

        if(exitCalculation || iter >= 20000)
        {
            int a = 0;
            break;
        }
    }

    saveData(outputFileName, w, firstX, dx, n, time);
    saveRes("results/case1Res.txt", history, dIter);

    std::cout << "Výpočet v čase t = " << time <<" proběhl úspěšně s " << iter << " iteracemi." << std::endl;

    return 0;
}
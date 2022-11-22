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
        writeToFile << density(w[i]) << ","<< velocity(w[i]) << "," << pressure(w[i]) << "," << internalEnergy(w[i]) << std::endl;
    }
        
    writeToFile.close();
}


std::vector<double> loadNozzleData(std::string fileName, double& dx, double& firstX, int& cells)
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
        out.push_back(std::stod(yy));
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


Vector3 initialCondition(double x, Vector3 wl, Vector3 wr, double initDiscontinuityPos)
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


Vector3 inlet(Vector3 w0, double pTotIn, double tTotIn)
{
    double mIn = velocity(w0)/soundSpeed(w0);

    if(mIn > 1)
    {
        std::cout << "Na vstupu se objevila nadzvukova rychlost" << std::endl;
    }

    double pIn = pTotIn/pow(1.0 + ((GAMMA - 1.0)/2.0)*mIn*mIn, (GAMMA/(GAMMA - 1)));
    double tIn = tTotIn/(1.0 + ((GAMMA - 1.0)/2.0)*mIn*mIn);

    return tempVeloPressToConservative(Vector3({tIn, velocity(w0), pIn}));
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
        dt = std::min(dt, dx/(fabs(velocity(w[i])) + soundSpeed(w[i])));
    }

    return dt;
}


int main(int argc, char** argv)
{
    std::string inputGeometryName = "cases/profile.txt";
    std::string inputConditionName = "cases/boundaryCondition.txt";
    std::string outputFileName = "results/nozzle1.txt";

    if(argc == 4)
    {
        inputGeometryName = argv[1];
        inputConditionName = argv[2];
        outputFileName = argv[3];
    }

    double time = 0;
    double setTime;
    double cfl = 0.9;;
    Vector3 initC;
    Vector3 boundaryC;
    loadInitBoundaryCondition(inputConditionName, GAMMA, R, setTime, initC, boundaryC);

    double pTotIn = boundaryC[0];
    double tTotIn = boundaryC[2];
    double pOut = boundaryC[3];

    int n;
    double dx;
    double firstX;
    std::vector<double> A = loadNozzleData("cases/profile.txt", dx, firstX, n);
    std::vector<double> dA = areaDiff(A, dx);


    std::vector<Vector3> w(n);
    std::vector<Vector3> wn(n);
    std::vector<Vector3> f(n + 1);

    Vector3 wInit = tempVeloPressToConservative(initC);
    for (int i = 0; i < w.size(); i++)
    {
        w[i] = wInit;
    }

    bool exitCalcualtion = false;
    int iter = 0;

    while (1)
    {

        Vector3 wIn = inlet(w[0], pTotIn, tTotIn);
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
            exitCalcualtion = true;
        }
        time += dt;

        for (int i = 0; i < n; i++)
        {
            wn[i] = w[i] - (dt/A[i])*dA[i]*(flux(w[i]) - Vector3({0, pressure(w[i]), 0})) - (dt/dx)*(f[i+1] - f[i]);
        }
        
        w = wn;

        iter++;

        if(exitCalcualtion)
        {
            break;
        }
    }

    saveData(outputFileName, w, firstX, dx, n, time);

    std::cout << "Výpočet v čase t = " << time <<" proběhl úspěšně s " << iter << " iteracemi." << std::endl;

    return 0;
}
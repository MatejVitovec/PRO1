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

void loadInitBoundaryCondition(std::string fileName, double& lGamma, double& lR, double& time, Vector3& initc, Vector3& inBC, Vector3& outBC)
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
    inBC[0] = std::stod(line);
    std::getline(file, line);
    inBC[1] = std::stod(line);
    std::getline(file, line);
    inBC[2] = std::stod(line);

    std::getline(file, line);
    outBC[0] = std::stod(line);
    std::getline(file, line);
    outBC[1] = std::stod(line);
    std::getline(file, line);
    outBC[2] = std::stod(line);

    file.close();
}


/*void saveData(std::string fileName, std::vector<Vector3> w, double domLen, double cells, double time)
{
    std::ofstream writeToFile(fileName);
    writeToFile << domLen << std::endl;
    writeToFile << cells << std::endl;
    writeToFile << time << std::endl;

    for (int i = 0; i < w.size(); i++)
    {
        writeToFile << density(w[i]) << ","<< velocity(w[i]) << "," << pressure(w[i]) << "," << internalEnergy(w[i]) << std::endl;
    }
        
    writeToFile.close();
}*/


std::vector<double> loadNozzleArea(std::string fileName, double& dx, double& firstX, int& cells)
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
    Vector3 initC;
    Vector3 inBC;
    Vector3 outBC;
    loadInitBoundaryCondition(inputConditionName, GAMMA, R, setTime, initC, inBC, outBC);

    int n;
    double dx;
    double firstX;
    std::vector<double> A = loadNozzleArea("cases/profile.txt", dx, firstX, n);
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

    std::cout << "Výpočet v čase t = " << time <<" proběhl úspěšně s " << iter << " iteracemi." << std::endl;

    return 0;
}
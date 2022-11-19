#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "Vector3.hpp"

enum {RHO, RHO_U, RHO_E};

double GAMMA;

inline double density(const Vector3& w)
{
    return w[RHO];
}


inline double velocity(const Vector3& w)
{
    return w[RHO_U] / w[RHO];
}


inline double totalEnergy(const Vector3& w)
{
    return w[RHO_E] / w[RHO];
}


inline double pressure(const Vector3& w)
{
    return (GAMMA - 1.0)*density(w)*(totalEnergy(w) - 0.5*velocity(w)*velocity(w));
}


inline double soundSpeed(const Vector3& w)
{
    return sqrt((GAMMA*pressure(w))/density(w));
}


inline Vector3 waveSpeeds(const Vector3& w)
{
    double u = velocity(w);
    double a = soundSpeed(w);

    return Vector3({u - a, u, u + a});
}


inline double spectralRadius(const Vector3& w)
{
    return fabs(velocity(w)) + soundSpeed(w);
}


inline Vector3 flux(const Vector3& w)
{
    double u = velocity(w);
    double p = pressure(w);

    return u * w + Vector3({0.0, p, p*u});
}

Vector3 primitiveToConservative(Vector3 v)
{
    return Vector3({v[0], v[0]*v[1], (v[2]/((GAMMA - 1.0)*v[0]))*v[0]});
}


Vector3 riemannInitialCondition(double x, Vector3 stateL, Vector3 stateR, double initDiscontinuityPos)
{
    if (x < initDiscontinuityPos)
    {
        return stateL;
    }
    else
    {
        return stateR;
    }
}

double maxTimeStep(const std::vector<Vector3>& w, double dx)
{
    double dt = 10e+100;

    for (int i = 0; i < w.size(); i++)
    {
        dt = std::min(dt, dx/spectralRadius(w[i]));
    }

    return dt;
}

Vector3 HLL(const Vector3& wl, const Vector3& wr)
{
    Vector3 lambdaL = waveSpeeds(wl);
    Vector3 lambdaR = waveSpeeds(wr);

    double sl = std::min(lambdaL[0], lambdaR[0]);
    double sr = std::max(lambdaL[2], lambdaR[2]);

    if (0 <= sl)
    {
        return flux(wl);
    }
    else if (0 < sr)
    {
        return (sr*flux(wl) - sl*flux(wr) + sl*sr*(wr - wl)) / (sr - sl);
    }
    else
    {
        return flux(wr);
    }
}

Vector3 HLLC(const Vector3& wl, const Vector3& wr)
{
    //TODO

    return Vector3();
}

void loadData(std::string fileName, double& domlen, double& diaph, double& cells, double& lGamma, double& time, Vector3& stateL, Vector3& stateR)
{
    std::ifstream file(fileName);
    std::string line;

    std::getline(file, line);
    domlen = std::stod(line);
    std::getline(file, line);
    diaph = std::stod(line);
    std::getline(file, line);
    cells = std::stod(line);
    std::getline(file, line);
    lGamma = std::stod(line);
    std::getline(file, line);
    time = std::stod(line);

    std::getline(file, line);
    stateL[0] = std::stod(line);
    std::getline(file, line);
    stateL[1] = std::stod(line);
    std::getline(file, line);
    stateL[2] = std::stod(line);

    std::getline(file, line);
    stateR[0] = std::stod(line);
    std::getline(file, line);
    stateR[1] = std::stod(line);
    std::getline(file, line);
    stateR[2] = std::stod(line);

    file.close();
}

void saveData(std::string fileName, std::vector<Vector3> w, double domLen, double cells, double time)
{
    std::ofstream writeToFile(fileName);
    writeToFile << domLen << std::endl;
    writeToFile << cells << std::endl;
    writeToFile << time << std::endl;

    for (int i = 0; i < w.size(); i++)
    {
        writeToFile << density(w[i]) << ","<< velocity(w[i]) << "," << pressure(w[i]) << "," << totalEnergy(w[i]) << std::endl;
    }
    
    writeToFile.close();
}


int main(int argc, char** argv)
{
    std::string inputFileName = "data.txt";
    std::string outputFileName = "results/result.txt";


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

    //saveData(outputFileName, w, domainLenght, n, setTime);

    //predelat na do while
    while (1)
    {
        
        f[0] = HLL(w[0], w[0]);
        f[n] = HLL(w[n-1], w[n-1]);

        for (int i = 1; i < n; i++)
        {
            f[i] = HLL(w[i-1], w[i]);
        }

        double dt = 0.4 * maxTimeStep(w, dx);        
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
        /*for (int i = 0; i < n; i++)
        {
            w[i] = wn[i];
        }*/

        iter++;

        if(exitCalcualtion)
        {
            break;
        }
    }

    saveData(outputFileName, w, domainLenght, n, setTime);

    return 0;
}
#ifndef EULER1D_HPP
#define EULER1D_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "Vector3.hpp"

namespace Euler1D
{
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


    inline double internalEnergy(const Vector3& w)
    {
        return pressure(w)/((GAMMA - 1.0)*density(w));
    }


    inline double soundSpeed(const Vector3& w)
    {
        return sqrt((GAMMA*pressure(w))/density(w));
    }


    inline Vector3 flux(const Vector3& w)
    {
        double u = velocity(w);
        double p = pressure(w);

        return u * w + Vector3({0.0, p, p*u});
    }


    Vector3 primitiveToConservative(Vector3 v)
    {
        return Vector3({v[0], v[0]*v[1], 0.5*v[0]*v[1]*v[1] + (v[2])/(GAMMA - 1)});
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
            writeToFile << density(w[i]) << ","<< velocity(w[i]) << "," << pressure(w[i]) << "," << internalEnergy(w[i]) << std::endl;
        }
        
        writeToFile.close();
    }
}

#endif
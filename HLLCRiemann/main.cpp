//test
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "Vector3.hpp"

enum {RHO, RHO_U, RHO_E};

double GAMMA;

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

void saveData(std::string fileName, std::vector<std::vector<double>> data, double domLen, double cells, double time)
{
    std::ofstream writeToFile(fileName);
    writeToFile << domLen << std::endl;
    writeToFile << cells << std::endl;
    writeToFile << time << std::endl;

    for (int j = 0; j < data.size(); j++)
    {
        for (int i = 0; i < data[j].size(); i++)
        {
            if(i == data[j].size() - 1)
            {
                writeToFile << data[j][i];
            }
            else
            {
                writeToFile << data[j][i] << ",";
            }
        }
        writeToFile << std::endl;
    }
    
    writeToFile.close();
}


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




main()
{
    
    return 0;
}
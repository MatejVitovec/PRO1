#include <iostream>
#include <fstream>
#include <cmath>
#include "NozzleGeometry.hpp"

NozzleGeometry::NozzleGeometry()
{
    createGeometry();
    calcAreaDiff();
}

NozzleGeometry::NozzleGeometry(std::string fileName)
{
    loadGeometry(fileName);
    calcAreaFaces();
    calcAreaDiff();
}


void NozzleGeometry::loadGeometry(std::string fileName)
{
    area.clear();

    std::ifstream inFile(fileName);
    std::string xx;
    std::string yy;
    double lastX;

    inFile >> xx >> yy;
    firstFaceX = std::stod(xx);
    area.push_back(std::stod(yy));

    inFile >> xx >> yy;
    lastX = std::stod(xx);
    area.push_back(std::stod(yy));

    dx = fabs(fabs(lastX) - fabs(firstFaceX));

    while (inFile >> xx >> yy)
    {
        //0.22 = y*2*d(110 mm)
        area.push_back(std::stod(yy)/**0.22*/);        
        double x = std::stod(xx);

        if (ceil(lastX) != ceil(x - dx))
        {
            std::cout << "Vstupni data geometrie nemají stejny krok dx" << std::endl;
        }
        lastX = x;
    }
    inFile.close();

    cells = area.size();
}

void NozzleGeometry::calcAreaFaces()
{
    //todo
}

double NozzleGeometry::areaFunction(double x)
{
    return 1.0 + (x - 0.5)*(x - 0.5);
}

void NozzleGeometry::createGeometry()
{
    area.clear();
    areaFaces.clear();
      
    cells = 50;
    dx = 1.0/cells;
    firstFaceX = 0.0;

    areaFaces.push_back(areaFunction(firstFaceX));

    for (int i = 0; i < cells; i++)
    {
        area.push_back(areaFunction(firstFaceX + i*dx + dx/2.0));
        areaFaces.push_back(areaFunction(firstFaceX + (i+1)*dx));
    }
}

/*void NozzleGeometry::calcAreaDiff()
{
    areaDiff.clear();

    areaDiff.push_back((area[1] - area[0])/dx);

    double vectorSize = area.size();
    for (int i = 1; i < vectorSize - 1; i++)
    {
        areaDiff.push_back((area[i+1] - area[i-1])/(2*dx));
    }
    
    areaDiff.push_back((area[vectorSize - 1] - area[vectorSize - 2])/dx);
}*/

void NozzleGeometry::calcAreaDiff()
{
    areaDiff.clear();

    for (int i = 0; i < area.size(); i++)
    {
        areaDiff.push_back((areaFaces[i+1] - areaFaces[i])/(dx));
    }
}

std::vector<double> NozzleGeometry::getArea()
{
    return area;
}

std::vector<double> NozzleGeometry::getAreaFaces()
{
    return areaFaces;
}

std::vector<double> NozzleGeometry::getAreaDiff()
{
    return areaDiff;
}
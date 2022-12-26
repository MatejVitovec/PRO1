#include <iostream>
#include <fstream>
#include <cmath>
#include "NozzleGeometry.hpp"

NozzleGeometry::NozzleGeometry()
{

}


void NozzleGeometry::loadGeometry(std::string fileName)
{
    area.clear();

    std::ifstream inFile(fileName);
    std::string xx;
    std::string yy;
    double lastX;

    inFile >> xx >> yy;
    firstX = std::stod(xx);
    area.push_back(std::stod(yy));

    inFile >> xx >> yy;
    lastX = std::stod(xx);
    area.push_back(std::stod(yy));

    dx = fabs(fabs(lastX) - fabs(firstX));

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

void NozzleGeometry::createGeometry()
{
    area.clear();
      
    cells = 400;
    dx = 1.0/cells;
    firstX = 0.0;

    double x = firstX;

    for (int i = 0; i < cells; i++)
    {
        area.push_back(1.0 + (x - 0.5)*(x - 0.5));
        x += dx;
    }
}

void NozzleGeometry::calcAreaDiff()
{
    areaDiff.clear();

    areaDiff.push_back((area[1] - area[0])/dx);

    double vectorSize = area.size();
    for (int i = 1; i < vectorSize - 1; i++)
    {
        areaDiff.push_back((area[i+1] - area[i-1])/(2*dx));
    }
    
    areaDiff.push_back((area[vectorSize - 1] - area[vectorSize - 2])/dx);
}

void NozzleGeometry::calcAreaDiffDividedByArea()
{
    areaDiffDividedByArea.clear();

    for (int i = 0; i < area.size(); i++)
    {
        areaDiffDividedByArea.push_back(areaDiff[i]/area[i]);
    }    
}

std::vector<double> NozzleGeometry::getArea()
{
    return area;
}

std::vector<double> NozzleGeometry::getAreaDiff()
{
    return areaDiff;
}

std::vector<double> NozzleGeometry::getAreaDiffDividedByArea()
{
    return areaDiffDividedByArea;
}
#ifndef NOZZLEGEOMETRY_H
#define NOZZLEGEOMETRY_H

#include <vector>
#include <string>
#include "Vector3.hpp"
#include "Mesh.hpp"

class NozzleGeometry : public Mesh
{
    public:
        NozzleGeometry();
        NozzleGeometry(std::string fileName);

        void loadGeometry(std::string fileName);
        double areaFunction(double x);
        void createGeometry();
        void calcAreaDiff();
        void calcAreaDiffDividedByArea();

        std::vector<double> getArea();
        std::vector<double> getAreaDiff();
        std::vector<double> getAreaDiffDividedByArea();
        

    private:
        std::vector<double> area;
        std::vector<double> areaDiff;
        std::vector<double> areaDiffDividedByArea;

};

#endif // NOZZLEGEOMETRY_H
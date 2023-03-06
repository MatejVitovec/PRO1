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
        void calcAreaFaces();

        double areaFunction(double x);
        void createGeometry();

        void calcAreaDiff();

        std::vector<double> getArea();
        std::vector<double> getAreaFaces();
        std::vector<double> getAreaDiff();
        

    private:
        std::vector<double> area;
        std::vector<double> areaFaces;
        std::vector<double> areaDiff;

};

#endif // NOZZLEGEOMETRY_H
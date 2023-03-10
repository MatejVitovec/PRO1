#ifndef NOZZLE_H
#define NOZZLE_H

#include <vector>
#include <string>
#include "Vector3.hpp"
#include "Mesh.hpp"
#include "BoundaryCondition.hpp"

class Nozzle : public Mesh
{
    public:
        Nozzle();
        Nozzle(int n);
        Nozzle(std::string fileName);

        void setInlet(std::shared_ptr<BoundaryCondition> in);
        void setOutlet(std::shared_ptr<BoundaryCondition> out);

        std::shared_ptr<BoundaryCondition> getInlet();
        std::shared_ptr<BoundaryCondition> getOutlet();

        void loadGeometry(std::string fileName);
        void calcAreaFaces();

        double areaFunction(double x);
        void createGeometry(int cellsIn);

        void calcAreaDiff();

        std::vector<double> getArea();
        std::vector<double> getAreaFaces();
        std::vector<double> getAreaDiff();
        

    private:
        std::vector<double> area;
        std::vector<double> areaFaces;
        std::vector<double> areaDiff;

        std::shared_ptr<BoundaryCondition> inlet;
        std::shared_ptr<BoundaryCondition> outlet;

};

#endif // NOZZLE_H
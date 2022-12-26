#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include "Vector3.hpp"
#include "EulerEquations.hpp"
#include "SpatialScheme.hpp"
#include "TemporalScheme.hpp"


class Mesh
{
    public:
        Mesh();
        Mesh(double dom, double nCells, double startPos);
        
        double getDx();
        double getCells();
        double getDomain();
        double getFirstX();

    protected:
        double dx;
        double cells;
        double domain;
        double firstX;

};

#endif // MESH_H
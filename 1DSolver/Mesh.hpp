#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include "Vector3.hpp"

class Mesh
{
    public:
        Mesh();
        Mesh(double dom, int nCells, double startPos);
        
        double getDx();
        int getCells();
        double getDomain();
        double getFirstX();

    protected:
        double dx;
        int cells;
        double domain;
        double firstFaceX;

};

#endif // MESH_H
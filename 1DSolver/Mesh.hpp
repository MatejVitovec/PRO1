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
        Mesh(double domain, double n);
        double getDx();
        double getCells();
        double detDomain();


    private:
        double dx;
        double n;
        double domain;

};

#endif // MESH_H
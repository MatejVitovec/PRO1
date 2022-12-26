#include <cmath>
#include "Mesh.hpp"


Mesh::Mesh()
{
    
}

Mesh::Mesh(double dom, double cells)
{
    domain = dom;
    n = cells;
    dx = dom/cells;
}

double Mesh::getDx()
{
    return dx;
}

double Mesh::getCells()
{
    return n;
}

double Mesh::detDomain()
{
    return domain;
}

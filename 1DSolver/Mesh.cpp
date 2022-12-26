#include <cmath>
#include "Mesh.hpp"


Mesh::Mesh()
{
    
}

Mesh::Mesh(double dom, double nCells, double startPos)
{
    domain = dom;
    cells = nCells;
    dx = dom/cells;
    firstX = startPos;
}

double Mesh::getDx()
{
    return dx;
}

double Mesh::getCells()
{
    return cells;
}

double Mesh::getDomain()
{
    return domain;
}

double Mesh::getFirstX()
{
    return firstX;
}

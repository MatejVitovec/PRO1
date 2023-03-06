#include <cmath>
#include "Mesh.hpp"


Mesh::Mesh()
{
    
}

Mesh::Mesh(double dom, int nCells, double startPos)
{
    domain = dom;
    cells = nCells;
    dx = dom/cells;
    firstFaceX = startPos;
}

double Mesh::getDx()
{
    return dx;
}

int Mesh::getCells()
{
    return cells;
}

double Mesh::getDomain()
{
    return domain;
}

double Mesh::getFirstX()
{
    return firstFaceX;
}

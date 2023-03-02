#ifndef SPATIALSCHEME_H
#define SPATIALSCHEME_H

#include <memory>
#include <vector>
#include "Vector3.hpp"

class SpatialScheme
{
    public:
        virtual std::vector<Vector3> calculateResidues(const std::vector<Vector3>& w, double dx) const = 0;

    protected:
    
};

#endif // SPATIALSCHEME_H
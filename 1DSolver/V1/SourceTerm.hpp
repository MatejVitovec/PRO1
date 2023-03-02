#ifndef SOURCETERM_H
#define SOURCETERM_H

#include <vector>
#include "Vector3.hpp"

class SourceTerm
{
    public:
        SourceTerm();
        virtual std::vector<Vector3> calculateSourceResidues(const std::vector<Vector3>& w);/*const = 0;*/

    protected:
    
};

#endif // SOURCETERM_H
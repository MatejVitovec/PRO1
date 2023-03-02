#ifndef MINMOD_H
#define MINMOD_H

#include "Vector3.hpp"

class Minmod : public SlopeLimiter
{
    public:
        virtual Vector3 calc(const Vector3& r) const;

    protected:        
    
};

#endif // MINMOD_H
#ifndef SLOPELIMITER_H
#define SLOPELIMITER_H

#include "Vector3.hpp"

class SlopeLimiter
{
    public:
        virtual Vector3 calc(const Vector3& r) const = 0;

    protected:
        
    
};

#endif // SLOPELIMITER_H
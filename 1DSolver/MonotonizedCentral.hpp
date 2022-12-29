#ifndef MONOTONIZEDCENTRAL_H
#define MONOTONIZEDCENTRAL_H

#include "Vector3.hpp"

class MonotonizedCentral : public SlopeLimiter
{
    public:
        virtual Vector3 calc(const Vector3& r) const;

    protected:        
    
};

#endif // MONOTONIZEDCENTRAL_H
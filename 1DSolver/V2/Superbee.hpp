#ifndef SUPERBEE_H
#define SUPERBEE_H

#include "Vector3.hpp"

class Superbee : public SlopeLimiter
{
    public:
        virtual Vector3 calc(const Vector3& r) const;

    protected:        
    
};

#endif // SUPERBEE_H
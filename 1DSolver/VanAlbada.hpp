#ifndef VANALBADA_H
#define VANALBADA_H

#include "Vector3.hpp"

class VanAlbada : public SlopeLimiter
{
    public:
        virtual Vector3 calc(const Vector3& r) const;

    protected:        
    
};

#endif // VANALBADA_H
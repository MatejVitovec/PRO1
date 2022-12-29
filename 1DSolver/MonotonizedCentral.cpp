#include <cmath>
#include "SlopeLimiter.hpp"
#include "MonotonizedCentral.hpp"

Vector3 MonotonizedCentral::calc(const Vector3& r) const
{
    Vector3 out;
    if(std::isinf(r[0]))
    {
        out[0] = 2.0;
    }
    else
    {
        out[0] = std::max(0.0, std::min(2.0*r[0], std::min(0.5*(1.0 + r[0]), 2.0)));
    }

    if(std::isinf(r[1]))
    {
        out[1] = 2.0;
    }
    else
    {
        out[1] = std::max(0.0, std::min(2.0*r[1], std::min(0.5*(1.0 + r[1]), 2.0)));
    }

    if(std::isinf(r[2]))
    {
        out[2] = 2.0;
    }
    else
    {
        out[1] = std::max(0.0, std::min(2.0*r[2], std::min(0.5*(1.0 + r[2]), 2.0)));
    }

    return out;
}
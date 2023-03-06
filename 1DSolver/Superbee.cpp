#include <cmath>
#include "SlopeLimiter.hpp"
#include "Superbee.hpp"

Vector3 Superbee::calc(const Vector3& r) const
{
    Vector3 out;
    if(std::isinf(r[0]))
    {
        out[0] = 2.0;
    }
    else
    {
        out[0] = std::max(0.0, std::min(std::min(2.0*r[0], 1.0), std::min(r[0], 2.0)));
    }

    if(std::isinf(r[1]))
    {
        out[1] = 2.0;
    }
    else
    {
        out[1] = std::max(0.0, std::min(std::min(2.0*r[1], 1.0), std::min(r[1], 2.0)));
    }

    if(std::isinf(r[2]))
    {
        out[2] = 2.0;
    }
    else
    {
        out[2] = std::max(0.0, std::min(std::min(2.0*r[2], 1.0), std::min(r[2], 2.0)));
    }

    return out;
}
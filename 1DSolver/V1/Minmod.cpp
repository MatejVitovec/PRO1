#include <cmath>
#include "SlopeLimiter.hpp"
#include "Minmod.hpp"

Vector3 Minmod::calc(const Vector3& r) const
{
    Vector3 out;
    if(std::isinf(r[0]))
    {
        out[0] = 1.0;
    }
    else
    {
        out[0] = std::max(0.0, std::min(1.0, r[0]));
    }

    if(std::isinf(r[1]))
    {
        out[1] = 1.0;
    }
    else
    {
        out[1] = std::max(0.0, std::min(1.0, r[1]));
    }

    if(std::isinf(r[2]))
    {
        out[2] = 1.0;
    }
    else
    {
        out[1] = std::max(0.0, std::min(1.0, r[2]));
    }

    return out;
}
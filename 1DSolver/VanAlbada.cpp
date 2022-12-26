#include "SlopeLimiter.hpp"
#include "VanAlbada.hpp"

Vector3 VanAlbada::calc(const Vector3& r) const
{
    Vector3 out;
    out[0] = (r[0]*r[0] + r[0])/(r[0]*r[0] + 1.0);
    out[1] = (r[1]*r[1] + r[1])/(r[1]*r[1] + 1.0);
    out[2] = (r[2]*r[2] + r[2])/(r[2]*r[2] + 1.0);

    return out;
}
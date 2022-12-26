#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <array>

class Vector3
{
    public:
        Vector3()
        {

        };
        
        Vector3(const std::array<double, 3>& in)
        {
            data = in;
        };

        double& operator[] (int i)
        {
            return data[i];
        }

        const double& operator[](int i) const
        {
            return data[i];
        }

    private:
        std::array<double, 3> data;
};


// u + v
inline Vector3 operator+ (const Vector3& u, const Vector3& v)
{
    Vector3 out;
    out[0] = u[0] + v[0];
    out[1] = u[1] + v[1];
    out[2] = u[2] + v[2];
    return out;
}

// u - v
inline Vector3 operator- (const Vector3& u, const Vector3& v)
{
    Vector3 out;
    out[0] = u[0] - v[0];
    out[1] = u[1] - v[1];
    out[2] = u[2] - v[2];
    return out;
}

// a * u
inline Vector3 operator* (double a, const Vector3& u)
{
    Vector3 out;
    out[0] = a * u[0];
    out[1] = a * u[1];
    out[2] = a * u[2];
    return out;
}

// w * u
inline Vector3 operator* (const Vector3& w, const Vector3& u)
{
    Vector3 out;
    out[0] = w[0] * u[0];
    out[1] = w[1] * u[1];
    out[2] = w[2] * u[2];
    return out;
}

// u / a
inline Vector3 operator/ (const Vector3& u, double a)
{
    Vector3 out;
    out[0] = u[0] / a;
    out[1] = u[1] / a;
    out[2] = u[2] / a;
    return out;
}

#endif
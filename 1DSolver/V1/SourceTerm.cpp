#include "SourceTerm.hpp"

SourceTerm::SourceTerm()
{

}

std::vector<Vector3> SourceTerm::calculateSourceResidues(const std::vector<Vector3>& w)
{
    std::vector<Vector3> out;

    for (int i = 0; i < w.size(); i++)
    {
        out.push_back(Vector3({0.0, 0.0, 0.0}));
    }

    return out;
}
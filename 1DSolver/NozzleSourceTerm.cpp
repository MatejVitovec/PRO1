#include "NozzleSourceTerm.hpp"

NozzleSourceTerm::NozzleSourceTerm()
{

}

NozzleSourceTerm::NozzleSourceTerm(std::shared_ptr<EulerEquations> euler, std::shared_ptr<NozzleGeometry> geom)
{
    setEquationModel(euler);
    setGeometry(geom);
}

void NozzleSourceTerm::setEquationModel(std::shared_ptr<EulerEquations> euler)
{
    eulerEqn = euler;
}

void NozzleSourceTerm::setGeometry(std::shared_ptr<NozzleGeometry> geom)
{
    geometry = geom;
}

std::vector<Vector3> NozzleSourceTerm::calculateSourceResidues(const std::vector<Vector3>& w)
{
    std::vector<Vector3> out;
    std::vector<double> areaDiffDividedByArea = geometry->getAreaDiffDividedByArea();

    for (int i = 0; i < w.size(); i++)
    {
        out.push_back((areaDiffDividedByArea[i])*(eulerEqn->flux(w[i]) - Vector3({0, eulerEqn->pressure(w[i]), 0})));
    }

    return out;
}
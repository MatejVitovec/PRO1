#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "Vector3.hpp"

enum {RHO, RHO_U, RHO_E};

double GAMMA;

inline double density(const Vector3& w)
{
    return w[RHO];
}


inline double velocity(const Vector3& w)
{
    return w[RHO_U] / w[RHO];
}


inline double totalEnergy(const Vector3& w)
{
    return w[RHO_E] / w[RHO];
}


inline double pressure(const Vector3& w)
{
    return (GAMMA - 1.0)*density(w)*(totalEnergy(w) - 0.5*velocity(w)*velocity(w));
}


inline double internalEnergy(const Vector3& w)
{
    return pressure(w)/((GAMMA - 1.0)*density(w));
}


inline double soundSpeed(const Vector3& w)
{
    return sqrt((GAMMA*pressure(w))/density(w));
}


inline Vector3 flux(const Vector3& w)
{
    double u = velocity(w);
    double p = pressure(w);

    return u * w + Vector3({0.0, p, p*u});
}


Vector3 primitiveToConservative(Vector3 v)
{
    return Vector3({v[0], v[0]*v[1], 0.5*v[0]*v[1]*v[1] + (v[2])/(GAMMA - 1)});
}


Vector3 riemannInitialCondition(double x, Vector3 wl, Vector3 wr, double initDiscontinuityPos)
{
    if (x < initDiscontinuityPos)
    {
        return wl;
    }
    else
    {
        return wr;
    }
}


double maxTimeStep(const std::vector<Vector3>& w, double dx)
{
    double dt = 10e+100;

    for (int i = 0; i < w.size(); i++)
    {
        dt = std::min(dt, dx/(fabs(velocity(w[i])) + soundSpeed(w[i])));
    }

    return dt;
}


Vector3 waveSpeeds(const Vector3& wl, const Vector3& wr)
{
    double ul = velocity(wl);
    double ur = velocity(wr);
    double al = soundSpeed(wl);
    double ar = soundSpeed(wr);

    double sl = std::min(ul - al, ur - ar);
    double sr = std::max(ul + al, ur + ar);

    double ss = (pressure(wr) - pressure(wl) + wl[RHO_U]*(sl - ul) - wr[RHO_U]*(sr - ur))/(wl[RHO]*sl - wl[RHO_U] - wr[RHO]*sr + wr[RHO_U]);

    return Vector3({sl, ss, sr});
}


Vector3 waveSpeedsEstimate(const Vector3& wl, const Vector3& wr)
{
    //PVRS
    double pl = pressure(wl);
    double pr = pressure(wr);
    double ul = velocity(wl);
    double ur = velocity(wr);
    double rhol = density(wl);
    double rhor = density(wr);
    double al = soundSpeed(wl);
    double ar = soundSpeed(wr);

    double pStar = std::fmax(0, 0.5*((pl + pr) - 0.25*(ul + ur)*(rhol + rhor)*(al + ar)));
    double ql;
    double qr;

    if(pStar > pl)
    {
        ql = pow(1 + ((GAMMA + 1)/(2*GAMMA))*((pStar/pl) - 1), 0.5);
    }
    else
    {
        ql = 1;
    }
    double sl = ul - al*ql;

    if(pStar > pr)
    {
        qr = pow(1 + ((GAMMA + 1)/(2*GAMMA))*((pStar/pr) - 1), 0.5);
    }
    else
    {
        qr = 1;
    }
    double sr = ur + ar*qr;

    double ss = (pr - pl + rhol*ul*(sl - ul) - rhor*ur*(sr - ur))/(rhol*sl - rhol*ul - rhor*sr + rhor*ur);

    return Vector3({sl, ss, sr});
}


Vector3 HLL(const Vector3& wl, const Vector3& wr)
{
    enum {sl, ss, sr};
    Vector3 wSpeed = waveSpeeds(wl, wr);

    if (0 <= sl)
    {
        return flux(wl);
    }
    else if (0 < sr)
    {
        return (wSpeed[sr]*flux(wl) - wSpeed[sl]*flux(wr) + wSpeed[sl]*wSpeed[sr]*(wr - wl)) / (wSpeed[sr] - wSpeed[sl]);
    }
    else
    {
        return flux(wr);
    }
}


Vector3 HLLC(const Vector3& wl, const Vector3& wr)
{
    enum {sl, ss, sr};
    Vector3 wSpeed = waveSpeedsEstimate(wl, wr);
    

    if (0 <= wSpeed[sl])
    {
        //FL
        return flux(wl);
    }
    else if(0 <= wSpeed[ss])
    {
        //F*L
        return (wSpeed[ss]*(wSpeed[sl]*wl - flux(wl)) + wSpeed[sl]*(pressure(wl) + density(wl)*(wSpeed[sl] - velocity(wl))*(wSpeed[ss] - velocity(wl)))*Vector3({0, 1, wSpeed[ss]}))/(wSpeed[sl] - wSpeed[ss]);
    }
    else if(0 <= wSpeed[sr])
    {
        //F*R
        return (wSpeed[ss]*(wSpeed[sr]*wr - flux(wr)) + wSpeed[sr]*(pressure(wr) + density(wr)*(wSpeed[sr] - velocity(wr))*(wSpeed[ss] - velocity(wr)))*Vector3({0, 1, wSpeed[ss]}))/(wSpeed[sr] - wSpeed[ss]);
    }
    else
    {
        //FR
        return flux(wr);
    }
}


void loadData(std::string fileName, double& domlen, double& diaph, double& cells, double& lGamma, double& time, Vector3& stateL, Vector3& stateR)
{
    std::ifstream file(fileName);
    std::string line;

    std::getline(file, line);
    domlen = std::stod(line);
    std::getline(file, line);
    diaph = std::stod(line);
    std::getline(file, line);
    cells = std::stod(line);
    std::getline(file, line);
    lGamma = std::stod(line);
    std::getline(file, line);
    time = std::stod(line);

    std::getline(file, line);
    stateL[0] = std::stod(line);
    std::getline(file, line);
    stateL[1] = std::stod(line);
    std::getline(file, line);
    stateL[2] = std::stod(line);

    std::getline(file, line);
    stateR[0] = std::stod(line);
    std::getline(file, line);
    stateR[1] = std::stod(line);
    std::getline(file, line);
    stateR[2] = std::stod(line);

    file.close();
}

void saveData(std::string fileName, std::vector<Vector3> w, double domLen, double cells, double time)
{
    std::ofstream writeToFile(fileName);
    writeToFile << domLen << std::endl;
    writeToFile << cells << std::endl;
    writeToFile << time << std::endl;

    for (int i = 0; i < w.size(); i++)
    {
        writeToFile << density(w[i]) << ","<< velocity(w[i]) << "," << pressure(w[i]) << "," << internalEnergy(w[i]) << std::endl;
    }
    
    writeToFile.close();
}


int main(int argc, char** argv)
{
    std::string inputFileName = "cases/case1.txt";
    std::string outputFileName = "results/HLLC.txt";

    if(argc == 3)
    {
        inputFileName = argv[1];
        outputFileName = argv[2];
    }

    double domainLenght;
    double initDiscontinuityPos;
    double n;
    double setTime;
    Vector3 stateL;
    Vector3 stateR;

    loadData(inputFileName, domainLenght, initDiscontinuityPos, n, GAMMA, setTime, stateL, stateR);


    double dx = domainLenght/n;
    double time = 0;

    Vector3 wl = primitiveToConservative(stateL);
    Vector3 wr = primitiveToConservative(stateR);

    std::vector<Vector3> w(n);
    std::vector<Vector3> wn(n);
    std::vector<Vector3> f(n + 1);

    for (int i = 0; i < w.size(); i++)
    {
        w[i] = riemannInitialCondition((i + 0.5)*dx, wl, wr, initDiscontinuityPos);
    }

    bool exitCalcualtion = false;
    int iter = 0;


    while (1)
    {        
        f[0] = HLLC(w[0], w[0]);
        f[n] = HLLC(w[n-1], w[n-1]);

        for (int i = 1; i < n; i++)
        {
            f[i] = HLLC(w[i-1], w[i]);
        }

        //CFL = 0.9
        double dt = 0.9 * maxTimeStep(w, dx);        
        if(setTime - time < dt)
        {
            dt = setTime - time;
            exitCalcualtion = true;
        }
        time += dt;

        for (int i = 0; i < n; i++)
        {
            wn[i] = w[i] - dt/dx * (f[i+1] - f[i]);
        }

        w = wn;

        iter++;

        if(exitCalcualtion)
        {
            break;
        }
    }

    saveData(outputFileName, w, domainLenght, n, setTime);

    std::cout << "Výpočet v čase t = " << time <<" proběhl úspěšně s " << iter << " iteracemi." << std::endl;

    return 0;
}
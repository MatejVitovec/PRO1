#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

struct State
{
    double d;
    double u;
    double p;
};

struct PressureFunctionMembers
{
    double f;
    double fd;
};

double GAMMA;

double soundSpeed(State state)
{
    return sqrt((GAMMA*state.p)/state.d);
}

PressureFunctionMembers pressureFunction(State stateK, double p)
{
    PressureFunctionMembers out;

    if(p <= stateK.p)
    {
        //Refraction wave
        double prat = p/stateK.p;
        double ck = soundSpeed(stateK);
        out.f = (2.0/(GAMMA - 1.0))*ck*(pow(prat, (GAMMA - 1.0)/(2*GAMMA)) - 1.0);
        out.fd = (1.0/(stateK.d*ck))*pow(prat, -((GAMMA + 1.0)/(2.0*GAMMA)));
    }
    else
    {
        // Shock wave
        double ak = (2.0/(GAMMA + 1.0))/stateK.d;
        double bk = ((GAMMA - 1.0)/(GAMMA+1.0))*stateK.p;
        double qrt = sqrt(ak/(bk + p));
        out.f = (p - stateK.p)*qrt;
        out.fd = (1 - (0.5*(p - stateK.p))/(bk + p))*qrt;
    }

    return out;
}

State starPressureVelocity(State stateL, State stateR)
{
    State out;
    int nIter = 0;
    double pTolerance;
    double startPressure;

    pTolerance = 1.0e-6;
    startPressure = 0.2;

    double p;
    double pOld = startPressure;
    double u;
    double uDiff = stateR.u - stateL.u;

    double change;

    PressureFunctionMembers prefunL;
    PressureFunctionMembers prefunR;

    while(1)
    {
        nIter++;
        prefunL = pressureFunction(stateL, pOld);
        prefunR = pressureFunction(stateR, pOld);
        p = pOld - (prefunL.f + prefunR.f + uDiff)/(prefunL.fd + prefunR.fd);
        change = 2.0*fabs((p - pOld)/(p + pOld));

        if (change <= pTolerance)
        {
            break;
        }
        if (p <= 0)
        {
            p = pTolerance;
        }

        pOld = p;
    }

    u = 0.5*(stateL.u + stateR.u + prefunR.f - prefunL.f);

    out.p = p;
    out.u = u;

    return out;
}



int main()
{
    double domainLenght;
    double initialDiscontinuityPosition;
    double time;

    double pressureStar;
    double velocityStar;

    State stateL;
    State stateR;

    GAMMA = 1.4;
    domainLenght = 1.0;
    initialDiscontinuityPosition = 0.5;
    time = 0.25;

    stateL.d = 1.0;
    stateL.u = 0.0;
    stateL.p = 1.0;    
    stateR.d = 0.125;
    stateR.u = 0.0;
    stateR.p = 0.1;
    
    State starRegionPU = starPressureVelocity(stateL, stateR);



    return 0;
}


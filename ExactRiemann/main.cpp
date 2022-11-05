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


double startPressureGuess(State stateL, State stateR)
{
    return 0.5*(stateL.p + stateR.p);
}


PressureFunctionMembers pressureFunction(State stateK, double p)
{
    PressureFunctionMembers out;

    if(p <= stateK.p)
    {
        //Rarefaction wave
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
    PressureFunctionMembers prefunL;
    PressureFunctionMembers prefunR;
    State out;    
    double p;
    double u;
    double change;

    double pTolerance = 1.0e-6;

    double startPressure = startPressureGuess(stateL, stateR);    
    double pOld = startPressure;    
    double uDiff = stateR.u - stateL.u;

    int nIter = 0;

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

State solution(double S, State stateStar, State stateL, State stateR)
{
    State out;

    if(S <= stateStar.u)
    {
        // Left of the constact discontinuity

        double cL = soundSpeed(stateL);

        if(stateStar.p > stateL.p)
        {
            // Left shock
            double SL = stateL.u - cL*sqrt(((GAMMA + 1.0)/(2.0*GAMMA))*(stateStar.p/stateL.p) + (GAMMA - 1.0)/(2.0*GAMMA));

            if(S <= SL)
            {
                // W=WL
                out = stateL;
            }
            else
            {
                // W=W*L
                out.d = stateL.d*((stateStar.p/stateL.p + (GAMMA - 1.0)/(GAMMA + 1.0))/((stateStar.p/stateL.p)*((GAMMA - 1.0)/(GAMMA + 1.0)) + 1.0));
                out.u = stateStar.u;
                out.p = stateStar.p;
            }
        }
        else
        {
            // Left rarefaction
            double SHL = stateL.u - cL;

            if(S <= SHL)
            {
                //W = WL
                out = stateL;
            }
            else
            {
                double STL = stateStar.u - cL*pow(stateStar.p/stateL.p, (GAMMA - 1.0)/(2.0*GAMMA));

                if(S > STL)
                {
                    // W = W*L
                    out.d = stateL.d*pow(stateStar.p/stateL.p, 1.0/GAMMA);
                    out.u = stateStar.u;
                    out.p = stateStar.p;
                }
                else
                {
                    // W = WLfan
                    double C = (2.0/(GAMMA + 1.0))*(cL + ((GAMMA - 1.0)/2.0)*(stateL.u - S));
                    out.d = stateL.d*pow(C/cL, 2.0/(GAMMA - 1.0));
                    out.u = (2.0/(GAMMA + 1.0))*(cL + ((GAMMA - 1.0)/2.0)*stateL.u + S);
                    out.p = stateL.p*pow(C/cL, (2.0*GAMMA)/(GAMMA - 1.0));
                }
            }
        }
    }
    else
    {
        // Right of the contact discontinuity

        double cR = soundSpeed(stateR);

        if(stateStar.p > stateR.p)
        {
            // Right shock
            double SR = stateR.u - cR*sqrt(((GAMMA + 1.0)/(2.0*GAMMA))*(stateStar.p/stateR.p) + (GAMMA - 1.0)/(2.0*GAMMA));

            if(S > SR)
            {
                // W=WR
                out = stateR;
            }
            else
            {
                // W=W*R
                out.d = stateR.d*((stateStar.p/stateR.p + (GAMMA - 1.0)/(GAMMA + 1.0))/((stateStar.p/stateR.p)*((GAMMA - 1.0)/(GAMMA + 1.0)) + 1.0));
                out.u = stateStar.u;
                out.p = stateStar.p;
            }
        }
        else
        {
            // Right rarefaction
            double SHR = stateR.u + cR;;

            if(S >= SHR)
            {
                // W = WR
                out = stateR;
            }
            else
            {
                double STR = stateStar.u + cR*pow(stateStar.p/stateR.p, (GAMMA - 1.0)/(2.0*GAMMA));

                if(S <= STR)
                {
                    // W = W*R
                    out.d = stateR.d*pow(stateStar.p/stateR.p, 1.0/GAMMA);
                    out.u = stateStar.u;
                    out.p = stateStar.p;
                }
                else
                {
                    //W = WRfan
                    double C = (2.0/(GAMMA + 1.0))*(cR - ((GAMMA - 1.0)/2.0)*(stateR.u - S));
                    out.d = stateR.d*pow(C/cR, 2.0/(GAMMA - 1.0));
                    out.u = (2.0/(GAMMA + 1.0))*(-cR + ((GAMMA - 1.0)/2.0)*stateR.u + S);
                    out.p = stateR.p*pow(C/cR, (2.0*GAMMA)/(GAMMA - 1.0));
                }
            }
        }
    }

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
    stateL.p = 1000.0;    
    stateR.d = 1.0;
    stateR.u = 0.0;
    stateR.p = 0.01;
    
    State starRegionPU = starPressureVelocity(stateL, stateR);



    return 0;
}


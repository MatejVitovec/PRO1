#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

struct State
{
    double density;
    double velocity;
    double pressure;
};

struct pressureFunctionMembers
{
    double f;
    double fd;
};

double gamma;

void starPressureVelocity(State stateL, State stateR)
{
    int nIter;
    double pressureTolerance;
    double startPressure;

    startPressure = 0.1;



}

pressureFunctionMembers pressureFunction()
{
    pressureFunctionMembers out;


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

    gamma = 1.4;

    domainLenght = 1;
    initialDiscontinuityPosition = 0.5;
    time = 0.25;

    stateL.density = 0;    
    stateL.pressure = 0;
    stateL.velocity = 0;
    stateR.density = 0;
    stateR.pressure = 0;
    stateR.velocity = 0;

    //sqrt((gamma*initStates.pressureL)/initStates.densityL);



    return 0;
}


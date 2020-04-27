#pragma once

#include "kernel/TonatiuhKernel.h"

// Declaration of some constants 
#define rad   (pi/180)
#define dEarthMeanRadius     6371.01    // In km
#define dAstronomicalUnit    149597890    // In km

struct TONATIUH_KERNEL cTime
{
    int iYear;
    int iMonth;
    int iDay;
    double dHours;
    double dMinutes;
    double dSeconds;
};

struct TONATIUH_KERNEL cLocation
{
    double dLongitude;
    double dLatitude;
};

struct TONATIUH_KERNEL cSunCoordinates
{
    double dEclipticLongitude;
    double dRightAscension;
    double dDeclination;
    double dHourAngle;
    double dZenithAngle;
    double dAzimuth;
};


TONATIUH_KERNEL void sunpos(cTime udtTime, cLocation udtLocation, cSunCoordinates *udtSunCoordinates);

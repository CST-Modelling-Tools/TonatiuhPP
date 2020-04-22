#pragma once

// Declaration of some constants 
#define rad   (pi/180)
#define dEarthMeanRadius     6371.01    // In km
#define dAstronomicalUnit    149597890    // In km

struct cTime
{
    int iYear;
    int iMonth;
    int iDay;
    double dHours;
    double dMinutes;
    double dSeconds;
};

struct cLocation
{
    double dLongitude;
    double dLatitude;
};

struct cSunCoordinates
{
    double dEclipticLongitude;
    double dRightAscension;
    double dDeclination;
    double dHourAngle;
    double dZenithAngle;
    double dAzimuth;
};


void sunpos(cTime udtTime, cLocation udtLocation, cSunCoordinates *udtSunCoordinates);

#include <cmath>

#include "sunpos.h"
#include "libraries/geometry/gc.h"

using namespace std;

void sunpos(cTime udtTime,cLocation udtLocation, cSunCoordinates *udtSunCoordinates)
{
    // Main variables
    double dJulianDate;
    double dElapsedJulianDays;
    double dDecimalHours;
    double dEclipticObliquity;

    // Auxiliary variables
    double dY;
    double dX;

    // Calculate difference in days between the current Julian Day
    // and JD 2451545.0, which is noon 1 January 2000 Universal Time
    {
        long int liAux1;
        long int liAux2;
        // Calculate time of the day in UT decimal hours
        dDecimalHours = udtTime.dHours + (udtTime.dMinutes
            + udtTime.dSeconds / 60.0 ) / 60.0;
        // Calculate current Julian Day
        liAux1 =(udtTime.iMonth-14)/12;
        liAux2=(1461*(udtTime.iYear + 4800 + liAux1))/4 + (367*(udtTime.iMonth
            - 2-12*liAux1))/12- (3*((udtTime.iYear + 4900
        + liAux1)/100))/4+udtTime.iDay-32075;
        dJulianDate=(double)(liAux2)-0.5+dDecimalHours/24.0;
        // Calculate difference between current Julian Day and JD 2451545.0
        dElapsedJulianDays = dJulianDate-2451545.0;
    }

    // Calculate ecliptic coordinates (ecliptic longitude and obliquity of the
    // ecliptic in radians but without limiting the angle to be less than 2*Pi
    // (i.e., the result may be greater than 2*Pi)
    {
        double dAux = 0.0172021241615*dElapsedJulianDays;
        udtSunCoordinates->dEclipticLongitude = 4.894969024+0.017202791697*dElapsedJulianDays
                            +5.78262765229295e-009*dElapsedJulianDays*cos(2.67824+dAux)
                            +0.03341656*cos(4.6693+dAux)
                            +0.00034894*cos(4.6261+0.03440424832*dElapsedJulianDays)
                            -0.00008339*sin(2.1824-0.00092421755*dElapsedJulianDays)
                            +0.00003934*cos(2.7441+0.01575190938*dElapsedJulianDays)
                            +0.00003461*cos(2.8289+9.64572211e-6*dElapsedJulianDays)
                            +0.00003136*cos(3.6277+0.21276871047*dElapsedJulianDays)
                            +0.00002676*cos(4.4181+0.02152065544*dElapsedJulianDays)
                            +0.00003046*cos(6.1352+0.01076032772*dElapsedJulianDays);

        dEclipticObliquity = 0.4090928040395968 - 6.21395528282155e-09*dElapsedJulianDays
                            +0.00004459673713551599*cos(2.182954286987892-9.24169576470094e-04*dElapsedJulianDays)
                            -2.782663310837079e-6*cos(87.60120254057658-0.0344055570784386*dElapsedJulianDays)
                            +1.0435812166758367e-7*cos(167.41964504653737-0.0698787501985692*dElapsedJulianDays);
    }

    // Calculate celestial coordinates ( right ascension and declination ) in radians
    // but without limiting the angle to be less than 2*Pi (i.e., the result may be
    // greater than 2*Pi)
    {
        double dSin_EclipticLongitude;
        dSin_EclipticLongitude= sin( udtSunCoordinates->dEclipticLongitude );
        dY = cos( dEclipticObliquity ) * dSin_EclipticLongitude;
        dX = cos( udtSunCoordinates->dEclipticLongitude );
        udtSunCoordinates->dRightAscension = atan2( dY,dX );
        if( udtSunCoordinates->dRightAscension < 0.0 ) udtSunCoordinates->dRightAscension = udtSunCoordinates->dRightAscension + gc::TwoPi;
        udtSunCoordinates->dDeclination = asin( sin( dEclipticObliquity )*dSin_EclipticLongitude );
    }

    // Calculate local coordinates ( azimuth and zenith angle ) in degrees
    {
        double dGreenwichMeanSiderealTime;
        double dLocalMeanSiderealTime;
        double dLatitudeInRadians;
        double dCos_Latitude;
        double dSin_Latitude;
        double dCos_HourAngle;
        double dParallax;

        dGreenwichMeanSiderealTime = -60053.302628+24.065709825*dElapsedJulianDays
            - 0.000292222*sin(0.0009242*dElapsedJulianDays+0.9574);

        dLocalMeanSiderealTime = (dGreenwichMeanSiderealTime*15
            + udtLocation.dLongitude) * gc::Degree;

        udtSunCoordinates->dHourAngle = dLocalMeanSiderealTime - udtSunCoordinates->dRightAscension;
        dLatitudeInRadians = udtLocation.dLatitude * gc::Degree;
        dCos_Latitude = cos( dLatitudeInRadians );
        dSin_Latitude = sin( dLatitudeInRadians );
        dCos_HourAngle= cos( udtSunCoordinates->dHourAngle );

        udtSunCoordinates->dZenithAngle = (acos( dCos_Latitude*dCos_HourAngle
            *cos( udtSunCoordinates->dDeclination ) + sin( udtSunCoordinates->dDeclination )*dSin_Latitude));
        dY = -sin( udtSunCoordinates->dHourAngle );
        dX = tan( udtSunCoordinates->dDeclination )*dCos_Latitude - dSin_Latitude*dCos_HourAngle;
        udtSunCoordinates->dAzimuth = atan2( dY, dX );
        if ( udtSunCoordinates->dAzimuth < 0.0 )
            udtSunCoordinates->dAzimuth = udtSunCoordinates->dAzimuth + gc::TwoPi;
        udtSunCoordinates->dAzimuth = udtSunCoordinates->dAzimuth/gc::Degree;
        // Parallax Correction
        dParallax=(dEarthMeanRadius/dAstronomicalUnit)
            *sin(udtSunCoordinates->dZenithAngle);
        udtSunCoordinates->dZenithAngle=(udtSunCoordinates->dZenithAngle
            + dParallax)/gc::Degree;
    }
}

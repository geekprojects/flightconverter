//
// Created by Ian Parker on 06/05/2025.
//

#include "civa.h"

using namespace std;

string convertToDMS(double degrees, bool isLatitude)
{
    bool isNegative = degrees < 0;
    degrees = fabs(degrees);
    float d = floor(degrees);
    float m = (degrees - d) * 60.0f;

    char dir;
    if (isLatitude)
    {
        if (isNegative)
        {
            dir = 'S';
        }
        else
        {
            dir = 'N';
        }
    }
    else
    {
        if (isNegative)
        {
            dir = 'W';
        }
        else
        {
            dir = 'E';
        }
    }

    char buffer[64];
    snprintf(buffer, 64, "%c%02d\u00B0%04.1f", dir, (int)d, m);

    return string(buffer);
}

bool CivaCommand::execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> flightPlan, map<std::string, std::string> args)
{
    int i = 1;
    printf("Civa: %lu waypoints\n", flightPlan->m_waypoints.size());
    for (WayPoint wp : flightPlan->m_waypoints)
    {
        string latitude = convertToDMS(wp.coordinate.latitude, true);
        string longitude = convertToDMS(wp.coordinate.longitude, false);

        string dmeStr = "";
        if (wp.navAid != nullptr && wp.navAid->getType() == UFC::NavAidType::DME)
        {
            dmeStr = "DME: " + to_string((float)wp.navAid->getFrequency() / 100.0f);
        }
        else if (wp.navAid != nullptr && wp.navAid->getType() == UFC::NavAidType::VOR)
        {
            dmeStr = "VOR: " + to_string((float)wp.navAid->getFrequency() / 100.0f);
        }

        printf("%02d: %s, %s: %s: %s\n", i, latitude.c_str(), longitude.c_str(), wp.id.c_str(), dmeStr.c_str());
        i++;
    }
    printf("\n");

    return true;
}

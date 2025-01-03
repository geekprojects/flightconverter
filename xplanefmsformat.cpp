//
// Created by Ian Parker on 23/10/2024.
//

#include "xplanefmsformat.h"

#include "flightplan.h"

using namespace std;
using namespace UFC;

void XPlaneFormat::save(shared_ptr<FlightConverter> flightConverter, shared_ptr<FlightPlan> flightPlan, string filename)
{
    FILE* fd = fopen(filename.c_str(), "w");
    if (fd == nullptr)
    {
        printf("XPlaneFormat::save() could not open file %s\n", filename.c_str());
        return;
    }

    fprintf(fd, "I\n");
    fprintf(fd, "1100 Version\n");
    fprintf(fd, "CYCLE %s\n", flightPlan->m_cycle.c_str());

    auto waypointCount = flightPlan->m_waypoints.size();
    if (!flightPlan->m_departureAirport.empty())
    {
        fprintf(fd, "ADEP %s\n", flightPlan->m_departureAirport.c_str());
        waypointCount++;
    }
    if (!flightPlan->m_destinationAirport.empty())
    {
        fprintf(fd, "ADES %s\n", flightPlan->m_destinationAirport.c_str());
        waypointCount++;
    }

    fprintf(fd, "NUMENR %lu\n", waypointCount);

    if (!flightPlan->m_departureAirport.empty())
    {
        auto airport = flightConverter->getAirports()->findByCode(flightPlan->m_departureAirport);
        printf("Airport: %s -> %ls\n", flightPlan->m_departureAirport.c_str(), airport->getName().c_str());
        fprintf(fd, "1 %s ADEP %0.6f %0.6f %0.6f\n", flightPlan->m_departureAirport.c_str(),
            airport->getElevation(),
            airport->getLocation().latitude, airport->getLocation().longitude);
    }

    for (auto const& waypoint : flightPlan->m_waypoints)
    {
        int type = -1;
        switch (waypoint.type)
        {
            using enum NavAidType;
            case VOR: type = 3; break;
            case FIX: type = 11; break;
            case WAY_POINT: type = 28; break;
            default:
                printf("Unhandled waypoint type: %d\n", (int)waypoint.type);
                exit(1);
        }

        string via = "DRCT";
        if (!waypoint.via.empty())
        {
            via = waypoint.via;
        }

        fprintf(
            fd,
            "%d %s %s %0.6f %0.6f %0.6f\n",
            type,
            waypoint.id.c_str(),
            via.c_str(),
            (float)flightPlan->m_cruisingAltitude,
            waypoint.coordinate.latitude,
            waypoint.coordinate.longitude);
    }
    if (!flightPlan->m_destinationAirport.empty())
    {
        auto airport = flightConverter->getAirports()->findByCode(flightPlan->m_destinationAirport);
        printf("Airport: %s -> %ls\n", flightPlan->m_destinationAirport.c_str(), airport->getName().c_str());
        fprintf(fd, "1 %s ADES %0.6f %0.6f %0.6f\n", flightPlan->m_destinationAirport.c_str(),
            airport->getElevation(),
            airport->getLocation().latitude, airport->getLocation().longitude);
    }

    fclose(fd);
}

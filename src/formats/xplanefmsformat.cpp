//
// Created by Ian Parker on 23/10/2024.
//

#include "formats/xplanefmsformat.h"

#include "flightconverter.h"
#include "flightplan.h"
#include "utils.h"

using namespace std;
using namespace UFC;

bool XPlaneFormat::check(std::string filename)
{
    auto text = readTextFile(filename, true);
    if (text.size() < 4)
    {
        return false;
    }
    if (text.at(0).at(0) != "I")
    {
        return false;
    }
    if (text.at(1).at(1) != "Version")
    {
        return false;
    }
    return true;
}

shared_ptr<FlightPlan> XPlaneFormat::load(string filename)
{
    auto text = readTextFile(filename, true);

    return nullptr;
}

bool XPlaneFormat::save(shared_ptr<FlightPlan> flightPlan, string filename)
{
    FILE* fd = fopen(filename.c_str(), "w");
    if (fd == nullptr)
    {
        printf("XPlaneFormat::save() could not open file %s\n", filename.c_str());
        return false;
    }

    fprintf(fd, "I\n");
    fprintf(fd, "1100 Version\n");
    fprintf(fd, "CYCLE %d\n", flightPlan->m_cycle);

    auto waypointCount = flightPlan->m_waypoints.size();
    if (!flightPlan->m_departureAirport.empty())
    {
        fprintf(fd, "ADEP %s\n", flightPlan->m_departureAirport.c_str());
        waypointCount++;
    }
    /*
    if (!flightPlan->m_departureRunway.empty())
    {
        fprintf(fd, "DEPRWY %s\n", flightPlan->m_departureRunway.c_str());
    }
    */
    if (!flightPlan->m_destinationAirport.empty())
    {
        fprintf(fd, "ADES %s\n", flightPlan->m_destinationAirport.c_str());
        waypointCount++;
    }

    /*
    if (!flightPlan->m_destinationRunway.empty())
    {
        fprintf(fd, "DESRWY %s\n", flightPlan->m_destinationRunway.c_str());
    }
    */

    fprintf(fd, "NUMENR %lu\n", waypointCount);

    if (!flightPlan->m_departureAirport.empty())
    {
        auto airport = m_flightConverter->getAirports()->findByCode(flightPlan->m_departureAirport);
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
            case DME:
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
        auto airport = m_flightConverter->getAirports()->findByCode(flightPlan->m_destinationAirport);
        printf("Airport: %s -> %ls\n", flightPlan->m_destinationAirport.c_str(), airport->getName().c_str());
        fprintf(fd, "1 %s ADES %0.6f %0.6f %0.6f\n", flightPlan->m_destinationAirport.c_str(),
            airport->getElevation(),
            airport->getLocation().latitude, airport->getLocation().longitude);
    }

    fclose(fd);

    return true;
}

//
// Created by Ian Parker on 15/05/2025.
//

#include "circuit.h"

#include "flightconverter.h"

using namespace std;
using namespace UFC;

float degreesToRadians(float degrees)
{
    return degrees * M_PI / 180.0f;
}

Coordinate getCoordinate(Coordinate coord, float bearing, float distance)
{
    constexpr float earthRadiusKm = 6371;

    bearing = degreesToRadians(bearing);
    float lat1 = degreesToRadians(coord.latitude);
    float lon1 = degreesToRadians(coord.longitude);

    Coordinate result;
    result.latitude = asin( sin(lat1)*cos(distance/earthRadiusKm) +
                      cos(lat1)*sin(distance/earthRadiusKm)*cos(bearing) );
    result.longitude = lon1 + atan2(sin(bearing)*sin(distance/earthRadiusKm)*cos(lat1),
                               cos(distance/earthRadiusKm)-sin(lat1)*sin(result.latitude) );
    result.latitude = result.latitude * 180.0f / M_PI;
    result.longitude = result.longitude * 180.0f / M_PI;
    return result;
}

std::vector<option> CircuitCommand::getOptions() const
{
    vector<option> options;
    options.push_back({"airport", required_argument, 0,  'a' });
    options.push_back({"runway", required_argument, 0,  'r' });
    return options;
}

bool CircuitCommand::execute(FlightPlanContainer* flightPlanContainer, map<std::string, std::string> args)
{
    auto it = args.find("airport");
    if (it == args.end())
    {
        printf("No airport specified\n");
        return false;
    }
    string airportCode = it->second;

    it = args.find("runway");
    if (it == args.end())
    {
        printf("No runway specified\n");
        return false;
    }
    string runwayNumber = it->second;

    auto airport = m_flightConverter->getAirports()->findByCode(airportCode);
    if (airport == nullptr)
    {
        printf("Unable to find airport %s\n", airportCode.c_str());
        return false;
    }

    bool foundRunway = false;
    Runway runway;
    for (auto rw : airport->getRunways())
    {
        printf("Runway: %s: bearing=%0.2f\n", rw.m_number.c_str(), rw.m_bearing);
        if (rw.m_number == runwayNumber)
        {
            foundRunway = true;
            runway = rw;
            break;
        }
    }
    if (!foundRunway)
    {
    printf("Unable to find runway %s\n", runwayNumber.c_str());
        return false;
    }

    printf("Found runway! Bearing=%0.2f\n", runway.m_bearing);

    /*
     * +-----------+
     * |           |
     * +-d1--X-----+
     */

    Coordinate pos1 = getCoordinate(runway.m_startLocation, runway.m_bearing, 10);
    Coordinate pos2 = getCoordinate(pos1, fmod(runway.m_bearing + 90.0, 360.0), 10);
    Coordinate pos3 = getCoordinate(pos2, fmod(runway.m_bearing + 180.0, 360.0), 30);
    Coordinate pos4 = getCoordinate(pos3, fmod(runway.m_bearing + 270.0, 360.0), 10);

    printf("Pos1: %ls\n", pos1.toString().c_str());
    printf("Pos2: %ls\n", pos2.toString().c_str());
    printf("Pos3: %ls\n", pos3.toString().c_str());
    printf("Pos4: %ls\n", pos4.toString().c_str());

    auto navAid1 = m_flightConverter->getNavData()->findNearest(pos1);
    auto navAid2 = m_flightConverter->getNavData()->findNearest(pos2);
    auto navAid3 = m_flightConverter->getNavData()->findNearest(pos3);
    auto navAid4 = m_flightConverter->getNavData()->findNearest(pos4);

    printf("Nav Aid 1: %s\n", navAid1->getId().c_str());
    printf("Nav Aid 2: %s\n", navAid2->getId().c_str());
    printf("Nav Aid 3: %s\n", navAid3->getId().c_str());
    printf("Nav Aid 4: %s\n", navAid4->getId().c_str());
    printf("DCT %s DCT %s DCT %s DCT %s\n",
        navAid1->getId().c_str(), navAid2->getId().c_str(), navAid3->getId().c_str(), navAid4->getId().c_str());

    auto flightPlan = make_shared<FlightPlan>();
    flightPlan->m_cycle = m_flightConverter->getNavData()->getCycle();
    flightPlan->m_departureAirport = airportCode;
    flightPlan->m_destinationAirport = airportCode;
    flightPlan->m_departureRunway = runwayNumber;
    flightPlan->m_destinationRunway = runwayNumber;
    flightPlan->m_waypoints.emplace_back(navAid1);
    flightPlan->m_waypoints.emplace_back(navAid2);
    flightPlan->m_waypoints.emplace_back(navAid3);
    flightPlan->m_waypoints.emplace_back(navAid4);

    flightPlanContainer->m_flightPlans.push_back(flightPlan);

    return true;
}


//
// Created by Ian Parker on 06/05/2025.
//

#include "simplify.h"

#include <ufc/geoutils.h>

using namespace std;
using namespace UFC;

bool SimplifyCommand::execute(FlightPlanContainer* flightPlanContainer, shared_ptr<FlightPlan> flightPlan, map<std::string, std::string> args)
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        auto count = flightPlan->m_waypoints.size();
        if (count < 3)
        {
            return true;
        }

        for (size_t i = 0; i < count - 2; i++)
        {
            WayPoint wp0 = flightPlan->m_waypoints[i];
            WayPoint wp1 = flightPlan->m_waypoints[i + 1];
            WayPoint wp2 = flightPlan->m_waypoints[i + 2];

            auto dist1 = UFC::GeoUtils::distance(wp0.coordinate, wp1.coordinate) / 1.852;
            auto dist2 = UFC::GeoUtils::distance(wp1.coordinate, wp2.coordinate) / 1.852;
            printf("%s -> %s -> %s, Distances: %0.2f, %0.2f\n", wp0.id.c_str(), wp1.id.c_str(), wp2.id.c_str(), dist1, dist2);
            if (dist1 > 200.0 || dist2 > 200.0)
            {
                continue;
            }

            float ang1 = GeoUtils::angleFromCoordinate(wp0.coordinate, wp1.coordinate);
            float ang2 = GeoUtils::angleFromCoordinate(wp0.coordinate, wp2.coordinate);

            float angDiff = fabs(ang1 - ang2);

            printf("%s -> %s -> %s, Angles: %0.2f, %0.2f -> %0.2f\n", wp0.id.c_str(), wp1.id.c_str(), wp2.id.c_str(), ang1, ang2, angDiff);

            if (angDiff < 0.75 &&
                (wp1.type == UFC::NavAidType::WAY_POINT || wp0.type != UFC::NavAidType::WAY_POINT || wp2.type != UFC::NavAidType::WAY_POINT))
            {
                printf(" -> Removing waypoint: %s\n", wp1.id.c_str());
                flightPlan->m_waypoints.erase(flightPlan->m_waypoints.begin() + i + 1);
                changed = true;
                break;
            }
        }
    }
    return true;
}

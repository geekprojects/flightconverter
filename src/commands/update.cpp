//
// Created by Ian Parker on 06/05/2025.
//

#include "update.h"

#include <cfloat>

#include "flightconverter.h"

using namespace std;

bool UpdateCommand::execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> flightPlan, map<std::string, std::string> args)
{
    auto navData = m_flightConverter->getNavData();

    for (auto& wp : flightPlan->m_waypoints)
    {
        printf("updateWaypoints: %s: %ls\n", wp.id.c_str(), wp.coordinate.toString().c_str());
        auto navaid = m_flightConverter->findNavAid(wp.id, wp.coordinate);

        bool validNavAid = (navaid != nullptr);
        if (!validNavAid)
        {
            printf("updateWaypoints: Way point is invalid, searching for suitable replacement...\n");
            auto nearest = navData->findNearest(wp.coordinate);
            bool validReplacement = false;
            if (nearest != nullptr)
            {
                auto distance = UFC::GeoUtils::distance(wp.coordinate, nearest->getLocation());

                // This should probably depend on altitude
                validReplacement = (distance < 2.0f);
            }
            if (validReplacement)
            {
                printf("updateWaypoints: %s: Replacing with %s\n", wp.id.c_str(), nearest->getId().c_str());
                wp.id = nearest->getId();
                wp.coordinate = nearest->getLocation();
                wp.type = nearest->getType();
                wp.navAid = nearest;
            }
            else
            {
                wp.type = UFC::NavAidType::WAY_POINT;
                wp.via = "";
                wp.navAid = nullptr;
            }
        }
    }

    removeDuplicates(flightPlan);

    // We've updated the cycle
    flightPlan->m_cycle = navData->getCycle();

    return true;
}

void UpdateCommand::removeDuplicates(std::shared_ptr<FlightPlan> flightPlan)
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (auto it = flightPlan->m_waypoints.begin(); it != flightPlan->m_waypoints.end() && (it + 1) != flightPlan->m_waypoints.end(); ++it)
        {
            if (it->id == (it + 1)->id)
            {
                flightPlan->m_waypoints.erase(it + 1);
                changed = true;
                break;
            }
        }
    }
}

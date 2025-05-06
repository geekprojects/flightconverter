//
// Created by Ian Parker on 24/11/2024.
//

#include "flightplan.h"

#include <cfloat>

#include "flightconverter.h"

using namespace std;

void FlightPlan::removeDuplicates()
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (auto it = m_waypoints.begin(); it != m_waypoints.end() && (it + 1) != m_waypoints.end(); ++it)
        {
            if (it->id == (it + 1)->id)
            {
                m_waypoints.erase(it + 1);
                changed = true;
                break;
            }
        }
    }
}

void FlightPlan::updateWaypoints(std::shared_ptr<FlightConverter> flightConnector)
{
    auto navData = flightConnector->getNavData();
    for (auto& wp : m_waypoints)
    {
        printf("updateWaypoints: %s: %ls\n", wp.id.c_str(), wp.coordinate.toString().c_str());
        auto navaids = navData->findById(wp.id);

        bool validNavAid = false;
        if (!navaids.empty())
        {
            shared_ptr<UFC::NavAid> nearestNavAid = nullptr;
            double distance = DBL_MAX;

            for (const auto& navaid : navaids)
            {
                printf("updateWaypoints:  -> Found navaid: %s (%ls)\n", navaid->getId().c_str(), navaid->getLocation().toString().c_str());
                double d = UFC::GeoUtils::distance(navaid->getLocation(), wp.coordinate);
                if (d < distance)
                {
                    distance = d;
                    nearestNavAid = navaid;
                }
            }
            if (nearestNavAid && distance < 0.1f)
            {
                validNavAid = true;
            }
        }
        else
        {
            printf("updateWaypoints:  -> No navaids found!\n");
        }

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
            }
            else
            {
                wp.type = UFC::NavAidType::WAY_POINT;
                wp.via = "";
            }
        }
    }

    removeDuplicates();

    // We've updated the cycle
    m_cycle = navData->getCycle();
}

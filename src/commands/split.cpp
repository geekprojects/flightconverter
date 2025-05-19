//
// Created by Ian Parker on 06/05/2025.
//

#include "split.h"

using namespace std;

bool SplitCommand::execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> plan, map<std::string, std::string> args)
{
    printf("Size=%lu\n", plan->m_waypoints.size());
    flightPlanContainer->m_flightPlans.clear();
    for (int i = 0, part = 1; i < plan->m_waypoints.size(); i += 8, part++)
    {
        shared_ptr<FlightPlan> segment = make_shared<FlightPlan>();
        *segment = *plan;
        if (i > 0)
        {
            segment->m_departureAirport = "";
            segment->m_departureRunway = "";
        }

        int end = i + 9;
        if (end >= plan->m_waypoints.size())
        {
            end = plan->m_waypoints.size();
        }
        else
        {
            segment->m_destinationAirport = "";
            segment->m_destinationRunway = "";
        }

        segment->m_waypoints = vector(plan->m_waypoints.begin() + i, plan->m_waypoints.begin() + end);

        flightPlanContainer->m_flightPlans.push_back(segment);
    }
    return true;
}

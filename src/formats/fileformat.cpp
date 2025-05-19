//
// Created by Ian Parker on 10/05/2025.
//

#include "fileformat.h"

#include "flightconverter.h"

void FileFormat::addWayPoint(std::shared_ptr<FlightPlan> flightPlan, WayPoint &wayPoint)
{
    wayPoint.navAid = m_flightConverter->findNavAid(wayPoint.id, wayPoint.coordinate);
    flightPlan->m_waypoints.push_back(wayPoint);
}

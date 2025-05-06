//
// Created by Ian Parker on 23/10/2024.
//

#ifndef FLIGHTPLAN_H
#define FLIGHTPLAN_H

#include <string>
#include <vector>

#include <ufc/geoutils.h>
#include <ufc/navdata.h>

class FlightConverter;

enum class FlightPlanType
{
    VFR,
    IFR
};

struct WayPoint
{
    UFC::NavAidType type = UFC::NavAidType::WAY_POINT;
    UFC::Coordinate coordinate;
    std::string id;
    std::string via;
};

struct FlightPlan
{
    FlightPlanType m_type = FlightPlanType::VFR;
    int m_cycle = 2501;
    int m_cruisingAltitude = 0;
    std::string m_aircraftType;

    std::string m_departureAirport;
    std::string m_departureRunway;
    std::string m_sid;
    std::string m_sidTrans;

    std::string m_destinationAirport;
    std::string m_destinationRunway;
    std::string m_destinationSTAR;
    std::string m_destinationSTARTrans;
    std::string m_destinationApproach;

    std::vector<WayPoint> m_waypoints;

    void removeDuplicates();
    void updateWaypoints(std::shared_ptr<FlightConverter> flightConverter);
};

#endif //FLIGHTPLAN_H

//
// Created by Ian Parker on 23/10/2024.
//

#ifndef FLIGHTCONVERTER_H
#define FLIGHTCONVERTER_H

#include <ufc/flightconnector.h>
#include <ufc/airports.h>

struct FlightPlan;

class FlightConverter
{
 private:
    std::shared_ptr<UFC::FlightConnector> m_flightConnector;
    std::shared_ptr<UFC::Airports> m_airports;
    std::shared_ptr<UFC::NavData> m_navData;

 public:
    bool init();

    std::shared_ptr<UFC::Airports> getAirports() { return m_airports; }
    std::shared_ptr<UFC::NavData> getNavData() { return m_navData; }
};

class FileFormat
{

 public:
    virtual ~FileFormat() = default;
    virtual std::shared_ptr<FlightPlan> load(std::shared_ptr<FlightConverter> flightConverter, std::string filename) = 0;
    virtual void save(std::shared_ptr<FlightConverter> flightConverter, std::shared_ptr<FlightPlan> flightPlan, std::string filename) = 0;
};

#endif //FLIGHTCONVERTER_H

//
// Created by Ian Parker on 23/10/2024.
//

#ifndef FLIGHTCONVERTER_H
#define FLIGHTCONVERTER_H

#include <memory>

#include <ufc/flightconnector.h>
#include <ufc/airports.h>

#include "formats/fileformat.h"

struct FlightPlan;


class FlightConverter : public UFC::Logger
{
    bool m_initialised = false;
    std::vector<FileFormat*> m_formats;

    std::shared_ptr<UFC::FlightConnector> m_flightConnector;
    std::shared_ptr<UFC::Airports> m_airports;
    std::shared_ptr<UFC::NavAids> m_navData;

 public:
    FlightConverter();
    ~FlightConverter() override = default;

    bool init();

    std::shared_ptr<UFC::Airports> getAirports();
    std::shared_ptr<UFC::NavAids> getNavData();

    std::shared_ptr<FlightPlan> load(const std::string &string);

    std::shared_ptr<UFC::FlightConnector> getFlightConnector() { return m_flightConnector; }

    FileFormat* getFormat(FormatType type) const;

    std::shared_ptr<UFC::NavAid> findNavAid(const std::string &id, UFC::Coordinate coordinate);
};


#endif //FLIGHTCONVERTER_H

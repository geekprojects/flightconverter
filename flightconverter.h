//
// Created by Ian Parker on 23/10/2024.
//

#ifndef FLIGHTCONVERTER_H
#define FLIGHTCONVERTER_H

#include <ufc/flightconnector.h>
#include <ufc/airports.h>

class XPlaneFormat;
class MSFSFormat;
struct FlightPlan;

enum class FormatType
{
   XPLANE,
   MSFS
};

class FlightConverter : public std::enable_shared_from_this<FlightConverter>
{
 private:

    std::shared_ptr<MSFSFormat> m_msfsFormat;
    std::shared_ptr<XPlaneFormat> m_xPlaneFormat;

    std::shared_ptr<UFC::FlightConnector> m_flightConnector;
    std::shared_ptr<UFC::Airports> m_airports;
    std::shared_ptr<UFC::NavAids> m_navData;

 public:
    bool init();

    std::shared_ptr<UFC::Airports> getAirports() { return m_airports; }
    std::shared_ptr<UFC::NavAids> getNavData() { return m_navData; }

    std::shared_ptr<FlightPlan> load(const std::string &string);
    void save(FormatType type, std::shared_ptr<FlightPlan> flightPlan, std::string filename);

    std::shared_ptr<UFC::FlightConnector> getFlightConnector();
};

class FileFormat
{
 protected:
    std::shared_ptr<FlightConverter> m_flightConverter;

 public:
    FileFormat(std::shared_ptr<FlightConverter> flightConverter) : m_flightConverter(flightConverter) { }
    virtual ~FileFormat() = default;

    virtual std::shared_ptr<FlightPlan> load(std::string filename) = 0;
    virtual void save(std::shared_ptr<FlightPlan> flightPlan, std::string filename) = 0;
};

#endif //FLIGHTCONVERTER_H

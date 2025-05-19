//
// Created by Ian Parker on 07/05/2025.
//

#ifndef FILEFORMAT_H
#define FILEFORMAT_H

#include <memory>
#include <string>

#include "flightplan.h"

class FlightConverter;
struct FlightPlan;

enum class FormatType
{
    XPLANE,
    MSFS_INI,
    MSFS_XML
 };

class FileFormat
{
protected:
    FlightConverter* m_flightConverter;

public:
    FileFormat(FlightConverter* flightConverter) : m_flightConverter(flightConverter) { }
    virtual ~FileFormat() = default;

    virtual FormatType getType() = 0;
    virtual std::string getName() = 0;
    virtual std::string getDescription() = 0;
    virtual bool check(std::string filename) = 0;
    virtual std::shared_ptr<FlightPlan> load(std::string filename) = 0;
    virtual bool save(std::shared_ptr<FlightPlan> flightPlan, std::string filename) = 0;

    void addWayPoint(std::shared_ptr<FlightPlan> flightPlan, WayPoint& wayPoint);
};

#endif //FILEFORMAT_H

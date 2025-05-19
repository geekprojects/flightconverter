//
// Created by Ian Parker on 23/10/2024.
//

#include "flightconverter.h"

#include <cfloat>
#include <string>
#include <memory>

#include "utils.h"
#include "formats/msfsiniformat.h"
#include "formats/xplanefmsformat.h"
#include "formats/simbriefjson.h"

using namespace std;
using namespace UFC;

FlightConverter::FlightConverter() : Logger("FlightConverter")
{
}

bool FlightConverter::init()
{
    if (m_initialised)
    {
        return true;
    }

    m_flightConnector = make_shared<FlightConnector>();

    auto dataSource = m_flightConnector->openDefaultDataSource();
    if (dataSource == nullptr)
    {
        log(ERROR, "Unable to open default data source");
        return false;
    }

    m_formats.push_back(new XPlaneFormat(this));
    m_formats.push_back(new MSFSIniFormat(this));
    m_formats.push_back(new SimbriefJson(this));

    m_initialised = true;

    return true;
}

std::shared_ptr<UFC::Airports> FlightConverter::getAirports()
{
    if (m_airports == nullptr)
    {
        m_airports = m_flightConnector->getDataSource()->loadAirports();
    }
    return m_airports;
}

std::shared_ptr<UFC::NavAids> FlightConverter::getNavData()
{
    if (m_navData == nullptr)
    {
        m_navData = m_flightConnector->getDataSource()->loadNavAids();
    }
    return m_navData;
}

std::shared_ptr<FlightPlan> FlightConverter::load(const std::string &fileName)
{
    vector<vector<string>> file = readTextFile(fileName, true);

    for (auto format : m_formats)
    {
        log(DEBUG, "Checking file format: %s", format->getName().c_str());
        if (format->check(fileName, file))
        {
            return format->load(fileName);
        }
    }

    log(ERROR, "Unable to detect file format");

    return nullptr;
}

FileFormat* FlightConverter::getFormat(FormatType type) const
{
    for (auto format : m_formats)
    {
        if (format->getType() == type)
        {
            return format;
        }
    }
    return nullptr;
}

std::shared_ptr<UFC::NavAid> FlightConverter::findNavAid(const std::string &id, UFC::Coordinate coordinate)
{
    auto navaids = getNavData()->findById(id);

    if (!navaids.empty())
    {
        shared_ptr<UFC::NavAid> nearestNavAid = nullptr;
        double distance = DBL_MAX;

        for (const auto& navaid : navaids)
        {
#if 0
            log(DEBUG, "findNavAid:  -> Found navaid: %s (%ls)", navaid->getId().c_str(), navaid->getLocation().toString().c_str());
#endif
            double d = GeoUtils::distance(navaid->getLocation(), coordinate);
            if (d < distance)
            {
                distance = d;
                nearestNavAid = navaid;
            }
        }
        if (nearestNavAid && distance < 0.1f)
        {
            return nearestNavAid;
        }
    }
    log(WARN, "findNavAid:  -> No navaids found with id: %s", id.c_str());

    return nullptr;
}

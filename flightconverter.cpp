//
// Created by Ian Parker on 23/10/2024.
//

#include "flightconverter.h"

#include "msfsformat.h"
#include "xplanefmsformat.h"

using namespace std;
using namespace UFC;

bool FlightConverter::init()
{
    m_flightConnector = make_shared<FlightConnector>();
    m_airports = m_flightConnector->openDefaultDataSource()->loadAirports();
    m_navData = m_flightConnector->openDefaultDataSource()->loadNavAids();

    m_msfsFormat = make_shared<MSFSFormat>(shared_from_this());
    m_xPlaneFormat = make_shared<XPlaneFormat>(shared_from_this());
    return true;
}

std::shared_ptr<FlightPlan> FlightConverter::load(const std::string &fileName)
{
    string lowerName = fileName;
    ranges::transform(lowerName, lowerName.begin(), ::tolower);

    if (lowerName.ends_with(".pln"))
    {
        return m_msfsFormat->load(fileName);
    }
    if (lowerName.ends_with(".fms"))
    {
        return m_xPlaneFormat->load(fileName);
    }
    return nullptr;
}

void FlightConverter::save(FormatType type, std::shared_ptr<FlightPlan> flightPlan, std::string filename)
{
    switch (type)
    {
        case FormatType::XPLANE:
            m_xPlaneFormat->save(flightPlan, filename);
            break;

        case FormatType::MSFS:
            m_msfsFormat->save(flightPlan, filename);
            break;
    }
}

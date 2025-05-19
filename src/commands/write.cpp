//
// Created by Ian Parker on 06/05/2025.
//

#include "write.h"
#include "flightconverter.h"

using namespace std;

bool WriteCommand::execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> flightPlan, map<std::string, std::string> args)
{
    FileFormat* format = m_flightConverter->getFormat(FormatType::XPLANE);
    if (format == nullptr)
    {
        return false;
    }

    string name = flightPlan->m_departureAirport + flightPlan->m_destinationAirport + ".fms";
    return format->save(flightPlan, name);
}

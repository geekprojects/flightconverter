//
// Created by Ian Parker on 19/05/2025.
//

#include "simbrief.h"

#include "flightconverter.h"
#include "formats/simbriefjson.h"

using namespace std;

std::vector<option> SimbriefCommand::getOptions() const
{
    vector<option> options;
    options.push_back({"user", required_argument, 0,  'u' });
    return options;
}

bool SimbriefCommand::execute(FlightPlanContainer* flightPlanContainer, std::map<std::string, std::string> args)
{
    auto simbriefJson = static_cast<SimbriefJson*>(m_flightConverter->getFormat(FormatType::SIMBRIEF_JSON));
    if (simbriefJson == nullptr)
    {
        return false;
    }
    auto flightPlan = simbriefJson->fetch(args["user"]);
    if (flightPlan == nullptr)
    {
        return false;
    }

    flightPlanContainer->m_flightPlans.push_back(flightPlan);

    return true;
}

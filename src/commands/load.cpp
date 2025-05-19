//
// Created by Ian Parker on 06/05/2025.
//

#include "load.h"

#include "flightconverter.h"

using namespace std;

vector<option> LoadCommand::getOptions() const
{
    vector<option> options;
    options.push_back({"file",     required_argument, 0,  'f' });
    options.push_back({"format",     required_argument, 0,  'm' });
    return options;
}

bool LoadCommand::execute(FlightPlanContainer* flightPlanContainer, map<std::string, std::string> args)
{
    auto it = args.find("file");
    if (it == args.end())
    {
        return false;
    }
    string fileName = it->second;

    auto flightPlan = m_flightConverter->load(fileName);
    if (flightPlan == nullptr)
    {
        return false;
    }
    flightPlanContainer->m_flightPlans.push_back(flightPlan);

    return true;
}

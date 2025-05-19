//
// Created by Ian Parker on 06/05/2025.
//

#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <getopt.h>

#include "flightplan.h"

class Command
{
 protected:
    FlightConverter* m_flightConverter;

 public:
    explicit Command(FlightConverter* flightConverter)
    {
        m_flightConverter = flightConverter;
    }
    virtual ~Command() = default;

    [[nodiscard]] virtual std::vector<option> getOptions() const
    {
        return {};
    }

    virtual bool execute(FlightPlanContainer* flightPlanContainer, std::map<std::string, std::string> args)
    {
        // Make a copy of the list. The command may change it.
        auto flightPlanList = flightPlanContainer->m_flightPlans;
        for (auto flightPlan : flightPlanList)
        {
            bool res;
            res = execute(flightPlanContainer, flightPlan, args);
            if (!res)
            {
                return false;
            }
        }
        return true;
    }

    virtual bool execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> flightPlan, std::map<std::string, std::string> args) { return false; }
};

#endif //COMMAND_H

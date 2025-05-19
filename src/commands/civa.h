//
// Created by Ian Parker on 06/05/2025.
//

#ifndef CIVA_H
#define CIVA_H

#include "commands/command.h"

class CivaCommand : public Command
{
 public:
    CivaCommand(FlightConverter* flightConverter) : Command(flightConverter) {}
    virtual ~CivaCommand() = default;

    bool execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> flightPlan, std::map<std::string, std::string> args) override;
};

#endif //CIVA_H

//
// Created by Ian Parker on 06/05/2025.
//

#ifndef WRITE_H
#define WRITE_H
#include "command.h"


class WriteCommand : public Command
{
 public:
    explicit WriteCommand(FlightConverter* flightConverter) : Command(flightConverter) {}
    bool execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> flightPlan, std::map<std::string, std::string> args) override;
};

#endif //WRITE_H

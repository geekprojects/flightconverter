//
// Created by Ian Parker on 06/05/2025.
//

#ifndef SPLIT_H
#define SPLIT_H
#include "command.h"


class SplitCommand : public Command
{
 public:
    explicit SplitCommand(FlightConverter* flightConverter) : Command(flightConverter) {}
    bool execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> flightPlan, std::map<std::string, std::string> args) override;
};



#endif //SPLIT_H

//
// Created by Ian Parker on 06/05/2025.
//

#ifndef UPDATE_H
#define UPDATE_H
#include "command.h"

class UpdateCommand : public Command
{
 public:
    explicit UpdateCommand(FlightConverter* flightConverter) : Command(flightConverter) {}

    bool execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> flightPlan, std::map<std::string, std::string> args) override;

    void removeDuplicates(std::shared_ptr<FlightPlan> flightPlan);
};



#endif //UPDATE_H

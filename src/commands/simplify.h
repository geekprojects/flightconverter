//
// Created by Ian Parker on 06/05/2025.
//

#ifndef SIMPLIFY_H
#define SIMPLIFY_H
#include <memory>

#include "command.h"
#include "flightconverter.h"

class SimplifyCommand : public Command
{
 public:
    explicit SimplifyCommand(FlightConverter* flightConverter) : Command(flightConverter) {}

    bool execute(FlightPlanContainer* flightPlanContainer, std::shared_ptr<FlightPlan> flightPlan, std::map<std::string, std::string> args) override;

};


#endif //SIMPLIFY_H

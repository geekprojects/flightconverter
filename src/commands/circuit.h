//
// Created by Ian Parker on 15/05/2025.
//

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "command.h"

class CircuitCommand : public Command
{
 public:
    explicit CircuitCommand(FlightConverter* flightConverter) : Command(flightConverter) {}

    [[nodiscard]] std::vector<option> getOptions() const override;

    bool execute(FlightPlanContainer* flightPlanContainer, std::map<std::string, std::string> args) override;
};


#endif //CIRCUIT_H

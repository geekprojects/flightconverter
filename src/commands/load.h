//
// Created by Ian Parker on 06/05/2025.
//

#ifndef LOAD_H
#define LOAD_H
#include "command.h"

class LoadCommand : public Command
{
 public:
    explicit LoadCommand(FlightConverter* flightConverter) : Command(flightConverter) {}

    [[nodiscard]] std::vector<option> getOptions() const override;

    bool execute(FlightPlanContainer* flightPlanContainer, std::map<std::string, std::string> args) override;
};



#endif //LOAD_H

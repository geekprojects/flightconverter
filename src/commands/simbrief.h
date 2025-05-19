//
// Created by Ian Parker on 19/05/2025.
//

#ifndef SIMBRIEF_H
#define SIMBRIEF_H
#include "command.h"

class SimbriefCommand : public Command
{
 public:
    explicit SimbriefCommand(FlightConverter* flightConverter) : Command(flightConverter) {}

    [[nodiscard]] std::vector<option> getOptions() const override;

    bool execute(FlightPlanContainer* flightPlanContainer, std::map<std::string, std::string> args) override;
};



#endif //SIMBRIEF_H

//
// Created by Ian Parker on 23/10/2024.
//

#ifndef MSFSFORMAT_H
#define MSFSFORMAT_H

#include "flightconverter.h"

class MSFSFormat : public FileFormat
{
 public:
    std::shared_ptr<FlightPlan> load(std::shared_ptr<FlightConverter> flightConverter, std::string filename) override;
    void save(std::shared_ptr<FlightConverter> flightConverter, std::shared_ptr<FlightPlan> flightPlan, std::string filename) override {};
};

#endif //MSFSFORMAT_H

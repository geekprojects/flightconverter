//
// Created by Ian Parker on 23/10/2024.
//

#ifndef XPLANEFMSFORMAT_H
#define XPLANEFMSFORMAT_H

#include "flightconverter.h"

class XPlaneFormat : public FileFormat
{
 public:
    std::shared_ptr<FlightPlan> load(std::shared_ptr<FlightConverter> flightConverter, std::string filename) override { return nullptr; };
    void save(std::shared_ptr<FlightConverter> flightConverter, std::shared_ptr<FlightPlan> flightPlan, std::string filename) override;

};

#endif //XPLANEFMSFORMAT_H

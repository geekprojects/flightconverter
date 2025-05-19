//
// Created by Ian Parker on 23/10/2024.
//

#ifndef XPLANEFMSFORMAT_H
#define XPLANEFMSFORMAT_H

#include "formats/fileformat.h"

class XPlaneFormat : public FileFormat
{
 public:
    explicit XPlaneFormat(FlightConverter* flightConverter)
        : FileFormat(flightConverter)
    {
    }

    FormatType getType() override { return FormatType::XPLANE; }
    std::string getName() override { return "xplane-fms"; }
    std::string getDescription() override { return "X-Plane FMS"; }

    bool check(std::string filename) override;
    std::shared_ptr<FlightPlan> load(std::string filename) override;
    bool save(std::shared_ptr<FlightPlan> flightPlan, std::string filename) override;
};

#endif //XPLANEFMSFORMAT_H

//
// Created by Ian Parker on 23/10/2024.
//

#ifndef MSFSINIFORMAT_H
#define MSFSINIFORMAT_H

#include "formats/fileformat.h"

class MSFSIniFormat : public FileFormat
{
 public:
    explicit MSFSIniFormat(FlightConverter* flightConverter)
        : FileFormat(flightConverter)
    {
    }

    FormatType getType() override { return FormatType::MSFS_INI; }
    std::string getName() override { return "msfs-ini"; }
    std::string getDescription() override { return "MSFS INI"; }
    bool check(std::string filename) override;
    std::shared_ptr<FlightPlan> load(std::string filename) override;
    bool save(std::shared_ptr<FlightPlan> flightPlan, std::string filename) override {return false; };
};

#endif //MSFSFORMAT_H

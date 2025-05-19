//
// Created by Ian Parker on 19/05/2025.
//

#ifndef SIMBRIEFJSON_H
#define SIMBRIEFJSON_H

#include "fileformat.h"

#include <nlohmann/json.hpp>

class SimbriefJson : public FileFormat
{
    std::shared_ptr<FlightPlan> load(nlohmann::json json);

 public:
    explicit SimbriefJson(FlightConverter* flightConverter);

    bool check(std::string filename, std::vector<std::vector<std::string>> file) override;
    std::shared_ptr<FlightPlan> load(std::string filename) override;

    std::shared_ptr<FlightPlan> fetch(std::string username);

    FormatType getType() override { return FormatType::SIMBRIEF_JSON;}

    std::string getName() override { return "simbrief-json"; }

    std::string getDescription() override { return "Simbrief JSON"; }

    bool save(std::shared_ptr<FlightPlan> flightPlan, std::string filename) override;
};


#endif //SIMBRIEFJSON_H

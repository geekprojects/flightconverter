//
// Created by Ian Parker on 19/05/2025.
//

#include "simbriefjson.h"
#include "flightconverter.h"

#include <curl/curl.h>

using namespace std;
using namespace nlohmann;
using namespace UFC;

struct Payload
{
    char* response = nullptr;
    size_t size = 0;
};

static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    Payload* mem = (Payload *)clientp;

    char *ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
    if (!ptr)
    {
        return 0;  /* out of memory */
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

SimbriefJson::SimbriefJson(FlightConverter* flightConverter)
    : FileFormat(flightConverter)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

bool SimbriefJson::check(string filename, std::vector<std::vector<std::string>> file)
{
    if (file.at(0).at(0) != "{")
    {
        return false;
    }

    FILE* fd = fopen(filename.c_str(), "r");
    auto json = json::parse(fd);
    fclose(fd);

    return (json["fetch"]["status"].get<string>() == "Success");
}

shared_ptr<FlightPlan> SimbriefJson::load(string filename)
{
    printf("Loading SimBrief plan!\n");

    FILE* fd = fopen(filename.c_str(), "r");
    auto json = json::parse(fd);
    fclose(fd);
    return load(json);
}

shared_ptr<FlightPlan> SimbriefJson::fetch(std::string username)
{
    string url = "https://www.simbrief.com/api/xml.fetcher.php?username=" + username + "&json=1";
    printf("Loading SimBrief plan from URL: %s\n", url.c_str());

    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* send all data to this function  */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

    /* we pass our 'chunk' struct to the callback function */
    Payload chunk;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);

    json json = json::parse(chunk.response);

    free(chunk.response);

    return load(json);
}

shared_ptr<FlightPlan> SimbriefJson::load(json json)
{
    auto flightPlan = make_shared<FlightPlan>();

    flightPlan->m_cycle = atoi(json["params"]["airac"].get<string>().c_str());
    flightPlan->m_departureAirport = json["origin"]["icao_code"].get<string>();
    flightPlan->m_destinationAirport = json["destination"]["icao_code"].get<string>();

    auto plan = json["navlog"]["fix"];
    for (auto fix : plan)
    {
        string ident = fix["ident"].get<string>();
        printf("Fix: %s\n", ident.c_str());
        if (fix["is_sid_star"].get<string>() == "1")
        {
        printf("Fix:  -> Skipping SID/STAR\n");
            continue;
        }
        string type = fix["type"].get<string>();
        float lat = atof(fix["pos_lat"].get<string>().c_str());
        float lon = atof(fix["pos_long"].get<string>().c_str());
        UFC::Coordinate coordinate = {lat, lon};
        printf("Fix:  -> lat=%0.2f, lon=%0.2f\n", lat, lon);

        WayPoint wayPoint;
        if (type == "vor" || type == "wpt")
        {
            auto navAid = m_flightConverter->findNavAid(ident, coordinate);
            if (navAid == nullptr)
            {
                printf("Fix:  -> No NavAid found\n");
                return nullptr;
            }
            printf("Fix:  -> Found NavAid: %s\n", navAid->getId().c_str());
            wayPoint = WayPoint(navAid);
        }
        else if (type == "ltlg")
        {
            if (ident == "TOC" || ident == "TOD")
            {
                continue;
            }
            wayPoint.id = ident;
            wayPoint.coordinate = coordinate;
            wayPoint.type = NavAidType::WAY_POINT;
        }
        flightPlan->m_waypoints.push_back(wayPoint);
    }
    return flightPlan;
}

bool SimbriefJson::save(std::shared_ptr<FlightPlan> flightPlan, std::string filename)
{
    return false;
}

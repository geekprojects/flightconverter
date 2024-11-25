
#include <iostream>

#include "flightconverter.h"
#include "flightplan.h"
#include "msfsformat.h"
#include "xplanefmsformat.h"

using namespace std;
using namespace UFC;

// The CIVA can only handle 9 way points, so split the flight plan into multiple files
void splitCIVA(shared_ptr<FlightConverter> converter, shared_ptr<FlightPlan> plan, FileFormat* format, string name)
{
    printf("Size=%lu\n", plan->m_waypoints.size());
    for (int i = 0, part = 1; i < plan->m_waypoints.size(); i += 8, part++)
    {
        shared_ptr<FlightPlan> segment = make_shared<FlightPlan>();
        *segment = *plan;
        if (i > 0)
        {
            segment->m_departureAirport = "";
            segment->m_departureRunway = "";
        }

        int end = i + 9;
        if (end >= plan->m_waypoints.size())
        {
            end = plan->m_waypoints.size();
        }
        else
        {
            segment->m_destinationAirport = "";
            segment->m_destinationRunway = "";
        }

        segment->m_waypoints = vector(plan->m_waypoints.begin() + i, plan->m_waypoints.begin() + end);

        format->save(converter, segment, name + "-part" + to_string(part) + ".fms");
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <filename>", argv[0]);
        exit(1);
    }
    string plnFile = argv[1];

    //auto idx = plnFile.find_last_of('.');
    //auto fmsFile = plnFile.substr(0, idx) + ".fms";

    MSFSFormat msfsFormat;
    XPlaneFormat xPlaneFormat;

    auto flightConverter = make_shared<FlightConverter>();
    flightConverter->init();

    auto fp = msfsFormat.load(flightConverter, plnFile);

    fp->updateWaypoints(flightConverter);

    string name = fp->m_departureAirport + fp->m_destinationAirport;
    xPlaneFormat.save(flightConverter, fp, name + ".fms");
    splitCIVA(flightConverter, fp, &xPlaneFormat, name);

    return 0;
}

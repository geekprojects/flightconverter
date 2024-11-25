//
// Created by Ian Parker on 23/10/2024.
//

#include "flightconverter.h"

using namespace std;
using namespace UFC;

bool FlightConverter::init()
{
    m_flightConnector = make_shared<FlightConnector>();
    m_airports = m_flightConnector->openDefaultDataSource()->loadAirports();
    m_navData = m_flightConnector->openDefaultDataSource()->loadNavData();
    return true;
}

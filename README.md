Flight Converter
==

Flight Converter is a tool for manipulating flight plans. It can load from different
sources, transform them and save or display them using different commands.

This is only for flight simulation, of course! It is a work in progress!

## Features
* Loads and saves:
  * X-Plane FMS format
  * MSFS .PLN
* Verify and update/replace way points from older AIRAC cycles.
* Breaks flight plans in to multiple parts for compatibility with the CIVA Navigation System.


## Commands

You use flightconverter by chaining different commands together to manipulate or create
your flight plans.

### Convert and update

Load in an old MSFS PLN file, update to the current AIRAC and write out an X-Plane FMS file:

flightconverter load --file EGLL-KJFK.PLN update write --file EGLLKJFK01.fms


### Prepare CIVA waypoints for Concorde or other classic airliner

Load an X-Plane FMS file, update to the current AIRAC, reduce the number of waypoints
and display it in a form ready to be entered in to a CIVA INS.

flightconverter load --file EGLLOBBI01.fms update simpify civa


### Create traffic circuits/patterns

Generate a simple flight plan for taking off, circling the airport and landing. This
is useful for practicing takeoffs and landings in airliners:

flightconverter circuit --airport EGLL --runway 27L write



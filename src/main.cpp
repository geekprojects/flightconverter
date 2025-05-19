
#include "flightconverter.h"
#include "flightplan.h"
#include "commands/circuit.h"
#include "commands/civa.h"
#include "commands/load.h"
#include "commands/simplify.h"
#include "commands/split.h"
#include "commands/update.h"
#include "commands/write.h"
#include "commands/simbrief.h"

using namespace std;
using namespace UFC;

unique_ptr<Command> getCommand(FlightConverter* flightConverter, const string& commandStr)
{
    if (commandStr == "load")
    {
        return make_unique<LoadCommand>(flightConverter);
    }
    if (commandStr == "update")
    {
        return make_unique<UpdateCommand>(flightConverter);
    }
    if (commandStr == "simplify")
    {
        return make_unique<SimplifyCommand>(flightConverter);
    }
    if (commandStr == "split")
    {
        return make_unique<SplitCommand>(flightConverter);
    }
    if (commandStr == "civa")
    {
        return make_unique<CivaCommand>(flightConverter);
    }
    if (commandStr == "write")
    {
        return make_unique<WriteCommand>(flightConverter);
    }
    if (commandStr == "circuit")
    {
        return make_unique<CircuitCommand>(flightConverter);
    }
    if (commandStr == "simbrief")
    {
        return make_unique<SimbriefCommand>(flightConverter);
    }
    printf("Unknown command: %s\n", commandStr.c_str());
    exit(1);
}

string getOptStr(const vector<option>& options)
{
    string optstr;
    for (auto const& o : options)
    {
        if (isprint(o.val))
        {
            optstr += (char)o.val;
            if (o.has_arg == required_argument)
            {
                optstr += ":";
            }
        }
    }
    return optstr;
}

int main(int argc, char** argv)
{
    FlightConverter flightConverter;
    flightConverter.init();

    // Skip argv[0]
    argc--;
    argv++;

    FlightPlanContainer flightPlanContainer;
    while (argc > 0)
    {
        string commandStr = argv[0];

        unique_ptr<Command> command;
        command = getCommand(&flightConverter, commandStr);
        printf("Command: %s\n", commandStr.c_str());

        auto options = command->getOptions();

        // Build an option string
        string optstr = getOptStr(options);

        map<string, string> args;
        int option_index;
        optind = 0;
        while (getopt_long(argc, argv, optstr.c_str(), options.data(), &option_index) != -1)
        {
            string name = options.at(option_index).name;

            string arg;
            if (optarg != nullptr)
            {
                arg = optarg;
            }
            args.try_emplace(name, arg);
        }

        bool res;
        res = command->execute(&flightPlanContainer, args);
        if (!res)
        {
            exit(2);
        }

        argv += optind;
        argc -= optind;
    }

    return 0;
}

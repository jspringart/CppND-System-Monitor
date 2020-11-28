#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    prevActive = active;
    active = LinuxParser::ActiveJiffies();
    prevIdle = idle;
    idle = LinuxParser::IdleJiffies();
    prevTotal = total;
    total = LinuxParser::Jiffies();
    float util_percentage = ((float)active - (float)prevActive) / ((float)total - (float)prevTotal);
    return util_percentage; 
}
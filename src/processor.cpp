#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() { 
    prevActive = active;
    active = LinuxParser::ActiveJiffies();
    prevIdle = idle;
    idle = LinuxParser::IdleJiffies();
    prevTotal = total;
    total = LinuxParser::Jiffies();
    float const util_percentage = static_cast<float>(active - prevActive) / static_cast<float>(total - prevTotal);
    return util_percentage; 
}
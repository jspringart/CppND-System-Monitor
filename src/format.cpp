#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) { 
    std::ostringstream formatted_time;
    long time = seconds;
    long hours = time / 3600;
    time = time % 3600;
    long min = time / 60;
    long sec = time % 60;
    formatted_time << std::setfill('0') << std::setw(2) << hours << ':' << std::setw(2) << min << ':' << std::setw(2) << sec;
    return formatted_time.str(); 
}
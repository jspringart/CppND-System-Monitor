#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

Process::Process(int pid) : pid_(pid), user_(LinuxParser::User(pid_)), cmd_(LinuxParser::Command(pid_)), ram_(LinuxParser::Ram(pid_)) {}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    int hertz = sysconf(_SC_CLK_TCK);
    total = (float)LinuxParser::ActiveJiffies(pid_) / (float)hertz;    
    seconds = (float)LinuxParser::UpTime() - (float)UpTime();  

    // float delta = seconds - prev_seconds;

    // if(pid_ == 2404)
    // {
    //     float test = 345;
    //     float d = test + 4;
    // }

    // if(delta == 0 || ticks < 2) {
    //     cpu_ = 0;
    //     ticks++;
    // }
    // else
    // {
    //     prev_total = total;
    //     prev_seconds = seconds;
    //     cpu_ = (float)(total - prev_total) / (float)(seconds - prev_seconds);
    //     ticks = 0;
    // }
    cpu_ = (float)total / (float)seconds;
    
    return cpu_;
}

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return to_string(stol(ram_) / 1000); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return  this->cpu_ > a.cpu_;
}

bool Process::operator==(Process const& a) const {
    return this->pid_ == a.pid_;
}
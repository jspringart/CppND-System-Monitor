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

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
    int hertz = sysconf(_SC_CLK_TCK);
    total = (float)LinuxParser::ActiveJiffies(pid_) / (float)hertz;    
    seconds = (float)UpTime();  
    cpu_ = (float)total / (float)seconds;
    
    return cpu_;
}

string Process::Command() { return cmd_; }

string Process::Ram() { return to_string(stol(ram_) / 1000); }

string Process::User() { return user_; }

long int Process::UpTime() { 
    long total_uptime = LinuxParser::UpTime();
    long local_uptime = LinuxParser::UpTime(pid_);
    return (long)(total_uptime - local_uptime); 
}

bool Process::operator<(Process const& a) const { return  this->cpu_ > a.cpu_; }
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
    int const hertz = sysconf(_SC_CLK_TCK);
    long const active_jiffies = LinuxParser::ActiveJiffies(pid_);
    if(active_jiffies != 0) {
        total = static_cast<float>(active_jiffies) / static_cast<float>(hertz);  
    }
    else {
        return 0.0;
    }
      
    seconds = static_cast<float>(UpTime());  
    cpu_ = total / seconds;
    
    return cpu_;
}

string Process::Command() { return cmd_; }

string Process::Ram() { return to_string(stol(ram_) / 1000); }

string Process::User() { return user_; }

long int Process::UpTime() { 
    long const total_uptime = LinuxParser::UpTime();
    long const local_uptime = LinuxParser::UpTime(pid_);
    return (long)(total_uptime - local_uptime); 
}

bool Process::operator<(Process const& a) const { return  this->cpu_ > a.cpu_; }
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (filestream.is_open() && std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  float mem_total{0};
  float mem_free{0};
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while(filestream.is_open() && std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "MemTotal:") {
        mem_total = std::stof(value);
      }
      if(key == "MemFree:") {
        mem_free = std::stof(value);
      }
    }    
  }
  float mem_usage = mem_total - mem_free;
  return (mem_usage / mem_total);
}

long LinuxParser::UpTime() {
  long uptime{0};
  string line, u_time, i_time;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> u_time >> i_time;
    uptime = std::stol(u_time);
  }
  return uptime;
}

long LinuxParser::Jiffies() {
  long total{0};
  vector<string> util = CpuUtilization();
  // Guest time is alread included in usertime
  for (int i = 0; i <= kSteal_; i++) {
    total += stol(util[i]);
  } 

  return total; 
}

vector<std::string> LinuxParser::ProcessStat(int pid) {
  vector<string> process_stat{};
  string line, value;  
  try { 
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);   
    if (filestream.is_open()) {    
      while(std::getline(filestream, line)) {
        std::istringstream linestream(line);       
        while(linestream >> value) {
          process_stat.push_back(value);
        }    
      }      
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  return process_stat;  
}

long LinuxParser::ActiveJiffies(int pid) { 
  long utime = stol(ProcessStat(pid)[kUtime]); 
  long stime = stol(ProcessStat(pid)[kStime]);
  long cutime = stol(ProcessStat(pid)[kCutime]);
  long cstime = stol(ProcessStat(pid)[kCstime]);

  long total_time = utime + stime + cutime + cstime;
  return total_time;

}

long LinuxParser::ActiveJiffies() { 
  long active{0};
  active = stol(CpuUtilization()[kUser_]) + stol(CpuUtilization()[kNice_]) + stol(CpuUtilization()[kSystem_]) + stol(CpuUtilization()[kIRQ_]) + stol(CpuUtilization()[kSoftIRQ_]) + + stol(CpuUtilization()[kSteal_]);
  return active; 
}

long LinuxParser::IdleJiffies() { 
  long idle{0};
  idle = stol(CpuUtilization()[kIdle_]) + stol(CpuUtilization()[kIOwait_]);
  return idle; 
}

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpus;
  string line, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      while(filestream.is_open() && std::getline(filestream, line)) {
        std::istringstream linestream(line);
        linestream >> value;
        if(value == "cpu") {       
          while(linestream >> value) {
            cpus.push_back(value);
          }
        }      
      } 
  }
  return cpus; 
}

int LinuxParser::TotalProcesses() { 
  int total_procs{0};
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(filestream.is_open() && std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "processes") {
        total_procs = std::stoi(value);
      }
    }    
  }
  return total_procs;  
}

int LinuxParser::RunningProcesses() { 
  int running_procs{0};
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(filestream.is_open() && std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running") {
        running_procs = std::stoi(value);
      }
    }    
  }
  return running_procs; 
}

string LinuxParser::Command(int pid) { 
  string line, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

string LinuxParser::Ram(int pid) { 
  string line, key, value, ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(filestream.is_open() && std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmSize:") {
        ram = value;
      }
    }    
  }
  return ram; 
}

string LinuxParser::Uid(int pid) {
  string uid{0};
  string line, key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(filestream.is_open() && std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "Uid:") {
        uid = value;
      }
    }    
  }
  return uid;
}

string LinuxParser::User(int pid) {
  string user{0};
  string line, user_name, x, uid;
  std::ifstream filestream(kPasswordPath);
  string search_uid = Uid(pid);
  if (filestream.is_open()) {
    while(filestream.is_open() && std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user_name >> x >> uid;
      if(search_uid == uid) {
        user = user_name;
      }
    }    
  }
  return user; 
}

long LinuxParser::UpTime(int pid) { 
  long starttime = (stol(ProcessStat(pid)[kStarttime]) / sysconf(_SC_CLK_TCK));
  return starttime; 
}
#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator==(Process const& a) const;

  // TODO: Declare any necessary private members
 private:
  int pid_{0};
  float cpu_{0};
  std::string user_{""};
  //float prev_seconds{0};
  float seconds{0};
  //float prev_total{0};
  float total{0};
  //long ticks{0};
  std::string cmd_{""};
  std::string ram_{""};
};

#endif
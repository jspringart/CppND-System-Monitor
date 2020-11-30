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
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
  int pid_{0};  
  float cpu_{0};  
  float seconds{0};
  float total{0};
  std::string user_{""};
  std::string cmd_{""};
  std::string ram_{""};
  
};

#endif
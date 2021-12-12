#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"
#include "processor.h"
using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  float jiffies_;  // public variable to be accessible

  Process(int pid, int cpuJiffies, long oldJiffies)
      : pid_(pid), oldJiffies_(oldJiffies), cpuJiffies_(cpuJiffies) {
    user_ = User();
    command_ = Command();
    cpuUtilization_ = CpuUtilization();
    ram_ = Ram();
    upTime_ = UpTime();
  }
  // Declare any necessary private members
 private:
  int pid_;
  string user_;
  string command_;
  float cpuUtilization_;
  string ram_;
  long int upTime_;
  long oldJiffies_;
  int cpuJiffies_;
};
#endif
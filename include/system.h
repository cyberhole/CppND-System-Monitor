#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

  long GetOldProcessJiffies(
      int pid);  // returns the previous value of jiffies for an old pid;

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::vector<Process> oldProcesses_ =
      {};  // vector that will be used to make a backup of the processes_
};

#endif
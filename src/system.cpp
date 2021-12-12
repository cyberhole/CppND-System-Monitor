#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
 * criteria */

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Comparison between to processes to find the one with more cpu
bool Compare(Process a, Process b) {
  return b < a;  // true if b<a
}

// Return a vector with the processes of the system
vector<Process>& System::Processes() {
  /*
  First the processes_ vector is reset so any old value is deleted

  Then each pid is pushed back into the vector but the constructor of the
  process is different from the one in the standard version of the work,
  now it has the cpu jiffies (difference of jiffies in the last amount
  of time checked) and the old jiffies value for the pid to be compared
  with the new
  */
  processes_ = {};                        // reset the vector processes_:
  int size = LinuxParser::Pids().size();  // size of the number of active pids
  int pid;

  for (int i = 0; i < size; i++) {
    pid = LinuxParser::Pids()[i];
    processes_.push_back(
        Process(pid, cpu_.getJiffies(), System::GetOldProcessJiffies(pid)));
  }

  std::sort(processes_.begin(), processes_.end(), Compare);  // Order the vector

  oldProcesses_ =
      {};  // clears the oldProcesses_ vector in order to make a new "coppy"
  for (int i = 0; i < size; i++) {
    oldProcesses_.push_back(processes_[i]);
  }
  return processes_;
}

// return the old value of jiffies for a pid
long System::GetOldProcessJiffies(int pid) {
  /*
  reads the vector oldProcesses_ to find the jiffies value
  and returns it
  else returns the new value and then the comparison made at process.cpp will
  give 0 for the difference between the old and the new.
  */
  for (int i = 0; i < (int)oldProcesses_.size(); i++)
    if (oldProcesses_[i].Pid() == pid) {
      return oldProcesses_[i].jiffies_;
    }
  return LinuxParser::ActiveJiffies(pid);
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

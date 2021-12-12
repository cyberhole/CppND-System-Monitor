#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"  // new from me

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  /* the old jiffies value is set by the system.cpp file and is obtained from a
    backup of the vector with the processes. then it is compared with the new
    jiffies to see the difference that difference is then divided to have the
    %of active cpu being used by that process
  */
  jiffies_ = LinuxParser::ActiveJiffies(pid_);  // new process jiffies
  float differenceJiffies =
      jiffies_ - oldJiffies_;  // comparison with old value
  return (differenceJiffies) /
         (float)cpuJiffies_;  // % of active cpu used for the process
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return cpuUtilization_ < a.cpuUtilization_;
}

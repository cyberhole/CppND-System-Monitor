#include "processor.h"

#include <iostream>
#include <vector>

#include "linux_parser.h"

using std::string;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  /* to make the calculation of utilization of the cpu in the last amount of
  time first the amount of time that has passed has been calculated then if
  finds the difference in the number of jiffies of the cpu and the calculation
  of (jiffies/time)/frequency is returned
  */
  int upTime_;
  float jiffOverTime;

  // calculates the amount of time that has passed by comparing it with the
  // previout upTime
  upTime.push_back(LinuxParser::UpTime());
  if (upTime.size() == 2) {
    upTime_ = upTime[1] - upTime[0];
    upTime.erase(upTime.begin());  // deletes the first value of the vector so
                                   // the second value becomes the first one for
                                   // the next time it performs this calculation
  } else
    upTime_ = 1;

  // finds the difference in the number of jiffies in the cpu
  jiffies.push_back(LinuxParser::ActiveJiffies());
  if (jiffies.size() == 2) {
    jiffies_ = jiffies[1] - jiffies[0];
    jiffies.erase(jiffies.begin());
  } else
    jiffies_ = 1;

  // performs the calculation of the amout of jiffies in that time and compares
  // it with the total frequency of the cpu
  jiffOverTime = jiffies_ / upTime_;
  return jiffOverTime / frequency_;
}

// Finds the frequency value for the cpu
int Processor::Frequency() {
  /*
at /proc/cpuinfo the word MHz is found and the next value is returned as the
frequency of the cpu
  */
  string line, cpu, key, value;
  int frequency;

  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kCpuinfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> cpu >> key >> value) {
        if (key == "MHz") {
          std::istringstream linestream(value);
          linestream >> frequency;
          return frequency;
        }
      }
    }
  }
  return 0;
}

// Return the number of jifies of the cpu
long int Processor::getJiffies() const {
  /*
  it returns the change in the number of jiffies in the last "time unit"
  */
  return jiffies_;
}
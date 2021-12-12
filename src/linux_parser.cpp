#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
    while (std::getline(filestream, line)) {
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
  string os, kernel, version;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  /*
  Function returns the memory utilization of the system
  */
  // declaration of local variables
  int memTotal, memAvailable;
  float memUtilization{0};
  string line, temp, line_;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);  // at /proc/meminfo
  // The 1st and 3rd line of the meminfo are going to be needed
  if (stream.is_open()) {
    std::getline(stream,
                 line);  // reads the first line and gets the second word
    std::istringstream linestream(line);
    linestream >> temp >> memTotal;
    std::getline(stream, line);  // reads the next two lines and in the second
                                 // line it also gets the second word
    std::getline(stream, line);
    std::istringstream linestream_(line);
    linestream_ >> temp >> memAvailable;
  }
  memUtilization = ((((float)memTotal - memAvailable)) /
                    memTotal);  // converts the first one to float to make sure
  return memUtilization;        // returns the memory utilization
}

// Read and return the time the system is up
long LinuxParser::UpTime() {
  /*
  Function returns uptime of the system.
  */
  long upTime;  // creation of variable where the total uptime will be saved
  string line;
  std::ifstream stream(
      kProcDirectory +
      kUptimeFilename);  // at /proc/uptime there are two values
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >>
        upTime;  // first word is the time that the machine is up in seconds the
                 // second is the idle time but wont be needed
  }
  return upTime;  // return the uptime
}

// Reads and returns a vector with all the jiffie values for the cpu
vector<int> LinuxParser::Jiffies() {
  // creation of local variables
  vector<int> Cpu_{};
  string cpu, line;
  int kUser_, kNice_, kSystem_, kIdle_, kIOwait_, kIRQ_, kSoftIRQ_, kSteal_,
      kGuest_, kGuestNice_;

  std::ifstream stream(LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename);  // at /proc/stat
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> kUser_ >> kNice_ >> kSystem_ >> kIdle_ >> kIOwait_ >>
        kIRQ_ >> kSoftIRQ_ >> kSteal_ >> kGuest_ >> kGuestNice_;
  }
  // saves all the variables inside the vector and returns it
  Cpu_.insert(Cpu_.end(), {kUser_, kNice_, kSystem_, kIdle_, kIOwait_, kIRQ_,
                           kSoftIRQ_, kSteal_, kGuest_, kGuestNice_});
  return Cpu_;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  /*
  Function accepts a pid as input and returns the active jiffies of that pid.
  */
  // Declaration of variables used
  string line, key;
  int system, userChildren, systemChildren, i{0}, activeJiffies{0};
  //
  std::ifstream stream(kProcDirectory + to_string(pid) +
                       kStatFilename);  // Open directory proc/$pid$/stat
  //
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      /*the positions with the values that are going to be used are 14 15 16 and
       17
       while the loop is reading the string, if it gets to the 14th position it
       saves that value and the other 3 that follow*/
      while (linestream >> key && i < 15) {
        if (i == 14) {
          linestream >> system >> userChildren >>
              systemChildren;  // as key is already defined
          activeJiffies = std::stol(key) + system + userChildren +
                          systemChildren;  // sum of all 4 values of jiffies
          return activeJiffies;            // returns the sum
        }
        // Some of the pid have more than one word with a space between in the
        // "second position" and this is a safety to make sure i there is 2 for
        // the (state) position
        if (key == "R" || key == "S" || key == "D" || key == "Z" ||
            key == "T" || key == "t" || key == "W" || key == "X" ||
            key == "x" || key == "K" || key == "w") {
          i = 2;
        }
        i++;
      }
    }
  }
  return 0;  // if it isn't able to find and read the values it returns 0
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  /* Active jiffies = User+System source ::
   https://www.anshulpatel.in/post/linux_cpu_percentage/ */
  vector<int> jiffies =
      LinuxParser::Jiffies();  // coppy of the jiffies vector from the cpu
  if (jiffies.size() > 0)
    return jiffies[0] + jiffies[2];  // reads the first and 3rd words
  else
    return 0;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<int> jiffies =
      LinuxParser::Jiffies();  // coppy of the jiffies vector from the cpu
  if (jiffies.size() > 0)
    return jiffies[3];  // reads the 4th position of the vector
  else
    return 0;
}

// Read and return CPU utilization
// I've done this function at Processor.cpp
// vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  // Declaration of local variables
  int totalProcesses = 0;
  string line, key, value;

  std::ifstream filestream(kProcDirectory + kStatFilename);  // /proc/stat

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {  // once it finds the word "processes"
          std::istringstream linestream(value);
          linestream >>
              totalProcesses;  // it saves the next word into "totalProcesses"
          return totalProcesses;  // and then returns it
        }
      }
    }
  }
  return 0;  // else returns 0
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  /*
same thing as TotalProcesses()
it finds the value procs_running and then returns the next word which is the
value of the total number of processe running
  */
  int procsRunning = 0;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);  // at /proc/stat
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          std::istringstream linestream(value);
          linestream >> procsRunning;
          return procsRunning;
        }
      }
    }
  }
  return 0;  // else returns 0
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  /*
  in this function the input pid is used in the path /proc/$pid$/cmdline
  and the function returns the first word
  */
  string line, name, command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    if (getline(stream, line)) {
      std::istringstream linestream(line);
      if (linestream >> command) {
        return command;
      }
    }
  }
  return "";
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  /*
  the pid value is again used for the path /proc/$pid$/status
  the function finds the value VmRSS: and then returns the next value after
  dividing it by 1000 (conversion from kb to mb);
  */
  string line, key;
  int value;  // to save the value of the memory as an int before converting it
              // into a string

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmRSS:") {
          return to_string(
              value /
              1000);  // value is in kb so it has to be converted into mb
        }
      }
    }
  }
  return "";
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  /*
  at /proc/$pid$/status the user id is looked for which comes after the word
  Uid: it is then returned
  */
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return "";
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  /*
  after having the user id which is calculated in the first line
  the function goes to /etc/passwd the function compares the userID in the path
  with the one it is looking for after the replacement of all : by empty space
  the first word is the name of the user and the 3rd is the user id if there is
  a match it returns the user id
  */
  string userIDToLook = LinuxParser::Uid(pid);
  string line;
  string userID, charX, userName;

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> userName >> charX >> userID) {
        if (userID == userIDToLook) {
          return userName;
        }
      }
    }
  }
  return "";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  /*
  the 21th word at proc/$pid$/stat is the time when the pid was started given in
  jiffies so at proc/$pid$/stat the uptime found is then divided by system clock
  to convert the jiffies into seconds then it has to be subtracted to the total
  amount of time and the final result is then returned
  */
  int i = 0;
  long upTime;
  string line, temp{};
  std::ifstream stream(kProcDirectory + to_string(pid) +
                       kStatFilename);  // at proc/$pid$/stat
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> temp && i < 22) {
      if (i == 21) {
        std::istringstream(temp) >> upTime;
        return LinuxParser::UpTime() - (upTime / sysconf(_SC_CLK_TCK));
        ;
      }
      // Some of the pid have more than one word with a space between in the
      // "second position" and this is a safety to make sure i there is 2 for
      // the (state) position
      if (temp == "R" || temp == "S" || temp == "D" || temp == "Z" ||
          temp == "T" || temp == "t" || temp == "W" || temp == "X" ||
          temp == "x" || temp == "K" || temp == "w") {
        i = 2;
      }
      i++;
    }
  }
  return 0;  // return 0 if it isn't able to read
}

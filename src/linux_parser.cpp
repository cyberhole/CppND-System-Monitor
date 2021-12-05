#include <dirent.h>
#include <unistd.h>
#include <sstream>
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

float LinuxParser::MemoryUtilization() { 

// TODO: Read and return the system memory utilization
 int memTotal, memAvailable;
  float memUtilization;
  string line,temp,line_;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  // the 1st and 3rd line of the meminfo are going to be needed

    if (stream.is_open()) {


    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> temp >> memTotal;
    std::cout<<memTotal;
    std::getline(stream, line);
    std::getline(stream, line);
    std::istringstream linestream_(line);
    linestream_ >> temp >> memAvailable;

    
  }
  memUtilization=(((memTotal-memAvailable)*100.00)/memTotal)/100;

  return memUtilization;
   }


long LinuxParser::UpTime() { 
  /* 
  Function returns uptime of the system.
  at /proc/uptime there are two values. 
  "The first number is the total number of seconds the system has been up. 
  The second number is how much of that time the machine has spent idle, in seconds." - google
  */
  // Creation of two values one for the up time and the other for the idle time(maybe will not be needed)
  long upTime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename); // get the two values
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>upTime>>idleTime;  //save the values first at upTime and the second at idleTime
  }
  return upTime; //return the uptime
  }

long LinuxParser::Jiffies() { 
/*
returns the total number of jiffies
gets the second string object at the 
*/

    string cpu, line;
    int jiffies; 

    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>cpu>>jiffies;
    }


return jiffies;}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename); // get the two values
  //float 13th line;
  return 0; }




// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 

  return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  return {}; }






int LinuxParser::TotalProcesses() { 
  return LinuxParser::Pids().size(); }

int LinuxParser::RunningProcesses() { 
/*
gets the number of running processes from /proc/stat
it looks for key procs_running and gets the value that is written after it.
*/
  int procsRunning=0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          std::istringstream linestream(value);
          linestream>>procsRunning;
          return procsRunning;
        }
      }
    }
  }
  return procsRunning;
   }




string LinuxParser::Command(int pid) { 
  /* Read and return the command associated with a process
  */
string line,name,command;
std::ifstream stream(kProcDirectory + to_string(pid) +kCmdlineFilename);
getline(stream,line);
//std::istringstream linestream(line);
//linestream>>name>>command;
return line;
}

string LinuxParser::Ram(int pid) { 
 //Read and return the memory used by a process
  string line,key,value;
  std::ifstream stream(kProcDirectory + to_string(pid) +kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmRSS:") {
          return value;
        }
      }
    }
  }
  return value;
}
string LinuxParser::Uid(int pid) { 
  // Read and return the user ID associated with a process
  string line,key,value;
  std::ifstream stream(kProcDirectory + to_string(pid) +kStatusFilename);

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
  return value;
}
string LinuxParser::User(int pid) { 
  // Read and return the user associated with a process
  string userIDToLook=LinuxParser::Uid(pid);
  string line;
  string userID,charX,userName;


std::ifstream stream(kPasswordPath);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> userName >> charX>>userID) {
        if (userID == userIDToLook) {

          return userName;
        }
      }
    }
  }
  return userName;
return string(); }
long LinuxParser::UpTime(int pid) { 
  // Read and return the uptime of a process
  int i=0;
  long upTime;
  string line,temp="";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
  while (linestream>>temp && i<=13 )
  {
    i++;
  }
  }
  std::istringstream(temp)>>upTime;
  return upTime; }

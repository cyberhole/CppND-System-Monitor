#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <vector>

#include "linux_parser.h"

using std::vector;

class Processor {
 public:
  float Utilization();
  static int Frequency();  // returns the frequency of the cpu
  long int getJiffies() const;
  // constructor;
  Processor() { Frequency(); };

 private:
  int frequency_ = Frequency();  // frequency of cpu
  vector<long int> upTime{};     // save old amount of time
  vector<long int> jiffies{};    // save old number of jiffies
  long int jiffies_{0};
};

#endif
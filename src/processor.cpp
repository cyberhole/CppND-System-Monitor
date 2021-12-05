#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float jiffOverTime=LinuxParser::Jiffies()/LinuxParser::UpTime();
    return jiffOverTime/100; }
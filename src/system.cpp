#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "format.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() {
  //  Return the system's CPU
  return cpu_;
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  Process tem(0);
  processes_.clear();
  vector<int> v2 = LinuxParser::Pids();
  for (int i : v2) {
   if(LinuxParser::ProcessTime(i) > 0.0001f )             // don't show processes with 0 Ram
    processes_.emplace_back(i);
  }
  
for(unsigned int i = 0; i < processes_.size() - 1; i++) {       			//sort by higher cpu% usage
        for(unsigned int j = 0; j < processes_.size() - 1; j ++){
            if(processes_[j].CpuUtilization() < processes_[j+1].CpuUtilization()){
                    tem = processes_[j+1];
                    processes_[j+1] = processes_[j];
                    processes_[j] = tem;
                }
        }
 }
  return processes_;
 
}

  // Return the system's kernel identifier
std::string System::Kernel() {
  return LinuxParser::Kernel();
}

// Return the system's memory utilization
float System::MemoryUtilization() {
   return LinuxParser::MemoryUtilization();
}

 // Return the operating system name
std::string System::OperatingSystem() {
  return LinuxParser::OperatingSystem();
}

  // Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

 // Return the total number of processes on the system
int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

  // Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime();
}
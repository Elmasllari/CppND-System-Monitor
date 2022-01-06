#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

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

float LinuxParser::MemoryUtilization(){
    std::ifstream filesteam(kProcDirectory + kMeminfoFilename);
   string key , line,value ;
   float memtotal, memfree,buffers;     //memavilable,
    if(filesteam.is_open()) { 
    	while (std::getline(filesteam, line)) { 
        std::istringstream lineStream(line);
        while(lineStream >> key >> value){ 
            if(key == "MemTotal:") 		   memtotal = stof(value);
            else if(key == "MemFree:")     memfree =stof(value);
           // else if(key == "MemAvilable:") memavilable = stof(value);
            else if(key == "Buffers:")     buffers = stof(value);             
            }
        }
      }
        return ((memtotal - memfree + buffers ) / memtotal );  
}

long LinuxParser::UpTime() {
  long uptime;
  string idle;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  return uptime;
}


// for Process::CpuUtilization()
float LinuxParser::ProcessTime(int pid) {
  string line, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  float  utime, stime, cutime, cstime, total_time , seconds;
  vector<string> values;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  //get all values needed
  	
    utime = stof( values[13]);
   stime = stof( values[14]);
   cutime = stof( values[15]);
    cstime = stof( values[16]);
  
  
  //apply formula
  total_time = utime + stime;
  total_time = total_time + cutime + cstime;

  seconds =  UpTime(pid) ;
  

float f =  (total_time/100  / seconds);

    return f; 
}

//for processor
vector<string> LinuxParser::CpuUtilization() {
  string line, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  vector<string> tem;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      tem.emplace_back(value);
    };
  }
  return tem;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes")   return value;   
      }
    }
  } 
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running")  return value;       
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid){
  string line,  key, value;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if(filestream.is_open()) {
         while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
  key = key + value + " ";
      }
     }       
    }
      return key; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  int mb;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmRSS") {                        // I have used VmRSS instead of VmSize as per Udacity 													guideline: https://man7.org/linux/man-pages/man5/proc.5.html
          mb = stoi(value);
          mb=mb/1024;  //kb to mb
          value = to_string(mb);
          return value;
      }
    }
  }
}
  return "0";
}
  
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") 
          return value;
      }
    }
  }
  return 0;
}

string LinuxParser::User(int pid) {
  string line;
  string key, value, mb;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> key>> mb) {
        if (mb == LinuxParser::Uid(pid)) 
          return value;
      }
    }
  }
  return 0;
 }

// Read and return the uptime of a process
long int LinuxParser::UpTime(int PID) 
{ 
  string line, value;
  vector<string> tem;
  long mb;
  std::ifstream stream(kProcDirectory + to_string(PID) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::stringstream linestream(line);
    while (linestream >> value)
    {
     tem.push_back(value);
    }
    mb = stol(tem[21]);
  }
  return UpTime() - mb/sysconf(_SC_CLK_TCK);

  
}
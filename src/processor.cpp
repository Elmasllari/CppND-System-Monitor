#include "processor.h"
#include "linux_parser.h"

// Returning the aggregate CPU utilization
float Processor::Utilization() {

  v = LinuxParser::CpuUtilization();
  //get values from vector   // v[0] = cpu
 user = stof(v[1]);
 nice = stof(v[2]);
system = stof(v[3]);
 idle = stof(v[4]);
 iowait = stof(v[5]);
 irq= stof(v[6]);
 softirq = stof(v[7]);
 steal = stof(v[8]);
 guest = stof(v[9]);
 guest_nice = stof(v[10]);
  
  //apply formula
  float PrevIdle,Idle,PrevNonIdle,NonIdle,PrevTotal,Total,totald, idled,cpu;
  PrevIdle = pidle + piowait;
  Idle = idle + iowait;
  PrevNonIdle = puser + pnice + psystem + pirq + psoftirq + psteal;
  NonIdle =  user + nice + system + irq + softirq + steal;
  PrevTotal = PrevIdle + PrevNonIdle;
  Total = Idle + NonIdle;
  totald = Total - PrevTotal;
  idled = Idle - PrevIdle;
  cpu = ((totald  - idled)/totald);
  
  
  
  
  
  //set current values to previous for next call of function
  puser = user;
  pnice = nice;
  psystem = system;
  pidle = idle;
  piowait = iowait;
  pirq = irq;
  psoftirq = softirq;
  psteal = steal;
  pguest = guest;
  pguest_nice = guest_nice;
  
 

  return cpu;
  
}


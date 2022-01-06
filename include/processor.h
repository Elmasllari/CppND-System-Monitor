#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization(); 


 private:
  // p for previous
 long puser=0, pnice=0, psystem=0, pidle=0, piowait=0, pirq=0, psoftirq=0, psteal=0, pguest=0, pguest_nice=0;
  long user=0, nice=0, system=0, idle=0, iowait=0, irq=0, softirq=0, steal=0, guest=0, guest_nice=0;
  std::vector<std::string> v;
};

#endif
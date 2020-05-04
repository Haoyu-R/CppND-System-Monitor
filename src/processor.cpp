#include "processor.h"
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>

using std::string;

// TODO: Return the aggregate CPU utilization
void calculateCPU(float &Idle, float &Total) {
  string line;
  string key;

  float user, nice, system, idle, iowait, irq, softirq, steal;

  std::ifstream fileStream("/proc/stat");

  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> key;
      if (key == "cpu") {
        lineStream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        Idle = idle + iowait;
        float NonIdle = user + nice + system + irq + softirq + steal;
        Total = Idle + NonIdle;
        break;
      }
    }
  }
}

float Processor::Utilization() {
  float idle, total, totald, idled;
  //calculateCPU(prevIdle, prevTotal);
  //std::this_thread::sleep_for(std::chrono::milliseconds(5000000));
  calculateCPU(idle, total);
 
  totald = total - prevTotal;
  idled = idle - prevIdle;

  prevIdle = idle;
  prevTotal = total;

  return (totald - idled) / totald;
}
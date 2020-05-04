#include <dirent.h>
#include <unistd.h>
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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
    string line;
    string key;
    float value = 0;
    float memTotal = 1;
    float memFree = 0;

    std::ifstream fileStream(kProcDirectory + kMeminfoFilename);
    if (fileStream.is_open()) {
      while (std::getline(fileStream, line)) {
        std::istringstream lineStream(line);
        lineStream >> key >> value;
        if (key == "MemTotal:") {
          memTotal = std::stof(value);
        } else if (key == "MemFree:") {
          memFree = std::stof(value);
          return (memTotal - memFree) / memTotal;
        }
      }
    }
    return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
    string line;
    long time;

    std::ifstream fileStream(kProcDirectory + kUptimeFilename);
    if (fileStream.is_open()) {
      std::getline(fileStream, line);
      std::istringstream lineStream(line);
      lineStream >> time;
      return std::stol(time);
    }
    
    return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
    return 0; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    string line;
    string key;
    int value;

    std::ifstream fileStream(kProcDirectory + kStatFilename);
    if (fileStream.is_open()) {
      while (std::getline(fileStream, line)) {
        std::istringstream lineStream(line);
        lineStream >> key;
        if (key == "processes") {
        lineStream >> std::stoi(value);
          return value;
        }
      }
    }

    return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
  string line;
  string key;
  int value;

  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> key;
      if (key == "procs_running") {
        lineStream >> std::stoi(value);
        return value;
      }
    }
  }
    return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);

  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      return line;
    }
  }
    
    return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  float kb;

  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);

  if (fileStream.is_open()) {
    while (std::(fileStream, line)) {
      std::istringstream lineStream(line);
      lineStream >> key;
      if (key == "VmSize:") {
        lineStream >> std::stoi(kb);
        return std::to_string(kb/1000);
      }
    }
  }
    return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
    string line;
    string key;
    string uid;

    std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                             kStatusFilename);

    if (fileStream.is_open()) {
      while (std::getline(fileStream, line)) {
        std::istringstream lineStream(line);
        lineStream >> key;
        if (key == "uid:") {
          lineStream >> uid;
          return uid;
        }
      }
    }
    return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(string uid) { 
  string line;
  string x;
  string key;
  string uid_;

  std::ifstream fileStream(kPasswordPath);

  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);

        lineStream >> key >> x >> uid_;
        if (uid_ == uid) {   
            return key;
        }
      
    }
  }
    return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string key;
  long t;

  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);

  if (fileStream.is_open()) {
    std::getline(fileStream, line);

    std::istringstream lineStream(line);
    for (int i = 0; i < 21; i++) {
      lineStream >> key;
    }
      lineStream >> t;

      return std::stol(t) / sysconf(_SC_CLK_TCK);
  
  }
    
    return 0; }

float LinuxParser::CpuUtilization(int pid) { 
  string line;
  string key;
  float uptime, utime, stime, cutime, cstime, starttime;

  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);

  if (fileStream.is_open()) {
    std::getline(fileStream, line);

    std::istringstream lineStream(line);
    for (int i = 0; i < 22; i++) {
      if (i == 0) {
        lineStream >> uptime;
        continue;
      } else if (i == 13) {
        lineStream >> utime;
        continue;
      } else if (i == 14) {
        lineStream >> stime;
        continue;
      } else if (i == 15) {
        lineStream >> cutime;
        continue;
      } else if (i == 16) {
        lineStream >> cstime;
        continue;
      } else if (i == 21) {
        lineStream >> starttime;
        break;
      }
      lineStream >> key;
    }

    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));

    return 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
  }
    
    return 0; }

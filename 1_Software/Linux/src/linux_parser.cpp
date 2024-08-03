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
  std::ifstream file("/System/Library/CoreServices/SystemVersion.plist");
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open SystemVersion.plist");
  }

  std::string line;
  std::string osName;
  std::string version;
  while (std::getline(file, line)) {
    if (line.find("<key>ProductName</key>") != std::string::npos) {
      // Get the next line which contains the OS name
      std::getline(file, line);
      std::size_t start = line.find("<string>") + 8;
      std::size_t end = line.find("</string>");
      osName = line.substr(start, end - start);
    } else if (line.find("<key>ProductVersion</key>") != std::string::npos) {
      // Get the next line which contains the version number
      std::getline(file, line);
      std::size_t start = line.find("<string>") + 8;
      std::size_t end = line.find("</string>");
      version = line.substr(start, end - start);
    }
    if (!osName.empty() && !version.empty()) {
      break;
    }
  }
  file.close();

  // Extract major and minor version numbers
  std::string major, minor;
  std::size_t dotPos = version.find('.');
  if (dotPos != std::string::npos) {
    major = version.substr(0, dotPos);
    minor = version.substr(dotPos + 1,
                           version.find('.', dotPos + 1) - (dotPos + 1));
  }

  // Format the output as macOS-X.Y
  return osName + "-" + major + "." + minor;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  std::cout << "Abdelrahman" << kernel << "/n";
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
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }

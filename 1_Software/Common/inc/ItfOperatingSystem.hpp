#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H
/***************************************************************************
 * Component: OperatingSystem.hpp
 *
 * Description: This component provides interfaces for interacting with
 * operating system - Abstract Class.
 *
 * Author: Abdelrahman Hossam
 *
 * Date: August 1st 2024
 *
 **************************************************************************/

/**************************************************
********************* Includes ********************
***************************************************/
#include <string>

/**************************************************
********************** Class **********************
***************************************************/
class ItfOperatingSystem {
 public:
  // System
  // Return the operating system name and version
  virtual std::string OperatingSystem() = 0;

  // Read and Return the system's kernel identifier (string)
  virtual std::string Kernel() = 0;

  // Read and Return the system's memory utilization
  virtual float MemoryUtilization() = 0;

  // Measure and Return the number of seconds since the system started running
  virtual long UpTime() = 0;

  // Get and Return the total number of processes on the system
  virtual int TotalProcesses() = 0;

  // Get and Return the number of processes actively running on the system
  virtual int RunningProcesses() = 0;

  // Get and Return the number of processes actively running on the system
  virtual std::vector<int> Pids() = 0;

  // Measure and Return CPU utilization
  virtual std::vector<std::string> CpuUtilization() = 0;

  // Processes
  // Read and return the command associated with a process
  virtual std::string Command(int pid) = 0;

  // Read and Return memory usage associated with a process
  virtual std::string Ram(int pid) = 0;

  // Read and return the user ID associated with a process
  virtual std::string Uid(int pid) = 0;

  // Read and return the user associated with a process
  virtual std::string User(int pid) = 0;

  // Read and return the uptime associated with a process
  virtual long int UpTime(int pid) = 0;
};

#endif
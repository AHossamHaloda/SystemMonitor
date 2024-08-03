#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H
/**************************************************
********************* Includes ********************
***************************************************/
#include <string>

#include "../../Common/inc/ItfOperatingSystem.hpp"

/**************************************************
********************** Class **********************
***************************************************/
class MacOs_parser : public ItfOperatingSystem {
 public:
  // Static method to obtain the singleton instance
  static MacOs_parser& getInstance() {
    // Static local variable to hold the single instance of MacOs_parser
    static MacOs_parser MacOs_parserSingleton;

    // Return a reference to the single instance of MacOs_parser
    return MacOs_parserSingleton;
  }
  // System
  // Return OS name and version
  std::string OperatingSystem() override;

  // Return kernal name and version
  std::string Kernel() override;

  // Return the system memory utilization
  float MemoryUtilization() override;

  // Return the system uptime
  long UpTime() override;

  // Return the running processes Ids
  std::vector<int> Pids() override;

  // Return the number of processes actively running on the system
  int TotalProcesses() override;

  // Return the number of processes actively running on the system
  int RunningProcesses() override;

  // Return CPU utilization
  std::vector<std::string> CpuUtilization() override;

  // Processes
  // Read and return the command associated with a process
  std::string Command(int pid) override;

  // Read and Return memory usage associated with a process
  std::string Ram(int pid) override;

  // Read and return the uptime associated with a process
  long int UpTime(int pid) override;

  // Read and return the user ID associated with a process
  std::string Uid(int pid) override;

  // Read and return the user associated with a process
  std::string User(int pid) override;

 private:
  // Private constructor definition
  MacOs_parser(){};
  // Delete copy constructor and assignment operator to ensure singleton
  // property.
  MacOs_parser(MacOs_parser const&) = delete;
  void operator=(MacOs_parser const&) = delete;
};

#endif
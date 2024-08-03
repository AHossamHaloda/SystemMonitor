#ifndef SYSTEM_H
#define SYSTEM_H
/**************************************************
********************* Includes ********************
***************************************************/
#include <string>
#include <vector>

#include "macOs_parser.h"
#include "process.h"
#include "processor.h"

/**************************************************
********************** Class **********************
***************************************************/
class System {
 public:
  /**
   * @brief Constructor that initializes the System with the MacOs_parser
   * singleton instance.
   *
   * The `MacOs_parser`
   * instance is used to provide operating system-specific functionality and
   * data for the `System` class.
   *
   */
  System() { m_itfParser = &MacOs_parser::getInstance(); }

  /**
   * @brief Get a reference to the Processor object.
   * @return Reference to the Processor object.
   */
  Processor& Cpu();

  /**
   * @brief Get a reference to the vector of Process objects.
   * @return Reference to the vector of Process objects.
   */
  std::vector<Process>& Processes();

  /**
   * @brief Calculate and return the system's memory utilization.
   * @return Float value representing the memory utilization.
   */
  float MemoryUtilization();

  /**
   * @brief Get the system uptime in seconds.
   * @return Long value representing the system uptime.
   */
  long UpTime();

  /**
   * @brief Get the total number of processes.
   * @return Integer value representing the total number of processes.
   */
  int TotalProcesses();

  /**
   * @brief Get the number of running processes.
   * @return Integer value representing the number of running processes.
   */
  int RunningProcesses();

  /**
   * @brief Get the kernel version of the operating system.
   * @return String representing the kernel version.
   */
  std::string Kernel();

  /**
   * @brief Get the name of the operating system.
   * @return String representing the operating system name.
   */
  std::string OperatingSystem();

 private:  // Private members
  // Processor object to manage CPU-related operations
  Processor cpu_;
  // Vector to store and manage a list of Process objects
  std::vector<Process> processes_ = {};
  // MacOs_parser object to handle macOS-specific parsing operations
  ItfOperatingSystem* m_itfParser;
};

#endif  // File Guard
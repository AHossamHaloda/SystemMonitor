#ifndef PROCESS_H
#define PROCESS_H
/**************************************************
********************* Includes ********************
***************************************************/
#include <string>

#include "macOs_parser.h"

/**************************************************
********************** Class **********************
***************************************************/
class Process {
 public:
  /**
   * @brief Constructs a `Process` object with the specified process ID.
   * @param pid The process ID of the process to initialize.
   */
  Process(int pid);

  /**
   * @brief Returns the process ID of this process.
   * @return The process ID.
   */
  int Pid() const;

  /**
   * @brief Returns the user who generated this process.
   * @return The username associated with this process.
   */
  std::string User() const;

  /**
   * @brief Returns the command that generated this process.
   * @return The command string associated with this process.
   */
  std::string Command() const;

  /**
   * @brief Returns the CPU utilization of this process.
   * @return The CPU utilization as a floating-point value.
   */
  float CpuUtilization() const;

  /**
   * @brief Returns the memory utilization of this process.
   * @return The memory usage as a string.
   */
  std::string Ram() const;

  /**
   * @brief Returns the age of this process in seconds.
   * @return The uptime of the process in seconds.
   */
  long int UpTime() const;

  /**
   * @brief Compares this process with another process for ordering.
   * @param a The process to compare against.
   * @return `true` if this process should be ordered before `a`, `false`
   * otherwise.
   */
  bool operator<(Process const& a) const;

 private:  // private methodes
  /**
   * @brief Updates the CPU utilization of this process and saves it in
   * `m_fCpuUtilization`.
   */
  void vidUpdateCpuUsage();

  /**
   * @brief Updates the user who generated this process and saves it in
   * `m_strUser`.
   */
  void vidUpdateUser();

  /**
   * @brief Updates the command that generated this process and saves it in
   * `m_strCommand`.
   */
  void vidUpdateCommand();

  /**
   * @brief Updates the memory utilization of this process and saves it in
   * `m_strRam`.
   */
  void vidUpdateRam();

  /**
   * @brief Updates the age of this process and saves it in
   * `m_u64Uptime`.
   */
  void vidUpdateUptime();

 private:  // private members
  // MacOs_parser object to handle macOS-specific parsing operations
  ItfOperatingSystem* m_itfParser;

  // The process ID (PID) of the current process.
  int m_u32Pid;

  // The command that generated the process.
  std::string m_strCommand;

  // The memory utilization of the process.
  std::string m_strRam;

  // The user who generated the process.
  std::string m_strUser;

  // The age of the process in seconds.
  long int m_u64Uptime;

  // The CPU utilization of the process as a percentage.
  float m_fCpu_utilization;
};

#endif  // File Guard

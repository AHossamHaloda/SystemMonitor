/**************************************************
********************* Includes ********************
***************************************************/
#include "process.h"

#include <libproc.h>     // Include for process-related functions on macOS
#include <pwd.h>         // Include for user account information functions
#include <sys/sysctl.h>  // Include for system control functions

#include <iostream>
#include <string>

using std::string;
using std::to_string;

// Constructs a `Process` object with the specified process ID.
/*********************************************************************************************/
Process::Process(int pid) : m_u32Pid(pid) {
  // Get the singleton instance of MacOs_parser and assign it to m_itfParser
  m_itfParser = &MacOs_parser::getInstance();
  // Updates all values associated with the process
  vidUpdateCpuUsage();
  vidUpdateCommand();
  vidUpdateRam();
  vidUpdateUptime();
  vidUpdateUser();
}

// Return this process's ID
/*********************************************************************************************/
int Process::Pid() const { return m_u32Pid; }

// Return this process's CPU utilization
/*********************************************************************************************/
float Process::CpuUtilization() const { return m_fCpu_utilization; }

// Return the command that generated this process
/*********************************************************************************************/
string Process::Command() const { return m_strCommand; }

// Return this process's memory utilization
/*********************************************************************************************/
string Process::Ram() const { return m_strRam; }

// Return the user (name) that generated this process
/*********************************************************************************************/
string Process::User() const { return m_strUser; }

// Return the age of this process (in seconds)
/*********************************************************************************************/
long int Process::UpTime() const { return m_u64Uptime; }

// Overload the "less than" comparison operator for Process objects
/*********************************************************************************************/
bool Process::operator<(Process const& a) const {
  // Compare the CPU utilization for sorting purposes
  return this->CpuUtilization() > a.CpuUtilization();
}

// Updates the CPU utilization of this process and saves it in
// `m_fCpuUtilization`.
/*********************************************************************************************/
void Process::vidUpdateCpuUsage() {
  bool success = true;  // Flag to check if operations are successful

  float fTotalTime{0.0};     // Total CPU time spent by the process (in seconds)
  long fStartTime{0};        // Process start time (in seconds since epoch)
  long u64CurrentTime{0};    // Current time (in seconds since epoch)
  long u64Uptime{0};         // System uptime (in seconds)
  long u64ProcessUptime{0};  // Process uptime (in seconds)

  struct proc_taskinfo taskInfo;  // Structure to hold task information
  struct proc_taskallinfo
      allInfo;              // Structure to hold all task-related information
  struct timeval boottime;  // Structure to hold system boot time

  // Initialize all structures to zero
  memset(&taskInfo, 0, sizeof(taskInfo));
  memset(&allInfo, 0, sizeof(allInfo));
  memset(&boottime, 0, sizeof(boottime));

  // Get process information using proc_pidinfo
  if (proc_pidinfo(m_u32Pid, PROC_PIDTASKINFO, 0, &taskInfo,
                   sizeof(taskInfo)) <= 0) {
    // Optional: Print an error message for debugging purposes
    // std::cerr << "Failed to get task info for PID: " << m_u32Pid << " with
    // error: " << strerror(errno) << std::endl;
    success = false;
  }

  // Calculate total CPU time spent by the process
  if (success) {
    // Convert CPU time from microseconds to seconds and sum user and system
    // times
    fTotalTime = taskInfo.pti_total_user / 1000000.0 +
                 taskInfo.pti_total_system / 1000000.0;
  }

  // Get process start time and system uptime
  if (success && proc_pidinfo(m_u32Pid, PROC_PIDTASKALLINFO, 0, &allInfo,
                              sizeof(allInfo)) <= 0) {
    // Optional: Print an error message for debugging purposes
    // std::cerr << "Failed to get all info for PID: " << m_u32Pid << " with
    // error: " << strerror(errno) << std::endl;
    success = false;
  }

  fStartTime = success ? allInfo.pbsd.pbi_start_tvsec
                       : 0;  // Process start time in seconds

  // Calculate system uptime
  if (success) {
    size_t size = sizeof(boottime);
    if (sysctlbyname("kern.boottime", &boottime, &size, nullptr, 0) != 0) {
      // Optional: Print an error message for debugging purposes
      // std::cerr << "Failed to get system uptime with error: " <<
      // strerror(errno) << std::endl;
      success = false;
    }
  }

  u64CurrentTime = time(nullptr);  // Current time in seconds
  u64Uptime = success ? (u64CurrentTime - boottime.tv_sec)
                      : 0;  // Calculate system uptime
  u64ProcessUptime =
      success ? (u64CurrentTime - fStartTime) : 0;  // Calculate process uptime

  // Calculate process uptime and CPU usage
  if (success && u64ProcessUptime > 0) {
    m_fCpu_utilization =
        (fTotalTime / u64ProcessUptime);  // Calculate CPU utilization
  } else {
    m_fCpu_utilization =
        0;  // Set CPU utilization to 0 if not successful or uptime is 0
  }

  // Optional: Print debug information
  // std::cout << "Formatted CPU Utilization: " << m_fCpu_utilization << "%" <<
  // std::endl;
}

// Updates the user who generated this process and saves it in `m_strUser`.
/*********************************************************************************************/
void Process::vidUpdateUser() { m_strUser = m_itfParser->User(m_u32Pid); }

// Updates the command that generated this process and saves it in
// `m_strCommand`.
/*********************************************************************************************/
void Process::vidUpdateCommand() {
  m_strCommand = m_itfParser->Command(m_u32Pid);
}

// Updates the memory utilization of this process and saves it in `m_strRam`.
/*********************************************************************************************/
void Process::vidUpdateRam() { m_strRam = m_itfParser->Ram(m_u32Pid); }

// Updates the age of this process and saves it in `m_u64Uptime`.
/*********************************************************************************************/
void Process::vidUpdateUptime() { m_u64Uptime = m_itfParser->UpTime(m_u32Pid); }
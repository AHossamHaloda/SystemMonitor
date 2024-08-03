
/**************************************************
********************* Includes ********************
***************************************************/
#include "system.h"

#include <string>
#include <vector>

#include "macOs_parser.h"
#include "process.h"
#include "processor.h"

// Return the system's CPU
/*********************************************************************************************/
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
/*********************************************************************************************/
std::vector<Process>& System::Processes() {
  // Clear the existing list of processes
  processes_.clear();

  // Fetch process IDs from the parser
  std::vector<int> processIds = m_itfParser->Pids();

  // Create a vector to hold process objects
  std::vector<Process> newProcesses;

  // Populate the vector with Process objects
  for (int pid : processIds) {
    newProcesses.emplace_back(pid);
  }

  // Sort processes based on CPU utilization in descending order
  // No sorting fuction is provided to sort so, < operator is used by default.
  std::sort(newProcesses.begin(), newProcesses.end());

  // Update the member variable with the sorted processes
  processes_ = std::move(newProcesses);

  // Return a reference to the updated list of processes
  return processes_;
}

// Return the system's kernel identifier (string)
/*********************************************************************************************/
std::string System::Kernel() { return m_itfParser->Kernel(); }

// Return the system's memory utilization
/*********************************************************************************************/
float System::MemoryUtilization() { return m_itfParser->MemoryUtilization(); }

// Return the operating system name
/*********************************************************************************************/
std::string System::OperatingSystem() { return m_itfParser->OperatingSystem(); }

// Return the number of processes actively running on the system
/*********************************************************************************************/
int System::RunningProcesses() { return m_itfParser->RunningProcesses(); }

// Return the total number of processes on the system
/*********************************************************************************************/
int System::TotalProcesses() { return m_itfParser->TotalProcesses(); }

// Return the number of seconds since the system started running
/*********************************************************************************************/
long int System::UpTime() { return m_itfParser->UpTime(); }
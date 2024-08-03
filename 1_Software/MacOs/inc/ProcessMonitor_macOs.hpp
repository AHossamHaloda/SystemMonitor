#ifndef PROCESS_MONITOR_MAC_OS_H
#define PROCESS_MONITOR_MAC_OS_H
/*
 * Component: ProcessMonitor_macOs.hpp
 *
 * Description: This component control the process monitoring for MacOs platform
 * - Concrete Class.
 *
 * Author: Abdelrahman Hossam
 *
 * Date: August 1st 2024
 *
 */

/**************************************************
********************* Includes *******************
***************************************************/
#include <iostream>
#include <string>

#include "macOs_parser.h"
#include "system.h"

/**************************************************
********************** Class **********************
***************************************************/
class ProcessMonitorMacOs {
 public:
  // ProcessMonitorMacOs constructor
  ProcessMonitorMacOs() { std::cout << "ProcessMonitorMacOs Constructor\n"; }

  // ProcessMonitorMacOs destructor
  ~ProcessMonitorMacOs() { std::cout << "ProcessMonitorMacOs Destructor"; }

  // Run Sytem Monitor
  void systemRun();

 private:
  System m_system;
};

#endif
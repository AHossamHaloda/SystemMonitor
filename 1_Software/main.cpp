
#include <iostream>

#ifdef LINUX_PLT
#include "Linux/inc/ProcessMonitor_linux.hpp"
#elif MAC_OS_PLT
#include "MacOs/inc/ProcessMonitor_macOs.hpp"
#endif

int main() {
#ifdef LINUX_PLT
  /* Create SystemMonitor Objects */
  ProcessMonitorLinux processMonitorLinux;
  /* Run System Monitor App */
  processMonitorLinux.systemRun();

#elif MAC_OS_PLT

  /* Create SystemMonitor Objects */
  ProcessMonitorMacOs processMonitorMacOs;
  /* Run System Monitor App */
  processMonitorMacOs.systemRun();

#endif
}
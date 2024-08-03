
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "Common/inc/system.h"
#include "Common/inc/format.h"

#ifdef LINUX_PLT
#include "Linux/inc/ProcessMonitor_linux.hpp"
#elif MAC_OS_PLT
#include "MacOs/inc/ProcessMonitor_macOs.hpp"
#endif

int main() {
  /********** Testing **********/
  /* System Objects */
  System system;
  Processor processor;
  ItfOperatingSystem* parser = &MacOs_parser::getInstance();

  /* System Variables */
  /* static */ float MemoryUtil;
  /* static */ long upTime;
  /* static */ int totNumOfProcesses;
  /* static */ int totNumOfRunnProcesses;
  /* static */ std::vector<int> pids;
  /* static */ std::string s;

  std::vector<std::string> Cpu;
  float fUtlization{0.0};
  long longValue{0};
  std::string strRam{};
  std::string strUser{};

  while (0) {
    fUtlization = processor.Utilization();
    // float percentage = fUtlization * 100;  // Convert to percentage
    // std::cout << "CPU Utilization: " << percentage << "%\r\n";
    std::cout << "fUtlization = " << fUtlization << "\n\n";
    // std::cout << "----------------------------\n\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  while (0) {
    int number = system.TotalProcesses();

    std::cout << "int TProcesses = " << number << "\n\n";
    // number.c_str()
    std::string tsting = std::to_string(system.TotalProcesses()).c_str();
    std::cout << "tt Processes = " << tsting << "\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  while (0) {
    system.Processes();

    std::cout << "----------------------------\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    break;
  }

  while (0) {
    // MemoryUtil = parser.MemoryUtilization();
    // upTime = parser.UpTime();
    // s = Format::ElapsedTime(upTime);
    pids = parser->Pids();
    // totNumOfProcesses = system.TotalProcesses();
    // totNumOfRunnProcesses = system.RunningProcesses();

    // Cpu = parser.CpuUtilization();

    // std::cout << "MemoryUtil = " << MemoryUtil << "\n\n";
    // std::cout << "upTime = " << upTime << "\n\n";
    // std::cout << "upTime = " << s << "\n\n";

    // std::cout << "main tProcesses = " << system.TotalProcesses() << "\n\n";
    // std::cout << "totNumOfRunnProcesses = " << totNumOfRunnProcesses<<"\n\n";

    for (auto pid : pids) {
      strRam = parser->Ram(pid);
      strUser = parser->Uid(pid);
      std::cout << " pid: " << pid << " Ram  = " << strRam
                << " uId: " << strUser << "\r\n";
    }

    // for (auto cpu : Cpu) {
    //   std::cout << "cpu = " << cpu << " \n\n";
    // }

    std::cout << "----------------------------\r\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
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
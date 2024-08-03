
/**************************************************
********************* Includes ********************
***************************************************/
#include "macOs_parser.h"

#include <libproc.h>  // Include for process-related functions on macOS
#include <mach/mach.h>  // Include for accessing system calls and types related to CPU utilization
#include <pwd.h>  // Include for user account information functions
#include <sys/proc_info.h>
#include <sys/sysctl.h>  // Provides sysctl functions for retrieving system information
#include <unistd.h>  // Include for POSIX operating system API functions

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "macOs_parser_cfg.h"

using std::string;

// #define Debug

// Read and Return the operating system name
/*********************************************************************************************/
string MacOs_parser::OperatingSystem() {
  std::ifstream file(kSystemVersion_plist);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open SystemVersion.plist");
  }

  string line;
  string osName;
  string version;
  while (std::getline(file, line)) {
    if (line.find("<key>ProductName</key>") != string::npos) {
      // Get the next line which contains the OS name
      std::getline(file, line);
      std::size_t start = line.find("<string>") + 8;
      std::size_t end = line.find("</string>");
      osName = line.substr(start, end - start);
    } else if (line.find("<key>ProductVersion</key>") != string::npos) {
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
  string major, minor;
  std::size_t dotPos = version.find('.');
  if (dotPos != string::npos) {
    major = version.substr(0, dotPos);
    minor = version.substr(dotPos + 1,
                           version.find('.', dotPos + 1) - (dotPos + 1));
  }

  // Format the output as macOS-X.Y
  return osName + "-" + major + "." + minor;
}

// Read and Return the system's kernel identifier (string)
/*********************************************************************************************/
std::string MacOs_parser::Kernel() {
  char kernelVersion[256];  // Buffer to hold the kernel version
  size_t size = sizeof(kernelVersion);

  // Retrieve the kernel version using sysctl
  if (sysctlbyname("kern.version", kernelVersion, &size, nullptr, 0) != 0) {
    throw std::runtime_error("Failed to retrieve kernel version");
  }

  // Convert the buffer to std::string
  std::string kernel(kernelVersion);

  // Find the position of the first colon, which precedes the version number
  size_t pos = kernel.find(':');
  if (pos != std::string::npos) {
    kernel = kernel.substr(0, pos);  // Extract up to the colon
  }

  return kernel;
}

// Calculate and Return the system's memory utilization
/*********************************************************************************************/
float MacOs_parser::MemoryUtilization() {
  kern_return_t kr;    // Variable to store the return value of Mach calls
  vm_size_t pageSize;  // Page size in bytes
  vm_statistics_data_t vmStats;  // Structure to store virtual memory statistics
  mach_msg_type_number_t count =
      HOST_VM_INFO_COUNT;  // Number of fields in vmStats

  // Get the page size
  pageSize = sysconf(_SC_PAGESIZE);

  // Get virtual memory statistics
  kr = host_statistics(mach_host_self(), HOST_VM_INFO,
                       reinterpret_cast<host_info_t>(&vmStats), &count);
  if (kr != KERN_SUCCESS) {
    throw std::runtime_error("host_statistics() failed!");
  }

  // Calculate the total, free, and used memory
  long freePages = vmStats.free_count;
  long activePages = vmStats.active_count;
  long inactivePages = vmStats.inactive_count;
  long speculativePages = vmStats.speculative_count;
  long wiredPages = vmStats.wire_count;

  // Calculate total memory in bytes
  long memFree = freePages * pageSize;
  long memActive = activePages * pageSize;
  long memInactive = inactivePages * pageSize;
  long memSpeculative = speculativePages * pageSize;
  long memWired = wiredPages * pageSize;
  long memTotal =
      (memFree + memActive + memInactive + memSpeculative + memWired);

  if (memTotal == 0) {
    return std::numeric_limits<float>::quiet_NaN();  // Avoid division by zero
  }

  // Calculate used memory percentage
  long memUsed = memTotal - memFree;
  float usedMemoryPercentage =
      static_cast<float>(memUsed) / static_cast<float>(memTotal);

  return usedMemoryPercentage;
}

// Calculate and Return the number of seconds since the system started running
/*********************************************************************************************/
long MacOs_parser::UpTime() {
  struct timeval boottime;
  size_t size = sizeof(boottime);

  // Retrieve the system boot time using sysctl
  if (sysctlbyname("kern.boottime", &boottime, &size, nullptr, 0) != 0) {
    throw std::runtime_error("Failed to retrieve boot time");
  }

  // Get the current time
  std::time_t nowEpoch = std::time(nullptr);
  if (nowEpoch == static_cast<std::time_t>(-1)) {
    throw std::runtime_error("Failed to get current time");
  }

  // Calculate uptime in seconds
  return static_cast<long>(difftime(nowEpoch, boottime.tv_sec));
}

// Get and Return the total number of processes on the system
/*********************************************************************************************/
int MacOs_parser::TotalProcesses() {
  int processList[8192];  // Increased buffer size
  size_t listSize = sizeof(processList);

  // Get the list of process IDs
  int numBytes = proc_listpids(PROC_ALL_PIDS, 0, processList, listSize);
  if (numBytes < 0) {
    throw std::runtime_error("Failed to retrieve process list");
  }

  // Calculate the number of PIDs retrieved
  int numProcesses = numBytes / sizeof(processList[0]);

  // Count the number of valid process IDs
  int totalProcesses = 0;
  for (int i = 0; i < numProcesses; ++i) {
    if (processList[i] != 0) {
      ++totalProcesses;
    }
  }

  return totalProcesses;
}

// Get and Return the number of processes actively running on the system
/*********************************************************************************************/
int MacOs_parser::RunningProcesses() {
  std::array<char, 128> buffer;
  std::string result;
  std::string command = "ps aux | awk '$8 ~ /^R/ {count++} END {print count}'";
  int runningProcesses = -1;  // Default return value in case of failure

  // Execute the command and get its output
  std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
  if (pipe) {
    // Read the output from the command
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      result += buffer.data();
    }

    // Check if result is empty
    if (!result.empty()) {
      // Trim leading/trailing whitespace
      result.erase(result.find_last_not_of(" \n\r\t") + 1);
      result.erase(0, result.find_first_not_of(" \n\r\t"));

      // Debug: Print the trimmed command output
      // std::cout << "Trimmed command output: [" << result << "]" << std::endl;

      // Convert output to integer
      try {
        runningProcesses = std::stoi(result);
      } catch (const std::exception& e) {
        std::cerr << "Failed to convert output to integer: " << e.what()
                  << std::endl;
        runningProcesses =
            -1;  // Set default value in case of conversion failure
      }
    } else {
      std::cerr << "Command output is empty or failed to execute properly."
                << std::endl;
    }
  } else {
    std::cerr << "popen() failed!" << std::endl;
  }

  return runningProcesses;
}

// Return the running processes Ids
/*********************************************************************************************/
std::vector<int> MacOs_parser::Pids() {
  std::vector<int> pids;  // Vector to store the list of PIDs

  // Command to get the list of PIDs using the `ps` command
  string command = "ps -A -o pid=";
  std::array<char, 256> buffer;  // Buffer to store command output
  string result;                 // String to accumulate the command output

  // Open a pipe to execute the command and read its output
  std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

  // Check if the pipe was successfully opened
  if (pipe) {
    // Read the command output from the pipe
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      result += buffer.data();  // Append the output to the result string
    }

    // Create a string stream from the result to parse each line
    std::istringstream stream(result);
    string line;

    // Process each line in the command output
    while (std::getline(stream, line)) {
      // Trim trailing whitespace from the line
      line =
          string(line.begin(),
                 std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
                   return !std::isspace(ch);
                 }).base());

      // Check if the line is not empty
      if (!line.empty()) {
        try {
          // Convert the line to an integer and add it to the PIDs vector
          int pid = std::stoi(line);
          pids.push_back(pid);
        } catch (const std::invalid_argument& e) {
          std::cerr << "Failed to convert PID: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
          std::cerr << "PID out of range: " << e.what() << std::endl;
        }
      }
    }
  } else {
    std::cerr << "popen() failed!" << std::endl;
  }

  // Return the vector containing the list of PIDs
  return pids;
}

// Return CPU utilization
/*********************************************************************************************/
std::vector<string> MacOs_parser::CpuUtilization() {
  std::vector<string> utilization;
  processor_info_array_t cpu_info1, cpu_info2;
  mach_msg_type_number_t num_cpu_info1, num_cpu_info2;
  unsigned int num_cpus;
  kern_return_t kr;

  // First data sample
  kr = host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &num_cpus,
                           &cpu_info1, &num_cpu_info1);
  if (kr != KERN_SUCCESS) {
    std::cerr << "Error getting CPU info: " << kr << std::endl;
    return {};
  }

  // Sleep for a somwtime to get the next sample
  // This delay should not be const and should be handeled based on the current
  // CPU load.
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // Second data sample
  kr = host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &num_cpus,
                           &cpu_info2, &num_cpu_info2);
  if (kr != KERN_SUCCESS) {
    std::cerr << "Error getting CPU info: " << kr << std::endl;
    vm_deallocate(mach_task_self(), (vm_address_t)cpu_info1,
                  num_cpu_info1 * sizeof(integer_t));
    return {};
  }

  // Calculate CPU utilization between the two samples
  for (unsigned int i = 0; i < num_cpus; ++i) {
    natural_t user1 = cpu_info1[i * CPU_STATE_MAX + CPU_STATE_USER];
    natural_t system1 = cpu_info1[i * CPU_STATE_MAX + CPU_STATE_SYSTEM];
    natural_t idle1 = cpu_info1[i * CPU_STATE_MAX + CPU_STATE_IDLE];
    natural_t nice1 = cpu_info1[i * CPU_STATE_MAX + CPU_STATE_NICE];

    natural_t user2 = cpu_info2[i * CPU_STATE_MAX + CPU_STATE_USER];
    natural_t system2 = cpu_info2[i * CPU_STATE_MAX + CPU_STATE_SYSTEM];
    natural_t idle2 = cpu_info2[i * CPU_STATE_MAX + CPU_STATE_IDLE];
    natural_t nice2 = cpu_info2[i * CPU_STATE_MAX + CPU_STATE_NICE];

    natural_t total1 = user1 + system1 + idle1 + nice1;
    natural_t total2 = user2 + system2 + idle2 + nice2;

    if (total1 == 0 || total2 == 0) {
      utilization.push_back("N/A");
    } else {
      double delta_total = total2 - total1;
      double delta_user = user2 - user1;
      double delta_system = system2 - system1;
      double delta_nice = nice2 - nice1;

      double cpu_usage =
          (delta_user + delta_system + delta_nice) / delta_total * 100.0;
      utilization.push_back(std::to_string(cpu_usage));
    }
  }

  // Clean up the CPU info arrays
  vm_deallocate(mach_task_self(), (vm_address_t)cpu_info1,
                num_cpu_info1 * sizeof(integer_t));
  vm_deallocate(mach_task_self(), (vm_address_t)cpu_info2,
                num_cpu_info2 * sizeof(integer_t));

  return utilization;
}
// Read and return the user ID associated with a process
string MacOs_parser::Uid(int pid) {
  struct proc_bsdinfo proc_info;
  string uid = "";

  // Retrieve process info using proc_pidinfo
  if (proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info)) ==
      sizeof(proc_info)) {
    // Use the UID from proc_bsdinfo
    uid = std::to_string(proc_info.pbi_uid);  // Convert UID to string
  } else {
#ifdef DEBUG
    std::cerr << "[MacOs_parser::Uid] Failed to get process info for PID: "
              << pid << ", Error: " << strerror(errno) << std::endl;
#endif
  }

  return uid;  // Return the UID as a string
}

// Read and return the user associated with a process
/*********************************************************************************************/
string MacOs_parser::User(int pid) {
  string strUser{0};              // Initialize string to store user name
  struct proc_bsdinfo proc_info;  // Structure to hold process information

  // Retrieve process information using proc_pidinfo
  if (proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info)) ==
      sizeof(proc_info)) {
    struct passwd* pw =
        getpwuid(proc_info.pbi_uid);  // Get password structure for the user ID
    if (pw) {
      strUser = string(pw->pw_name);  // Store the user name
    } else {
      // Optional: Print an error message for debugging purposes
#ifdef DEBUG
      std::cerr << "[MacOs_parser::User] Failed to get psswd strc for PID: "
                << pid << ", Error: " << strerror(errno) << std::endl;
#endif
    }
  } else {
    // Optional: Print an error message for debugging purposes
#ifdef DEBUG
    std::cerr << "[MacOs_parser::User] Failed to get process info for PID: "
              << pid << ", Error: " << strerror(errno) << std::endl;
#endif
  }

  return strUser;
}

// Read and return the command associated with a process
/*********************************************************************************************/
string MacOs_parser::Command(int pid) {
  string strCommand{0};  // Initialize string to store Command path information
  char pathBuffer[PROC_PIDPATHINFO_MAXSIZE];  // Buffer to store the path of the
                                              // command

  // Retrieve the command path for the given PID
  if (proc_pidpath(pid, pathBuffer, sizeof(pathBuffer)) > 0) {
    strCommand = string(pathBuffer);  // Store the command path
  } else {
    // Optional: Print an error message for debugging purposes
#ifdef DEBUG
    std::cerr << "[MacOs_parser::Command] Failed to get cmd path for PID: "
              << pid << ", Error: " << strerror(errno) << std::endl;
#endif
  }

  return strCommand;
}

// Read and Return memory usage associated with a process
/*********************************************************************************************/
string MacOs_parser::Ram(int pid) {
  string strRam{0};                // Initialize string to store RAM information
  struct proc_taskinfo proc_info;  // Structure to hold process task info

  // Retrieve task info for the given PID
  if (proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &proc_info, sizeof(proc_info)) ==
      sizeof(proc_info)) {
    // Convert resident size from bytes to MB
    long memInMB = proc_info.pti_resident_size / (1024 * 1024);
    strRam = std::to_string(memInMB);
  } else {
    // Optional: Print an error message for debugging purposes
#ifdef DEBUG
    std::cerr << "[MacOs_parser::Ram] Failed to get process info for PID: "
              << pid << ", Error: " << strerror(errno) << std::endl;
#endif
  }

  // Optional: Print the RAM information for debugging purposes
#ifdef DEBUG
  std::cout << "m_sRam = " << strRam << "\n\n";
#endif

  return strRam;  // Return RAM usage as a string
}

// Read and return the uptime associated with a process
/*********************************************************************************************/
long MacOs_parser::UpTime(int pid) {
  long u64Uptime{0};              // Initialize long to store Uptime value
  struct proc_bsdinfo proc_info;  // Structure to hold process info

  // Retrieve process info for the given PID
  if (proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info)) ==
      sizeof(proc_info)) {
    time_t now = time(NULL);  // Get current time

    u64Uptime = now - proc_info.pbi_start_tvsec;  // Calculate uptime

    // Check uptime if negative
    if (u64Uptime < 0) {
      u64Uptime = 0;
    }
  } else {
    // Optional: Print an error message for debugging purposes
#ifdef DEBUG
    std::cerr << "[MacOs_parser::UpTime] Failed to get process info for PID: "
              << pid << ", Error: " << strerror(errno) << std::endl;
#endif
  }

  return u64Uptime;
}
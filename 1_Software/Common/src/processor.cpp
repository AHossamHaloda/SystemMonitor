/**************************************************
********************* Includes ********************
***************************************************/
#include "processor.h"
// Include Mach header for low-level system APIs like task, memory, and IPC
// management.
#include <mach/mach.h>

#include <sstream>  // For std::istringstream
#include <string>
#include <vector>

// Calculates the average CPU utilization based on the collected percentage
// data.
/*********************************************************************************************/
float Processor::Utilization() {
  float utilization{0.0};  // Default value if no valid data
  // Retrieve the list of CPU utilization percentages as strings
  std::vector<std::string> cpuUtilPercentages = m_itfParser->CpuUtilization();
  float sum = 0.0f;  // Sum of valid CPU utilization percentages
  size_t count = 0;  // Number of valid CPU utilization values

  // Iterate through each percentage string
  for (const auto& percentage : cpuUtilPercentages) {
    try {
      // Convert the string to a float
      float value = std::stof(percentage);
      sum += value;  // Accumulate the value
      count++;       // Increment the valid count
    } catch (const std::invalid_argument&) {
      // Handle parsing errors
    }
  }

  // Calculate average utilization if there are valid values
  if (count > 0) {
    utilization = (sum / count) / 100.0f;  // Convert percentage to fraction
  }

  // Return the computed CPU utilization as a fraction
  return utilization;
}
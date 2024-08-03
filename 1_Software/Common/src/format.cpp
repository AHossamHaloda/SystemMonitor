/**************************************************
********************* Includes ********************
***************************************************/
#include "format.h"

#include <iomanip>  // For stream manipulators like std::setw and std::setfill
#include <sstream>
#include <string>

// Formats a given number of seconds into a string representing elapsed time
// in the format "HH:MM:SS".
std::string Format::ElapsedTime(long seconds) {
  std::stringstream timeStream;

  if (seconds > 0) {
    // Calculate hours, minutes, and seconds
    long hours = seconds / 3600;
    long minutes = (seconds % 3600) / 60;
    long secs = seconds % 60;

    // Format time components as two-digit numbers
    timeStream << std::setw(2) << std::setfill('0') << hours << ":"
               << std::setw(2) << std::setfill('0') << minutes << ":"
               << std::setw(2) << std::setfill('0') << secs;
  } else {
    timeStream << "00:00:00";
  }

  return timeStream.str();
}
#ifndef FORMAT_H
#define FORMAT_H
/**************************************************
********************* Includes ********************
***************************************************/
#include <string>

namespace Format {

/**
 * @brief Formats a given number of seconds into a string representing elapsed
 * time in the format "HH:MM:SS".
 *
 * @param seconds The total number of seconds to format.
 * @return A string representing the elapsed time in "HH:MM:SS" format.
 */
std::string ElapsedTime(long times);

};  // namespace Format

#endif  // File Guard
#ifndef PROCESSOR_H
#define PROCESSOR_H

/**************************************************
********************* Includes ********************
***************************************************/
#include "macOs_parser.h"

/**************************************************
********************** Class **********************
***************************************************/
class Processor {
 public:
  /**
   * @brief Constructor that initializes the Processor with the MacOs_parser
   * singleton instance.
   *
   * This constructor sets the `m_itfParser` member to the singleton instance of
   * `MacOs_parser`, which provides the operating system-specific functionality
   * and data required by the `Processor` class.
   *
   */
  Processor() { m_itfParser = &MacOs_parser::getInstance(); };

  /**
   * @brief Computes and returns the average CPU utilization as fractional
   * 0>=CpuUtilization<=1.
   *
   * This function aggregates CPU utilization percentages retrieved from the
   * `m_itfParser` interface. It calculates the average CPU utilization based
   * on the collected percentage data.
   *
   * @return The average CPU utilization as a float. Returns 0.0
   *         if no valid CPU utilization data is available or if there are
   *         errors during data processing.
   */
  float Utilization();

 private:  // private members
  // MacOs_parser object to handle macOS-specific parsing operations
  ItfOperatingSystem* m_itfParser;
};

#endif  // File Guard
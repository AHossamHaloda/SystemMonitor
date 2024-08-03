/*
 * Component: ProcessMonitor_macOs.cpp
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
#include "ProcessMonitor_macOs.hpp"

#include <string>

#include "ncurses_display.h"

void ProcessMonitorMacOs::systemRun() { NCursesDisplay::Display(m_system); }

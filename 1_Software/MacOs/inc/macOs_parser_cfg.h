#ifndef SYSTEM_PARSER_CFG_H
#define SYSTEM_PARSER_CFG_H
/*
 * Component: macOs_parser_cfg.h
 *
 * Description: This component provides cfg. parameters and pathes for MacOs
 * platform.
 *
 * Author: Abdelrahman Hossam
 *
 * Date: August 1st 2024
 *
 */

/**************************************************
********************* Includes *******************
***************************************************/
#include <string>

// Paths

// A plist file containing macOS system version information
const std::string kSystemVersion_plist{
    "/System/Library/CoreServices/SystemVersion.plist"};

#endif

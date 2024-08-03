#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "../inc/process.h"
#include "../inc/system.h"

namespace NCursesDisplay {
void Display(System& system, int n = 10);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
std::string ProgressBar(float percent);
};  // namespace NCursesDisplay

#endif
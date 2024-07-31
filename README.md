# CppND-System-Monitor

This is the code for my System Monitor Project in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

![System Monitor](images/monitor.png)

The **system monitor** program list the operating system, kernel version, memory utilization, total number of processes, number of running processes, and uptime.


The Project is extended to support macOs as well as linux.

---


## <u>Dependencies</u>

**1- ncurses library**
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

**For linux Environment**
You need to install ncurses within your own Linux environment:     
**`sudo apt install libncurses5-dev libncursesw5-dev`**

**For MacOS**
macOS includes the ncurses library by default, so you generally don't need to install it separately, however if you need to install or update ncurses, you can use Homebrew.
**Installing with Homebrew**
**`brew install ncurses`**



**2- Make**
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

---

## Instructions

1. Clone the project repository: `git clone https://github.com/nancyreschka/System-Monitor`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`

---
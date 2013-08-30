/* file: main.cpp
   
   Startup File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This is the entry point for the program that creates
   a new shell object and then starts the control loop.
   
*/


#include "wimpyshell.h"
#include <iostream>

using namespace std;

int main() {
   
   WimpyShell newShell;
   newShell.startShell();
   
   return 0;
}

/* file: wimpyshell.h
   
   Shell Header File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class represents the entire Wimpy Shell. An
   instance of this class is essentially an entire
   shell. It's only public method is to start the main
   control loop of the shell.
   
   NOTE: There is a maximum limit on the number of
         characters that are supported for the current
         working directory (CWD). Extremely nested
         folders will probably break this shell.
         
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on constructors:                   !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   WimpyShell()
   --------------------------------------------------
      This is the basic constructor for the class.
      
      POST: The object is initialized.
   
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on public methods:                 !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   void startShell()
   --------------------------------------------------
      This starts the main control loop for Wimpy Shell.
   
      POST: Wimpy Shell is now running until the user
         decides to quit the program.
   
*/

#ifndef WSH_HEADER
#define WSH_HEADER

#include <iostream>
#include <string>
#include <unistd.h>
#include <errno.h>
#include "JobManager.h"
#include "PipeManager.h"
#include "Command.h"
#include "PipedCommand.h"
#include "ForeJob.h"

// size limit (# of chars) supported for the current working directory
const int MAX_CWD_SIZE = 256;

class WimpyShell {
    
    public:
    
         // constructor
         WimpyShell();
         
         // this is where the all the magic happens
         void startShell();
    
    private:
         
         // methods dealing with builtin commands
         bool runBuiltinCommands();
         void runChangeDir();
         void runWait();
         void runAboutwsh();
         
         //------------------------------------------------------------
         // Data
         //------------------------------------------------------------
         JobManager jobManager;
         Command currentCmdLine;
};

#endif

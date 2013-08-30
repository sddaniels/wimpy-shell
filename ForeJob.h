/* file: ForeJob.h
   
   Foreground Job Class Header File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the code needed to redirect,
   execute, and wait for a foreground job.
   
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on constructors:                   !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   ForeJob(Command new_command)
   --------------------------------------------------
      This is the basic constructor for the class.
   
      PRE:  new_command must a parsed Command object.
   
      POST: new_command is now the value of this
            oject's command.
      
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on public methods:                 !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   bool execute()
   --------------------------------------------------
      Tries to execute the job and then wait for it
      to finish running.
      
      POST: Returns true if the job was successfully
            started and finished. Returns false if
            there was an error.
   
*/

#ifndef FORE_HEADER
#define FORE_HEADER

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "Command.h"

using namespace std;

class ForeJob {
   
    public:
    
         // constructor
         ForeJob(Command new_command);
         
         // execute the job
         bool execute();
    
    private:
    
         // redirection commands
         void redirectOutput();
         void redirectInput();
         
         //------------------------------------------------------------
         // Data
         //------------------------------------------------------------
         Command my_command;
};

#endif

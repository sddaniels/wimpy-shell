/* file: PipeManager.h
   
   Pipe Manager Class Header File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the code needed to execute a
   series of piped commands. It handles creation and
   redirection to pipes of an instance of the PipedCommand
   class.
   
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on constructors:                   !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   PipeManager(PipedCommand new_command)
   --------------------------------------------------
      This is the basic constructor for the class. It
      must be passed a valid PipedCommand object.
      
      PRE:  new_command must be a parsed PipedCommand
            object.
      
      POST: The object has been initialized. This object's
            piped command is set to be the same as
            new_command.
      
      
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on public methods:                 !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   void execute()
   --------------------------------------------------
      Tries to execute the job and then wait for it
      to finish running.
      
      POST: Returns true if the job was successfully
            started and finished. Returns false if
            there was an error.
   
*/

#ifndef PIPE_HEADER
#define PIPE_HEADER

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "PipedCommand.h"

using namespace std;

class PipeManager {
   
    public:
    
         // constructor
         PipeManager(PipedCommand new_command);
         
         void execute();
    
    private:
    
         // methods dealing with pipes
         bool createPipes();
         void closePipes();
         void deletePipes();
         
         // methods dealing with children
         bool createLastChild();
         bool createMiddleChild(int command_index);
         bool createFirstChild();
         void waitForChildren();
         
         // redirection and execution commands
         void redirectOutput(int file_descriptor);
         void redirectInput(int file_descriptor);
         void callExec(int command_index);
         
         //------------------------------------------------------------
         // Data
         //------------------------------------------------------------
         PipedCommand my_command;
         vector<int*> pipe_fds;
         vector<int> pids;
};

#endif

/* file: BackJob.h
   
   Background Job Class Header File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the code needed to redirect,
   execute, check the status of, and wait for a background
   job.
   
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on constructors:                   !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   BackJob(Command new_command)
   --------------------------------------------------
      This is the basic constructor for the class.
      
      PRE:  new_command must a parsed Command object
   
      POST: The object has been initialized. new_command
            is now the value of this object's command.
            
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on public methods:                 !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   bool execute()
   --------------------------------------------------
      Tries to execute the background job. Does not
      wait for it to finish executing.
   
      POST: Returns true if the background job was
            successfully started. Returns false if there
            was an error.
      
      
   bool waitForMe()
   --------------------------------------------------
      Tries to suspend the process until the background
      job specified by my_process_id is finished executing.
      
      PRE:  The method execute() has already been succesfully
            called.
      
      POST: Returns true after the background process is
            finished. Returns false if there was an error
            trying to wait.
         
   
   Command getCommand() const
   --------------------------------------------------
      Returns the background job's Command object.
   
      POST: Returns a Command object.
      
      
   int getPid() const
   --------------------------------------------------
      Returns the background job's process id.
   
      POST: Returns an integer corresponding to this
            background job's process id. Returns -1 if
            the job has not been executed.
      
      
   bool isRunning() const
   --------------------------------------------------
      Returns whether the background job is currently
      running.
   
      POST: Returns true if the job is running. Returns
            false if it is not.
      
      
   bool isFinished() const
   --------------------------------------------------
      Returns whether the background job has finished
      running but has not been displayed to the user yet.
   
      POST: Returns true if the job has finished but
            the user has not been notified. Returns
            false if those conditions are not true.
      
      
   bool isTerminated() const
   --------------------------------------------------
      Returns whether the background job has terminated,
      the user has been notified, and this object is
      now historic information.
   
      POST: Returns true if the job has terminated and
            the user has been notified. Returns false if
            those conditions are not true.
      
      
   bool isFailed() const
   --------------------------------------------------
      Returns whether the background job encountered
      an error when it was being started.
   
      POST: Returns true if there was an error trying to
            run the job. Returns false if there was not.
    
    
    void setFinished(bool yes_no)
    --------------------------------------------------
      Sets whether the process has finished running but
      has not been displayed to the user yet.
      
      PRE:  yes_no is a valid boolean.
      
      POST: If yes_no is true, then the method isFinished()
            will return true. If yes_no is false, then this
            object basically has no status assigned.
    
    
    void setTerminated(bool yes_no)
    --------------------------------------------------
      Sets whether the process has terminated, the user
      has been notified, and this object is now historical
      information.
      
      PRE:  yes_no is a valid boolean.
      
      POST: If yes_no is true, then the method isTerminated()
            will return true. If yes_no is false, then this
            object basically has no status assigned.
   
*/

#ifndef BACK_HEADER
#define BACK_HEADER

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

class BackJob {
   
   public:
   
         // constructor
         BackJob(Command new_command);
         
         // execute the job
         bool execute();
         bool waitForMe();
         
         // get commands
         Command getCommand() const;
         int getPid() const;
         bool isRunning() const;
         bool isFinished() const;
         bool isTerminated() const;
         bool isFailed() const;
         
         // set commands
         void setFinished(bool yes_no);
         void setTerminated(bool yes_no);
   
   private:
         
         // redirection commands
         void redirectOutput();
         void redirectInput();
         
         //------------------------------------------------------------
         // Data
         //------------------------------------------------------------
         int my_process_id;
         bool is_running;
         bool is_finished;   // job just finished, will be displayed next time
         bool is_terminated; // job is essentially dead weight now
         bool is_failed;
         Command my_command;
   
};

#endif

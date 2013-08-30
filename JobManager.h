/* file: JobManager.h
   
   Job Manager Class Header File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class manages and keeps track of all the background
   jobs that are being run. It is in charge of starting,
   managing the status of, and waiting for all of the
   background jobs spawned by the shell. Each background
   job is represented by an instance of the BackJob class.
      
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on constructors:                   !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   JobManager()
   --------------------------------------------------
      This is the basic constructor for the class.
      
      POST: The object has been initialized. There are
            no background jobs running.
            
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on public methods:                 !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   void createBackgroundJob(Command new_command)
   --------------------------------------------------
      Creates and tries to execute as a background job
      the command the user passes.
      
      PRE:  new_command must be a parsed Command object.
      
      POST: If the job starts successfully, it is added to
            the jobs data structure as a running job.
            Otherwise, it is recorded as failed.
      
      
   bool waitForJob(int job_num)
   --------------------------------------------------
      Tries to suspend the process until the background
      job specified by the passed integer has finished
      executing.
      
      PRE:  job_num is an integer that refers to a job
            number of a background job.
      
      POST: Returns true after the background process is
            finished. Returns false if the job number
            specified by the user doesn't correspond
            to a running process or if an error was
            encountered.
   

   void updateJobStatus()
   --------------------------------------------------
      Checks for all background jobs that have finished
      executing and updates their status.
   
      POST: All jobs that have finished executing have
            their status set to finished.
      
      
   void printJobs()
   --------------------------------------------------
      Prints to standard out all of the running and
      recently finished jobs.
      
      POST: All running and finished jobs are printed.
      
      
   void clearOldJobs()
   --------------------------------------------------
      Clears out all recently finished background jobs.
      If no jobs are running and there is nothing left
      to display to the user, the job history is cleaned
      out. (next new background job will have a job # of 1)
      
      POST: All finished jobs are set to terminated. If no
            jobs are left running or set to be displayed,
            the data structure that keeps track of jobs
            is cleared out.
      
*/

#ifndef MNGR_HEADER
#define MNGR_HEADER

#include "Command.h"
#include "BackJob.h"
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

class JobManager {
    
    public:
    
         //constructor
         JobManager();
         
         // job control methods
         void createBackgroundJob(Command new_command);
         bool waitForJob(int job_num);
         
         // methods related to job status updates
         void updateJobStatus();
         void printJobs();
         void clearOldJobs();
    
    private:
    
         // vector index conversion methods
         int noToVec(int job_num);
         int vecToNo(int vec_index);
         
         //------------------------------------------------------------
         // Data
         //------------------------------------------------------------
         vector<BackJob> jobs;
         int num_running;
         int num_finished;
    
};

#endif

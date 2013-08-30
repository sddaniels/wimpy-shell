/* file: JobManager.cpp
   
   Job Manager Class Implementation File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class manages and keeps track of all the background
   jobs that are being run. It is in charge of starting,
   managing the status of, and waiting for all of the
   background jobs spawned by the shell. Each background
   job is represented by an instance of the BackJob class.
   
*/

#include "JobManager.h"

/******************************************************
   This is the basic constructor for the class.
   
   POST: No jobs have been created yet. Job counters
         set to zero.
*/
JobManager::JobManager() {
   // no jobs yet
   num_running = 0;
   num_finished = 0;
}


/******************************************************
   Creates and tries to execute as a background job
   the command the user passes.
   
   PRE:  new_command must be a parsed Command object.
   
   POST: If the job starts successfully, it is added to
         the jobs vector as a running job and the running
         jobs counter is increased by one. Otherwise, it
         is recorded as failed in the jobs vector.
*/
void JobManager::createBackgroundJob(Command new_command) {
   
   // create new background job
   jobs.push_back(BackJob(new_command));
   
   // try to run job, if successful
   if (jobs[jobs.size()-1].execute())
      num_running++;
}

/******************************************************
   Tries to suspend the process until the background
   job specified by the int job_num has finished
   executing.
   
   PRE:  job_num is an integer.
   
   POST: Returns true after the background process is
         finished. Returns false if the job number
         specified by the user doesn't correspond
         to a running process or if an error was
         encountered.
*/
bool JobManager::waitForJob(int job_num) {
   
   int job_index = noToVec(job_num);
   
   // assume job number given is not running
   bool valid_job = false;
   
   // validate job number
   if (!(job_index >= jobs.size())) {
      if (jobs[job_index].isRunning()) {
         valid_job = true;
      }
   }
   
   // wait for job
   if (valid_job) {
      bool success = jobs[job_index].waitForMe();
      
      if (success) {
         num_running--;
         num_finished++;   
      }
      
      return success;
   }
   
   // still here, so job_num was invalid
   cout << "Could not wait for job:" << endl;
   cout << "  Invalid job number." << endl;
   
   return false;
}

/******************************************************
   Checks for all background jobs that have finished
   executing and updates their status.
   
   POST: All finished jobs are set to finished.
*/
void JobManager::updateJobStatus() {
   
   // linux system call to get a terminated child
   int finished_pid = waitpid(-1, NULL, WNOHANG);
   
   // update until no terminated children left
   while ((finished_pid != -1) && (finished_pid != 0)) {
      
      // update job in vector to "finished" status
      for (int updateCtr = 0; updateCtr < jobs.size(); updateCtr++) {
         if (finished_pid == jobs[updateCtr].getPid()) {
            jobs[updateCtr].setFinished(true);
            num_running--;
            num_finished++;
         }
      }
      
      // system call again
      finished_pid = waitpid(-1, NULL, WNOHANG);
   }
   
   // check if something nasty happend
   if ((finished_pid == -1) && (errno != 10)) { // errno 10 is "no children found" 
      cout << "Background process update error: pid " << finished_pid << endl;
      cout << "  " << strerror(errno) << "." << endl;
   }
 
}

/******************************************************
   Prints to standard out all of the running and
   recently finished jobs.
   
   POST: All running and finished jobs are printed.
*/
void JobManager::printJobs() {
   
   // print all running jobs
   if (num_running > 0) {
      
      cout << "    Running:" << endl;
      
      for (int runCtr = 0; runCtr < jobs.size(); runCtr++) {
         if (jobs[runCtr].isRunning()) {
            cout << "        [" << vecToNo(runCtr) << "] " << jobs[runCtr].getCommand().getCommandText() << endl;
         }
      }
   }
   
   // print all finished jobs
   if (num_finished > 0) {
      
      cout << "    Finished:" << endl;
      
      for (int finCtr = 0; finCtr < jobs.size(); finCtr++) {
         if (jobs[finCtr].isFinished()) {
            cout << "        [" << vecToNo(finCtr) << "] " << jobs[finCtr].getCommand().getCommandText() << endl;
         }
      }
   }
}

/******************************************************
   Clears out all recently finished background jobs.
   If no jobs are running and there is nothing left
   to display to the user, the job history is cleaned
   out. (next new background job will have a job # of 1)
   
   POST: All finished jobs are set to terminated. If no
         jobs are left running or set to be displayed,
         the jobs vector is cleared.
*/
void JobManager::clearOldJobs() {
   
   // go through vector and set all "finshed" jobs to "terminated"
   for (int termCtr = 0; termCtr < jobs.size(); termCtr++) {
      
      if(jobs[termCtr].isFinished()) {
         jobs[termCtr].setTerminated(true);
         num_finished--;
      }
   }
   
   // clear out vector if all info is old
   if ((num_running == 0) && (num_finished == 0)) {
      jobs.clear();
   }
   
}

/******************************************************
   Converts a job number into a vector index.
   
   PRE:  job_num is an integer.
   
   POST: Returns the vector index of that job number.
*/
int JobManager::noToVec(int job_num) {
   int vec_index = job_num - 1;
   return vec_index;
}
         
/******************************************************
   Converts a vector index into a job number.
   
   PRE:  vec_index is an integer.
   
   POST: Returns the job number corresponding to the
         passed index.
*/
int JobManager::vecToNo(int vec_index) {
   int job_num = vec_index + 1;
   return job_num;
}

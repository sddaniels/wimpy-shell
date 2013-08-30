/* file: BackJob.cpp
   
   Background Job Class Implementation File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the code needed to redirect,
   execute, check the status of, and wait for a background
   job.
   
*/

#include "BackJob.h"

using namespace std;

/******************************************************
   This is the basic constructor for the class.
   
   PRE:  new_command must a parsed Command object
   
   POST: my_command is set and all of the booleans
         are initialized to false
*/
BackJob::BackJob(Command new_command) {
   my_command = new_command;
   
   is_running = false;
   is_terminated = false;
   is_finished = false;
   is_failed = false;
   
   my_process_id = -1;
}

/******************************************************
   Tries to execute the background job.
   
   POST: Returns true if the background job was
         successfully started. Returns false if there
         was an error.
*/
bool BackJob::execute() {
   
   // linux system call
   int pid = fork();
   
   // error
   if (pid < 0) {
      cout << "Execution error: " << endl;
      cout << "  Could not create process." << endl;
      
      is_failed = true;
      return false;
   }
   
   // child process
   if (pid == 0) {
      
      // check for input redirection
      if (my_command.isInputRedirected()) {
         redirectInput();
      }
      
      // check for output redirection
      if (my_command.isOutputRedirected()) {
         redirectOutput();
      }
      
      // linux system call
      execvp(my_command.getCommandName().c_str(), my_command.getArgsArray());
      
      // still here, must be an error
      is_failed = true;
      cout << endl << "Execution error:" << endl;
      
      if (errno == 2)
         cout << "  Command not found." << endl;   
      else
         cout << "  " << strerror(errno) << "." << endl;
      
      // kludge to fix prompting inelegance
      cout << "wsh: ";
      
      exit(-1);
   }
   
   // still here, must be the parent
   my_process_id = pid;
   
   is_running = true;
   return true;
}

/******************************************************
   Tries to suspend the process until the background
   job specified by my_process_id is finished executing.
   
   PRE:  The background job specified by my_command
         has been started successfully
         (i.e. execute() has been called)
   
   POST: Returns true after the background process is
         finished. Returns false if there was an error
         trying to wait.
*/
bool BackJob::waitForMe() {
   
   int pid_success = waitpid(my_process_id, NULL, 0);
   
   // check if something nasty happend
   if (pid_success == -1) {
      cout << "Could not wait for job:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      return false;
   }
   
   setFinished(true);
   
   return true;
}

/******************************************************
   Returns the background job's Command object.
   
   POST: my_command is returned.
*/
Command BackJob::getCommand() const {
   return my_command;
}

/******************************************************
   Returns the background job's process id.
   
   POST: my_process_id is returned.
*/
int BackJob::getPid() const {
   return my_process_id;
}

/******************************************************
   Returns whether the background job is currently
   running.
   
   POST: is_running is returned.
*/
bool BackJob::isRunning() const {
   return is_running;
}

/******************************************************
   Returns whether the background job has finished
   running but has not been displayed to the user yet.
   
   POST: is_finished is returned.
*/
bool BackJob::isFinished() const {
   return is_finished;
}

/******************************************************
   Returns whether the background job has terminated,
   the user has been notified, and this object is
   now historic information.
   
   POST: is_terminated is returned.
*/
bool BackJob::isTerminated() const {
   return is_terminated;
}

/******************************************************
   Returns whether the background job encountered
   an error when it was being started.
   
   POST: is_failed is returned.
*/
bool BackJob::isFailed() const {
   return is_failed;
}

/******************************************************
   Sets whether the process has finished running but
   has not been displayed to the user yet.
   
   PRE:  yes_no is a valid boolean.
   
   POST: is_finished is set and the other booleans
         are set to false.
*/
void BackJob::setFinished(bool yes_no) {
   is_running = false;
   is_finished = yes_no;
   is_terminated = false;
}

/******************************************************
   Sets whether the process has terminated, the user
   has been notified, and this object is now historical
   information.
   
   PRE:  yes_no is a valid boolean.
   
   POST: is_terminated is set and the other booleans
         are set to false.
*/
void BackJob::setTerminated(bool yes_no) {
   is_running = false;
   is_finished = false;
   is_terminated = yes_no;
}

/******************************************************
   Tries to redirect the output of the job to the file
   specified in the my_command object.
   
   PRE:  my_command has been parsed.
   
   POST: Returns after a successful redirect attempt.
         Otherwise, an error message is printed and
         the process terminates.
*/
void BackJob::redirectOutput() {
   
   // convert filename string into char array
   char *file_ptr = NULL;
   file_ptr = new char [my_command.getOutputFileName().size() + 1]; 
   strcpy(file_ptr, my_command.getOutputFileName().c_str());
    
   // linux system call get file descriptor
   int file_descriptor = open(file_ptr, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
   
   // check if something nasty happened
   if (file_descriptor == -1) {
      
      cout << "Output file error:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   }
   
   // linux system call to redirect output
   int redirect_success = dup2(file_descriptor, 1);
   
   // check if something nasty happened
   if (redirect_success == -1) {
      
      cout << "Output file redirect error:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   }
   
   close(file_descriptor);
   
   // check if something nasty happened
   if (redirect_success == -1) {
      
      cout << "Output file error:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   } 
}

/******************************************************
   Tries to redirect the input of the job to the file
   specified in the my_command object.
   
   PRE:  my_command has been parsed.
   
   POST: Returns after a successful redirect attempt.
         Otherwise, an error message is printed and
         the process terminates.
*/
void BackJob::redirectInput() {
   
   // convert filename string into char array
   char *file_ptr = NULL;
   file_ptr = new char [my_command.getInputFileName().size() + 1]; 
   strcpy(file_ptr, my_command.getInputFileName().c_str());
    
   // linux system call get file descriptor
   int file_descriptor = open(file_ptr, O_RDONLY);
   
   // check if something nasty happened
   if (file_descriptor == -1) {
      
      cout << "Input file error:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   }
   
   // linux system call to redirect input
   int redirect_success = dup2(file_descriptor, 0);
   
   // check if something nasty happened
   if (redirect_success == -1) {
      
      cout << "Input file redirect error:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   }
   
   close(file_descriptor);
   
   // check if something nasty happened
   if (redirect_success == -1) {
      
      cout << "Input file error:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   }  
}

/* file: ForeJob.cpp
   
   Foreground Job Class Implementation File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the code needed to redirect,
   execute, and wait for a foreground job.
   
*/

#include "ForeJob.h"

using namespace std;

/******************************************************
   This is the basic constructor for the class.
   
   PRE:  new_command must a parsed Command object
   
   POST: my_command is set.
*/
ForeJob::ForeJob(Command new_command) {
   my_command = new_command;
}

/******************************************************
   Tries to execute the job and then wait for it
   to finish running.
   
   POST: Returns true if the job was successfully started
         and finished running. Returns false if there
         was an error.
*/
bool ForeJob::execute() {
   
   // linux system call
   int pid = fork();
   
   // error
   if (pid < 0) {
      cout << "Execution error: " << endl;
      cout << "  Could not create process." << endl;
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
      
      // linux system call to replace process with another process
      execvp(my_command.getCommandName().c_str(), my_command.getArgsArray());
      
      // still here, must be an error
      cout << "Execution error:" << endl;
      
      if (errno == 2)
         cout << "  Command not found." << endl;
      else
         cout << "  " << strerror(errno) << "." << endl;
      
      exit(-1);
   }
   
   // still here, must be the parent
   int pid_success = waitpid(pid, NULL, 0);
   
   // check if something nasty happend
   if (pid_success == -1) {
      cout << "Execution error:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      return false;
   }
   
   return true;
}

/******************************************************
   Tries to redirect the output of the job to the file
   specified in the my_command object.
   
   PRE:  my_command has been parsed.
   
   POST: Returns after a successful redirect attempt.
         Otherwise, an error message is printed and
         the process terminates.
*/
void ForeJob::redirectOutput() {
   
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
void ForeJob::redirectInput() {
   
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

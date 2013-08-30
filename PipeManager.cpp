/* file: PipeManager.cpp
   
   Pipe Manager Class Implementation File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the code needed to execute a
   series of piped commands. It handles creation and
   redirection to pipes of an instance of the PipedCommand
   class.
   
*/

#include "PipeManager.h"

/******************************************************
   This is the basic constructor for the class. It
   must be passed a valid PipedCommand object.
   
   PRE:  new_command must be a parsed PipedCommand object.
   
   POST: The member variable my_command is initialized
         to the piped command that the user passed.
*/
PipeManager::PipeManager(PipedCommand new_command) {
   my_command = new_command;
}

/******************************************************
   Tries to execute the job and then wait for it
   to finish running.
   
   POST: Returns true if the job was successfully started
         and finished running. Returns false if there
         was an error.
*/
void PipeManager::execute() {

   // create arrays to pass to pipe system call
   createPipes();
   
   // create last child first
   if (!createLastChild())
      return;
      
   // create all middle children in reverse order
   int num_middle_children = my_command.getCommands().size() - 2; // - 2 because we're doing first and last separately
   for (int childCtr = num_middle_children; childCtr > 0; childCtr--) {
      
      if (!createMiddleChild(childCtr))
         return;
   }
   
   if (!createFirstChild())
      return;
   
   // must be in parent now, close pipes and wait for children
   closePipes();
   waitForChildren();
   deletePipes();

   return;   
}

/******************************************************
   Tries to redirect the output of the currently
   running process to the passed file descriptor.
   
   PRE:  file_descriptor is a valid descriptor for
         the write end of a pipe.
   
   POST: Returns after a successful redirect attempt.
         Otherwise, an error message is printed and
         the process terminates.
*/
void PipeManager::redirectOutput(int file_descriptor) {
   
   // linux system call to redirect output
   int redirect_success = dup2(file_descriptor, 1);
   
   // check if something nasty happened
   if (redirect_success == -1) {
      cout << "Could not use pipeline:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   }
   
   redirect_success = close(file_descriptor);
   
   // check if something nasty happened
   if (redirect_success == -1) {
      
      cout << "Input file error:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   }
}

/******************************************************
   Tries to redirect the input of the currently
   running process to the passed file descriptor.
   
   PRE:  file_descriptor is a valid descriptor for
         the read end of a pipe.
   
   POST: Returns after a successful redirect attempt.
         Otherwise, an error message is printed and
         the process terminates.
*/
void PipeManager::redirectInput(int file_descriptor) {
   
   // linux system call to redirect input
   int redirect_success = dup2(file_descriptor, 0);
   
   // check if something nasty happened
   if (redirect_success == -1) {
      cout << "Could not use pipeline:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   }
   
   redirect_success = close(file_descriptor);
   
   // check if something nasty happened
   if (redirect_success == -1) {
      
      cout << "Input file error:" << endl;
      cout << "  " << strerror(errno) << "." << endl;
      exit(-1);
   }
}

/******************************************************
   Tries to replace the currently running process with
   the another job that needs to be run in the pipeline.
   
   PRE:  command_index is an integer that refers to a
         job in the PipedCommand object my_command.
   
   POST: Does not return if successful. If an error was
         encountered, an error message is printed and
         the process terminates.
*/
void PipeManager::callExec(int command_index) {
      
      // do a quick reality check
      if (command_index >= my_command.getCommands().size()) {
         cout << "Could not use pipeline:" << endl;
         cout << "  Invalid command index." << endl;
         exit(-1);
      }
      
      // linux system call to replace process with another process
      execvp(my_command.getCommands()[command_index].getCommandName().c_str(), my_command.getCommands()[command_index].getArgsArray());
      
      // still here, must be an error
      cout << "Could not use pipeline:" << endl;
      
      if (errno == 2)
         cout << "  Command not found." << endl;
      else
         cout << "  " << strerror(errno) << "." << endl;
      
      exit(-1);
}

/******************************************************
   Creates the appropriate number of pipes needed to
   run the entired piped command.
   
   POST: Returns true if all of the pipes were successfully
         created and their file descriptor array pointers
         pushed onto the vector pipe_fds. Returns false
         if an error was encountered.
*/
bool PipeManager::createPipes() {
   
   // we need 1 less pipe than the number of commands
   int num_pipes_needed = my_command.getCommands().size() - 1;
   
   // create pipes and push arrays onto vector
   for (int pipePtr = 0; pipePtr < num_pipes_needed; pipePtr++) {
      int* pipefd = new int[2];
      pipe_fds.push_back(pipefd);
     
      // linux system call to create a pipe
      if (pipe(pipe_fds[pipe_fds.size() - 1]) == -1) {
         cout << "Could not create pipe:" << endl;
         cout << "  " << strerror(errno) << "." << endl;
         return false;
      }
   }
}

/******************************************************
   Tries to redirect the output of the currently
   running process to the passed file descriptor.
   
   PRE:  Usually this method is run only after pipes
         have been created.
   
   POST: Returns after all of the pipes that are stored
         in pipe_fds have had their file descriptors closed.
*/
void PipeManager::closePipes() {
   
   // close read/write ends of all pipes in parent
   for (int closePtr = 0; closePtr < pipe_fds.size(); closePtr++) {
      
      //linux system calls
      close(pipe_fds[closePtr][0]);
      close(pipe_fds[closePtr][1]);
   }
}

/******************************************************
   Deallocates all of the space that was allocated
   to store pipe file descriptors, then clears
   out the pointers to those arrays.
   
   PRE:  file_descriptor is a valid descriptor for
         the write end of a pipe.
   
   POST: Returns after a successful redirect attempt.
         Otherwise, an error message is printed and
         the process terminates.
*/
void PipeManager::deletePipes() {
   
   // deallocate memory used in pipe file descriptor arrays
   for (int dePtr = 0; dePtr < pipe_fds.size(); dePtr++) {
      delete [] pipe_fds[dePtr];
   }
   
   pipe_fds.clear();
}

/******************************************************
   Tries to redirect, fork, and execute the last job in
   the piped command.
   
   PRE:  The necessary pipes have already been created
         and file descriptor array pointers are stored
         in the vector pipe_fds.
   
   POST: Returns true in the parent if successful. Nothing
         is returned on success in the child. Returns false
         if an error was encountered.
*/
bool PipeManager::createLastChild() {
   
   // linux system call
   int pid = fork();
   
   // error checking
   if (pid == -1) {
      cout << "Execution error: " << endl;
      cout << "  Could not create process." << endl;
      return false;
   }
      
   // child code
   if (pid == 0) {
      
      // figure out which pipe we're using
      int my_pipe = pipe_fds.size() - 1;
      
      close(pipe_fds[my_pipe][1]);         // we don't want to write to pipe
      redirectInput(pipe_fds[my_pipe][0]); // read end of the pipe
      
      // close all other pipes
      for (int closePtr = 0; closePtr < pipe_fds.size(); closePtr++) {
      
         if (closePtr != my_pipe) {
            //linux system calls
            close(pipe_fds[closePtr][0]);
            close(pipe_fds[closePtr][1]);
         }
      }
      
      // replace process code with last job
      callExec(my_command.getCommands().size() - 1);
   }
   
   // still here, must be in parent
   pids.push_back(pid);
   return true;
}

/******************************************************
   Tries to redirect, fork, and execute the job passed
   to the method. This method is designed to work with
   two pipes, so it should not be used for the first
   and last jobs in the pipeline.
   
   PRE:  The necessary pipes have already been created
         and file descriptor array pointers are stored
         in the vector pipe_fds.
   
   POST: Returns true in the parent if successful. Nothing
         is returned on success in the child. Returns false
         if an error was encountered.
*/
bool PipeManager::createMiddleChild(int command_index) {
   
   // linux system call
   int pid = fork();
   
   // error checking
   if (pid == -1) {
      cout << "Execution error: " << endl;
      cout << "  Could not create process." << endl;
      return false;
   }
      
   // child code
   if (pid == 0) {
   
      // figure out which pipes we're using
      int out_pipe = command_index;
      int in_pipe = command_index - 1;
      
      redirectInput(pipe_fds[in_pipe][0]);   // read end of the pipe
      close(pipe_fds[in_pipe][1]);
      
      redirectOutput(pipe_fds[out_pipe][1]); // write end of pipe
      close(pipe_fds[out_pipe][0]);
      
      // close all other pipes
      for (int closePtr = 0; closePtr < pipe_fds.size(); closePtr++) {
      
         if ((closePtr != in_pipe) && (closePtr != out_pipe)) {
            //linux system calls
            close(pipe_fds[closePtr][0]);
            close(pipe_fds[closePtr][1]);
         }
      }
      
      // replace process code with the given job
      callExec(command_index);
   }
   
   // still here, must be in parent
   pids.push_back(pid);
   return true;
}

/******************************************************
   Tries to redirect, fork, and execute the first job in
   the piped command.
   
   PRE:  The necessary pipes have already been created
         and file descriptor array pointers are stored
         in the vector pipe_fds.
   
   POST: Returns true in the parent if successful. Nothing
         is returned on success in the child. Returns false
         if an error was encountered.
*/
bool PipeManager::createFirstChild() {
   
   // linux system call
   int pid = fork();
   
   // error checking
   if (pid == -1) {
      cout << "Execution error: " << endl;
      cout << "  Could not create process." << endl;
      return false;
   }
      
   // child code
   if (pid == 0) {
      
      // first job, so we're using the first pipe
      int my_pipe = 0;
      
      close(pipe_fds[my_pipe][0]);          // we don't want to read from pipe
      redirectOutput(pipe_fds[my_pipe][1]); // write end of pipe
      
      // close all other pipes
      for (int closePtr = 0; closePtr < pipe_fds.size(); closePtr++) {
      
         if (closePtr != my_pipe) {
            //linux system calls
            close(pipe_fds[closePtr][0]);
            close(pipe_fds[closePtr][1]);
         }
      }
      
      // replace process code with first job
      callExec(0);
   }
   
   // still here, must be in parent
   pids.push_back(pid);
   return true;
}

/******************************************************
   Tries to suspend the parent process until all of
   the children that were created have finished executing.
   After the process resumes, the pids of the children
   are cleared out.
   
   PRE:  Usually, this method will not be called until
         all of the children necessary for the pipe
         have been forked.
   
   POST: Returns when all of the children have finished
         executing. The vector pids has been cleared.
*/
void PipeManager::waitForChildren() {
   
   // go through pids vector in reverse order and wait for children
   for (int pidCtr = (pids.size() - 1); pidCtr > -1; pidCtr--) {
      
      // linux system call
      waitpid(pids[pidCtr], NULL, 0);
   }
   
   pids.clear();
}

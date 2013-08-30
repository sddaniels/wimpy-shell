/* file: wimpyshell.cpp
   
   Shell Implementation File
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
   
*/

#include "wimpyshell.h"

using namespace std;

/******************************************************
   This is the basic constructor for the class.
   
   POST: The object is initialized.
*/
WimpyShell::WimpyShell() {

   // nothing to initialize, really
}

/******************************************************
   This starts the main control loop for Wimpy Shell.
   
   POST: Wimpy Shell is now running until the user
         decides to quit the program.
*/
void WimpyShell::startShell() {
   
   cout << "Welcome to Wimpy Shell v1.0" << endl;
   
   // main control loop
   while (!cin.eof()) {
      
      currentCmdLine.resetCommand();
      
      // command line prompt
      cout << "wsh: ";
      
      // get a command line from the user
      string userInputString;
      getline(cin, userInputString);
      currentCmdLine.setCommandText(userInputString);
      
      // update status of jobs before starting another one
      jobManager.updateJobStatus();
      
      // check for piping, must process differently
      PipedCommand pipedCmdLine;
      pipedCmdLine.setCommandText(currentCmdLine.getCommandText());
      
      if (pipedCmdLine.checkForPiping()) { // piped command
         
         if (!pipedCmdLine.parsePipedCommand()) {
            cout << "Command could not be parsed: " << endl;
            cout << "  " << pipedCmdLine.getErrorReason() << endl;
         } else {
            PipeManager pipeManager(pipedCmdLine);
            pipeManager.execute();
         }
         
      } else { // normal command
       
         // parse and check for errors
         if (!currentCmdLine.parseCommandText()) {
            
            if (!(currentCmdLine.getErrorReason() == "Empty command.")) {
               cout << "Command could not be parsed: " << endl;
               cout << "  " << currentCmdLine.getErrorReason() << endl;
            }
            
         } else { // parsed correctly
            
            // try to run builtin commands
            if(!runBuiltinCommands()) {
               
               // not a builtin, run background or foreground job
               if (currentCmdLine.isBackgroundJob()) {
                  jobManager.createBackgroundJob(currentCmdLine);
               } else {
                  ForeJob run_me(currentCmdLine);
                  run_me.execute();
               } 
            }
            
         }
         
      }
      
      // update jobs again and print
      jobManager.updateJobStatus();
      jobManager.printJobs();
      jobManager.clearOldJobs();
      
   } // end of main while
   
   currentCmdLine.resetCommand();
}

/******************************************************
   Checks the current command to see if it should be
   handled as a builtin command by the shell. If it
   is, the command is executed.
   
   POST: Returns true if the current command is a
         builtin, regardless of if it executes
         successfully. Returns false if the current
         command is not a builtin command.
*/
bool WimpyShell::runBuiltinCommands() {
   
   // end the shell
   if (currentCmdLine.getCommandName() == "exit") {
      exit(0);
   }
   
   // change directory
   if (currentCmdLine.getCommandName() == "cd") {
      runChangeDir();
      return true;
   }
   
   // wait for background process
   if (currentCmdLine.getCommandName() == "wait") {
      runWait();
      return true;
   }
   
   // personal vanity
   if (currentCmdLine.getCommandName() == "aboutwsh") {
      runAboutwsh();
      return true;
   } 
   
   return false;
}

/******************************************************
   Tries to change the current working directory of
   wimpy shell to the first argument of the cd command.
   The current working directory is then printed to
   standard output.
   
   PRE:  currentCmdLine must be a change directory
         "cd" command.
   
   POST: Returns after an attempt is made to change the
         CWD and the CWD is printed to stdout. If an
         error was encountered an error message is printed
         and the CWD remains the same.
*/
void WimpyShell::runChangeDir() {
   
   // check to make sure an argument exists, segmentation faults BAD!!!
   if (currentCmdLine.getArgs().empty()) {
      
      cout << "Could not change directory:" << endl;
      cout << "  No directory given." << endl;
      
   // otherwise try to change directory
   } else {
    
      // linux system call
      int change_success = chdir(currentCmdLine.getArgs()[0].c_str());
      
      if (change_success == -1) {
         cout << "Could not change directory:" << endl;
         
         if (errno == 2) { // file not found
            cout << "  No such directory." << endl;
         } else {
            cout << "  " << strerror(errno) << "." << endl;
         }
      } 
      
   }
   
   // create array to store address
   char *cwd_ptr = NULL;
   cwd_ptr = new char [MAX_CWD_SIZE];

   // linux system call
   getcwd(cwd_ptr, MAX_CWD_SIZE);
   
   // convert cstring to string and print
   string cwd = static_cast<const char*>(cwd_ptr);
   cout << "Working directory " << cwd << endl;
   
   delete [] cwd_ptr;
   cwd_ptr = NULL;
}

/******************************************************
   Tries to wait for the background job specified
   in the command arguments to finished executing.
   
   PRE:  currentCmdLine must be a "wait" command.
   
   POST: Returns after an attempt is made to wait for
         the process specified in the first argument
         of currentCmdLine.
*/
void WimpyShell::runWait() {
      
   // check for an argument
   if (currentCmdLine.getArgs().size() == 0) {
      cout << "Could not wait for job:" << endl;
      cout << "  No job number given." << endl;
   
   // try to wait for the job
   } else {
      int job_num = atoi(currentCmdLine.getArgs()[0].c_str());
      jobManager.waitForJob(job_num);
   }
}

/******************************************************
   Prints my ode to personal vanity to standard output.
   
   PRE:  currentCmdLine must be the about Wimpy Shell
         "aboutwsh" command.
   
   POST: Returns after the logo and info has been printed.
*/
void WimpyShell::runAboutwsh() {
   
   cout << " _      _                                  ___" << endl;
   cout << "  |  |  /    ,                            /    )   /           /   /" << endl;
   cout << "  | /| /        _  _      __              \\       /__    __   /   /" << endl;
   cout << "  |/ |/    /   / /  )   /   ) /   /        \\     /   ) /___) /   /" << endl;
   cout << "__/  |____/___/ /  /___/___/_(___/_____(____/___/   /_(___ _/___/___" << endl;
   cout << "                      /         /" << endl;
   cout << "                     /      (_ /" << endl << endl;
   cout << "Wimpy Shell was written by Shea Daniels in 2006." << endl << endl;
}

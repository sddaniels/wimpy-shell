/* file: PipedCommand.cpp
   
   Piped Command Class Implementation File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the data and functions
   needed for storing and parsing piped commands.
   
*/

#include "PipedCommand.h"

using namespace std;

/******************************************************
   This is the basic constructor for the class.
   
   POST: The member variable command_text is initialized
         to "none".
*/
PipedCommand::PipedCommand() {
   command_text = "none";
}

/******************************************************
   Sets the text of the entire command to whatever
   string the user specifies.
   
   PRE:  new_cmd_text is a string containing the text
         of the command line the user input
         
   POST: command_text has been set
*/
void PipedCommand::setCommandText(string new_cmd_text) {
   command_text = new_cmd_text;
}

/******************************************************
   Returns the text of the command line.
      
   POST: command_text has been returned.
*/         
string PipedCommand::getCommandText() const {
   return command_text;
}

/******************************************************
   Returns the reason the command could not be parsed.
      
   POST: error_reason has been returned.
*/
string PipedCommand::getErrorReason() const {
   return error_reason;
}

/******************************************************
   Returns the vector of commands for the piped command.
      
   POST: The vector cmds has been returned.
*/
vector<Command> PipedCommand::getCommands() const {
   return cmds;
}

/******************************************************
   Checks to see if the command actually is a piped
   command or if it is not. (looks for '|' chars)
      
   PRE:  command_text has been set.
   
   POST: Returns true if the command requires piping.
         Returns false if it does not.
*/         
bool PipedCommand::checkForPiping() {
   
   int pipePosition = command_text.find('|');
   
   // no pipes in command
   if (pipePosition == string::npos)
      return false;
      
   return true;
}


/******************************************************
   Tries to parse the piped command into separate jobs
   and then parse those commands.
      
   PRE:  command_text has been set.
   
   POST: Returns true if successful. Also, the member
         vector cmds will contain the nested jobs
         in the command, and those will be parsed as
         well. Returns false and sets error_reason
         if unsuccessful.
*/  
bool PipedCommand::parsePipedCommand() {
   
   int currentPos = 0;
   bool newWord = true;
   int currentCommand;
   
   // check for '|' char in first position of string
   // gives nasty error if we don't check
   if (command_text[0] == '|') {
      error_reason = "Missing a job.";
      return false;
   }
   
   // keep parsing until the end of the string, unless error
   while (currentPos < command_text.size()) {  
      
      // check if current char is part of the command
      if (command_text[currentPos] != '|') {
         
         if (newWord) {
            
            newWord = false;
            
            // push a new Command onto the vector
            cmds.push_back(Command("pipe"));
            
            // get address of newest arg
            currentCommand = cmds.size() - 1;
            
            cmds[currentCommand].setCommandText("");
         }
         
         // append current char to end of sub command
         cmds[currentCommand].setCommandText(cmds[currentCommand].getCommandText() + command_text[currentPos]);
         
         currentPos++;  
         
      // end of command
      } else {
         
         // skip past '|' character
         currentPos++;
         
         // try to parse the command
         if (!cmds[currentCommand].parseCommandText()) {
            error_reason = cmds[currentCommand].getErrorReason();
            
            if (error_reason == "Empty command.")
               error_reason = "Missing a job.";
            
            return false;
         }
         
         newWord = true;
      }
   }
   
   // parse the last command, since we didn't get to it in the while loop
   if (!cmds[currentCommand].parseCommandText()) {
      error_reason = cmds[currentCommand].getErrorReason();
      
      if (error_reason == "Empty command.")
         error_reason = "Missing a job.";
      
      return false;
   }
   
   // check to make sure there's at least two commands in vector
   if (cmds.size() < 2) {
      error_reason = "Missing a job.";
      return false;
   }
   
   // FOLLOWING FOR TESTING ONLY!!!!
   /*
   for (int i = 0; i < cmds.size(); i++) {
      cout << "pipe command " << i << " : '" << cmds[i].getCommandText() << "'" << endl;
   }
   
   cout << endl;
   */
   
   return true;
}

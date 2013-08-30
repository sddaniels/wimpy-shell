/* file: Command.cpp
   
   Command Class Implementation File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the data and functions
   needed for storing and parsing command lines.
   
*/

#include "Command.h"

using namespace std;

/******************************************************
   This is the basic constructor for the class.
   
   POST: All of the string vars are initialized to "none".
*/
Command::Command() {
   
   // set flags
   input_redirect = false;
   output_redirect = false;
   background_job = false;
   piped_job = false;
   
   // init data to "none"
   command_text = "none";
   cmd_name = "none";
   input_file = "none";
   output_file = "none";
   error_reason = "none";
}

/******************************************************
   This is the constructor for commands that are part
   of a larger piped job. 
   
   PRE:  pipe_me is any string.
   
   POST: All of the string vars are initialized to "none".
         If the passed string pipe_me equal "pipe", then 
         the command will be set up to belong to a larger
         PipedCommand object.
*/
Command::Command(string pipe_me) {
   
   // set flags
   input_redirect = false;
   output_redirect = false;
   background_job = false;
   
   if ("pipe" == pipe_me) {
      piped_job = true;
   } else {
      piped_job = false;
   }
   
   // init data to "none"
   command_text = "none";
   cmd_name = "none";
   input_file = "none";
   output_file = "none";
   error_reason = "none";
}

/******************************************************
   Sets the command text variable.
   
   PRE:  new_cmd_text is a string containing the new
         command line
         
   POST: command_text is replaced with the new string
*/
void Command::setCommandText(string new_cmd_text) {
   
   command_text = new_cmd_text;
}

/******************************************************
   Returns the command text variable.
   
   POST: A string containing the text of the command
         line is returned.
*/
string Command::getCommandText() const {
   
   return command_text;
}

/******************************************************
   Returns the reason the command couldn't be parsed.

   POST: If command couldn't be parsed, returns a string
         containing the reason.
         If command was parsed successfully or has not
         been parsed at all, returns "none".
*/
string Command::getErrorReason() const {
   return error_reason;  
}

/******************************************************
   Returns the name of the executable for the command.

   POST: cmd_name is returned.
*/
string Command::getCommandName() const {
   return cmd_name;
}

/******************************************************
   Returns the name of the file that input should
   be redirected to.

   POST: input_file is returned.
*/
string Command::getInputFileName() const {
   return input_file;
}

/******************************************************
   Returns the name of the file that output should
   be redirected to.

   POST: output_file is returned.
*/
string Command::getOutputFileName() const {
   return output_file;
}

/******************************************************
   Returns the vector that contains all of the command
   arguments.
   
   POST: cmd_arguments is returned.
*/
vector<string> Command::getArgs() const {
   return cmd_arguments;
}

/******************************************************
   Returns a pointer to an array of pointers to null
   terminated character arrays containing the command
   arguments. This data structure is appropriate for
   use with the execv() system call.
   
   PRE:  The command has been parsed.
   
   POST: The pointer to the dynamically allocated
         array is returned. 
*/
char ** Command::getArgsArray() {
   
   // figure out how big array should be
   int array_size = cmd_arguments.size();
   array_size += 2; // argv[0] is command, last entry is NULL
   
   char **argv = new char*[array_size]; 
   
   // convert command name to null terminated char array
   // and save into argv array
   // THIS IS UGLY BECAUSE CSTRINGS SUCK!
   argv[0] = NULL;
   argv[0] = new char[cmd_name.size() + 1];
   strcpy(argv[0], cmd_name.c_str());
   
   // put all of the arguments in vector into array
   for (int arg_ctr = 1; arg_ctr < (array_size-1); arg_ctr++) {
      
      // get string from vector
      int vector_ctr = arg_ctr - 1;
      string current_arg = cmd_arguments[vector_ctr];
      
      // convert to cstring and save into argv array
      argv[arg_ctr] = NULL;
      argv[arg_ctr] = new char[current_arg.size() + 1];
      strcpy(argv[arg_ctr], current_arg.c_str());
   }
   
   argv[array_size-1] = NULL;
   
   return argv;
}

/******************************************************
   Returns whether the command will have its input
   redirected to a file instead of std input.
   
   POST: Returns true if the input is redirected.
         Returns false if it is not.
*/
bool Command::isInputRedirected() const{
   return input_redirect;
}

/******************************************************
   Returns whether the command will have its output
   redirected to a file instead of std out.
   
   POST: Returns true if the output is redirected.
         Returns false if it is not.
*/
bool Command::isOutputRedirected() const {
   return output_redirect;
}

/******************************************************
   Returns whether the command will execute as a
   background job.
   
   POST: Returns true if the command is a background job.
         Returns false if it is not.
*/
bool Command::isBackgroundJob() const {
   return background_job;
}


/******************************************************
   Returns whether the command is part of a larger
   piped command object.
   
   POST: Returns true if the command will be piped.
         Returns false if it won't be.
*/
bool Command::isPipedJob() const {
   return piped_job;
}

/******************************************************
   Takes the command text variable and parses it
   into a command and its arguments by words.
   
   PRE:  command_text has been set.
   
   POST: A boolean is returned based on whether the
         command was parsed correctly or not. If not,
         the member variable error_reason is set to
         the reason for the error. If the command was
         parsed correctly, all of the data will be set
         correctly and the member vector cmd_arguments
         will contain the arguments of the command.
*/
bool Command::parseCommandText() {
   
   if (command_text == "") {
      error_reason = "Empty command.";
      return false;
   }
   
   // check for unsupported features
   string unsupported = checkForUnsupportedFeatures();
   if (unsupported != "none") {
      cout << "Warning:" << endl;
      cout << "  Unsupported feature: " << unsupported << endl;
   }
   
   // always start at the beginning of the string!
   int currentPos = 0;
   
   // ignore leading spaces
   currentPos = parseLeadingSpaces(currentPos);
   
   //check to make sure there's a command left
   if (currentPos == command_text.size()) {
      error_reason = "Empty command.";
      return false;
   }
   
   currentPos = parseCmdString(currentPos);
   
   // keep parsing until the end of the string, unless error
   while (currentPos < command_text.size()) {
       
      if (command_text[currentPos] == ' ') { // skip space
         currentPos++;
         
      } else if (command_text[currentPos] == '<') { // redirect input
   
         // check for piping, redirection is not allowed in a piped job
         if (piped_job) {
            error_reason = "Redirection is not supported for piped commands.";
            return false;
         }
         
         // check for multiple redirect statements
         if (input_redirect) {
            error_reason = "Too many input redirects.";
            return false;
         }
         
         input_redirect = true;
         currentPos = parseInputFileString(currentPos);
   
      } else if (command_text[currentPos] == '>') { // redirect output
   
         // check for piping, redirection is not allowed in a piped job
         if (piped_job) {
            error_reason = "Redirection is not supported for piped commands.";
            return false;
         }
         
         // check for multiple redirect statements
         if (output_redirect) {
            error_reason = "Too many output redirects.";
            return false;
         }
         
         output_redirect = true;
         currentPos = parseOutputFileString(currentPos);
   
      } else if (command_text[currentPos] == '&') { // background job
            
         // check for piping, pipes can't be executed in a background job
         if (piped_job) {
            error_reason = "Piped jobs cannot be run in the background.";
            return false;
         }
         
         background_job = true;
         currentPos++;
   
      } else { // surely must be an argument to parse
         
         currentPos = parseArgString(currentPos);
      }
   }
   
   return true;
}

/******************************************************
   Resets the entire object to its original state,
   as if it had just been constructed.
   
   POST: Object is reset.
*/
void Command::resetCommand() {
   
   // set flags
   input_redirect = false;
   output_redirect = false;
   background_job = false;
   piped_job = false;
   
   // init data to "none"
   command_text = "none";
   cmd_name = "none";
   input_file = "none";
   output_file = "none";
   error_reason = "none";
   
   // clear vector
   cmd_arguments.clear();
}

/******************************************************
   Check to see if the user is using unsupported
   features. These include:
      - single and double quotes
      - the '*' operator
      - the '?' operator
      - the '~' operator
      - escape characters
   
   PRE:   The string 'command_text' has been set.
   
   POST:  Returns a string containing the name of
          the unsupported feature.  If none found,
          returns "none".
           
*/
string Command::checkForUnsupportedFeatures() {
   
   int currentPos = 0;
   string unsupported = "none";
   
   while (currentPos < command_text.size()) {
    
      // double quotes
      if (command_text[currentPos] == (char) 0x22)
         unsupported = "quoted arguments";
      
      // single quotes
      else if (command_text[currentPos] == (char) 0x27)
         unsupported = "quoted arguments";
         
      // * operator
      else if (command_text[currentPos] == (char) 0x2A)
         unsupported = "wildcard characters";
         
      // ? operator
      else if (command_text[currentPos] == (char) 0x3F)
         unsupported = "wildcard characters";
         
      // ~ operator
      else if (command_text[currentPos] == (char) 0x7E)
         unsupported = "tilde for home directory";
         
      // escaped characters '\'
      else if (command_text[currentPos] == (char) 0x5C)
         unsupported = "escaped characters";
      
      currentPos++;
   }
      
   return unsupported;
}

/******************************************************
   Returns whether or not the char at position
   'currentPos' is a word separator. Words are separated
   by the following:
      - ' ' (space)
      - '>' (output redirect)
      - '<' (input redirect)
      - '&' (backgorund job)
   
   PRE:  The integer currentPos is the next char in
         command_text to be read.
   
   POST:  Returns true if the char at position currentPos
          is a word separator.
*/
bool Command::isSep(int currentPos) {
   
   if (command_text[currentPos] == ' ') {
      return true;
   } else if (command_text[currentPos] == '>') {
      return true;
   } else if (command_text[currentPos] == '<') {
      return true;
   } else if (command_text[currentPos] == '&') {
      return true;
   }
   
   // still here, must not be a separator
   return false;
}

/******************************************************
   Parses an entire command word, then skips the trailing
   space.
   
   PRE:  The integer 'currentPos' is the next char in
         the string 'command_text' to be read.
   
   POST: The command name is stored in the string 'cmd_name'.
         Return value is next char in 'command_text' to be read.
*/
int Command::parseCmdString(int currentPos) {
   
   cmd_name = "";
  
   // everything up to space is the cmd name
   while ((currentPos < command_text.size()) 
          && 
          (!isSep(currentPos))) 
   {         
      cmd_name += command_text[currentPos];
      currentPos++;
   }
   
   //return location of char directly after command word
   return currentPos;
}

/******************************************************
   Parses an entire command word, then skips the trailing
   space.
   
   PRE:  The integer 'currentPos' is the next char in
         the string 'command_text' to be read.
   
   POST: The argument word is stored in the next available
         space in the arguments vector.
         Return value is next char in 'command_text' to be read.
*/
int Command::parseArgString(int currentPos){
   
   bool firstChar = true;
   int currentArg;
   
   // everything up to separator char is the argument word
   while ((currentPos < command_text.size()) 
          && 
          (!isSep(currentPos)))
   {
      if (firstChar) {
         
         firstChar = false;
         
         // push a blank string onto the vector so we can
         // add to the end of it
         cmd_arguments.push_back("");
         
         // get address of newest arg
         currentArg = cmd_arguments.size() - 1;
      }
            
      cmd_arguments[currentArg] += command_text[currentPos];
      currentPos++;
   }
   
   // return location of char directly after argument word
   return currentPos;
}

/******************************************************
   Parses a redirect input command, from the beginning
   '<' char all the way to skipping the trailing space
   
   PRE:  The integer currentPos is the next char in
         command_text to be read, the '<' char.
   
   POST: The input file name is stored in the string 'input_file'.
         Return value is next char in the string 'command_text'
         to be read.
*/
int Command::parseInputFileString(int currentPos) {
   
   input_file = "";
   
   // skip past < char
   currentPos++;
   
   // get to file name
   currentPos = parseLeadingSpaces(currentPos);
  
   // everything up to separator char is the file name
   while ((currentPos < command_text.size()) 
          && 
          (!isSep(currentPos))) 
   {         
      input_file += command_text[currentPos];
      currentPos++;
   }
   
   // return location of char directly after filename
   return currentPos;
}

/******************************************************
   Parses a redirect output command, from the beginning
   '>' char all the way to skipping the trailing space
   
   PRE:  The integer currentPos is the next char in
         command_text to be read, the '>' char.
   
   POST: The output file name is stored in the string 'output_file'.
         Return value is next char in the string 'command_text'
         to be read.
*/
int Command::parseOutputFileString(int currentPos) {
   
   output_file = "";
   
   // skip past > char
   currentPos++;
   
   // get to file name
   currentPos = parseLeadingSpaces(currentPos);
  
   // everything up to separator char is the file name
   while ((currentPos < command_text.size()) 
          && 
          (!isSep(currentPos))) 
   {         
      output_file += command_text[currentPos];
      currentPos++;
   }
   
   // return location of char directly after filename
   return currentPos;
}

/******************************************************
   Removes the leading spaces from the string 'command_text'
   starting from currentPos
   
   PRE:  The integer currentPos is the next char in
         command_text to be read.
   
   POST:  The value of currentPos is the next char that
          isn't a space. 
*/
int Command::parseLeadingSpaces(int currentPos) {
   
   while (command_text[currentPos] == ' ')
      currentPos++;
      
   return currentPos;
   
}

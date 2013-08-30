/* file: Command.h
   
   Command Class Header File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the data and functions
   needed for storing and parsing command lines.
   
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on constructors:                   !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   Command()
   --------------------------------------------------
      This is the basic constructor for the class.
      
      POST: The object has been initialized.
      
      
   Command(string pipe_me)
   --------------------------------------------------
      This is the constructor for commands that are part
      of a larger piped job. 
      
      PRE:  pipe_me is a string.
      
      POST: The object has been initialized.
            If the passed string pipe_me equal "pipe",
            then the command will be set up to belong
            to a larger PipedCommand object.
            
            
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on public methods:                 !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   
   void setCommandText(string new_cmd_text)
   --------------------------------------------------
      Sets the command text of this object.
   
      PRE:  new_cmd_text is a string containing the text
            of a command line.
         
      POST: new_cmd_text is now the value of this object's
            command text.
   
   
   string getCommandText() const
   --------------------------------------------------
      Returns the command text of this object.
   
      POST: A string containing the text of the command
            line is returned.
    
      
   string getErrorReason() const
   --------------------------------------------------
      Returns the reason the command couldn't be parsed.

      POST: If command couldn't be parsed, returns a string
            containing the reason. If command was parsed
            successfully or has not been parsed at all,
            returns "none".
      
      
   string getCommandName() const
   --------------------------------------------------
      Returns the name of the executable for the command.

      POST: Returns the name of the executable. Returns
            "none" if the command hasn't been parsed.
      
      
   string getInputFileName() const
   --------------------------------------------------
      Returns the name of the file that input should
      be redirected to.

      POST: Returns the filename. If the command has not
            been parsed or is not input redirected,
            "none" is returned.
      
      
   string getOutputFileName() const
   --------------------------------------------------
      Returns the name of the file that output should
      be redirected to.

      POST: Returns the filename. If the command has not
            been parsed or is not output redirected,
            "none" is returned.
      
      
   vector<string> getArgs() const
   --------------------------------------------------
      Returns the vector that contains all of the command
      arguments.
   
      POST: Returns the vector. If the command has
            not been parsed or the command has no
            arguments, an empty vector is returned.
      
      
   char ** getArgsArray()
   --------------------------------------------------
      Returns a pointer to an array of pointers to null
      terminated character arrays containing the command
      arguments. This data structure is appropriate for
      use with the execv() system call.
      
      PRE:  The command has been parsed.
      
      POST: The pointer to the dynamically allocated
            array is returned.
   
   
   bool isInputRedirected() const
   --------------------------------------------------
      Returns whether the command will have its input
      redirected to a file instead of std input.
      
      POST: Returns true if the input is redirected.
            Returns false if it is not.
      
      
   bool isOutputRedirected() const
   --------------------------------------------------
      Returns whether the command will have its output
      redirected to a file instead of std out.
      
      POST: Returns true if the output is redirected.
            Returns false if it is not.
     
      
   bool isBackgroundJob() const
   --------------------------------------------------
      Returns whether the command will execute as a
      background job.
   
      POST: Returns true if the command is a background
            job. Returns false if it is not.
     
      
   bool isPipedJob() const
   --------------------------------------------------
      Returns whether the command is part of a larger
      PipedCommand object.
   
      POST: Returns true if the command will be piped.
            Returns false if it won't be.
   
   
   bool parseCommandText()
   --------------------------------------------------
      Takes the command line text of this object and
      parses it into an executable word, arguments,
      and redirects based on separator chars.
   
      PRE:  The command line text of this object has
            been set.
   
      POST: A boolean is returned based on whether the
            command was parsed correctly or not. If false
            is returned, the reason is stored by this
            object. If the command was parsed correctly
            and true is returned, all of the data
            will be set correctly and available using
            the public "get" commands.
    
      
   void resetCommand()
   --------------------------------------------------
      Resets the entire object to its original state,
      as if it had just been constructed using the
      basic constructor.
   
      POST: This object is reset.
      
*/

#ifndef CMD_HEADER
#define CMD_HEADER

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Command {
   
    public:
    
         // constructors
         Command();
         Command(string pipe_me);
         
         // set functions
         void setCommandText(string new_cmd_text);
         
         // get functions
         string getCommandText() const;
         string getErrorReason() const;
         string getCommandName() const;
         string getInputFileName() const;
         string getOutputFileName() const;
         vector<string> getArgs() const;
         char ** getArgsArray();
         
         // bool functions that return special command options
         bool isInputRedirected() const;
         bool isOutputRedirected() const;
         bool isBackgroundJob() const;
         bool isPipedJob() const;
         
         // other functions
         bool parseCommandText();
         void resetCommand();
         
    
    private:
    
         // checkers
         string checkForUnsupportedFeatures();
         bool isSep(int currentPos);
         
         // word parsing functions
         int parseCmdString(int currentPos);
         int parseArgString(int currentPos);
         int parseInputFileString(int currentPos);
         int parseOutputFileString(int currentPos);
         int parseLeadingSpaces(int currentPos);
    
         //------------------------------------------------------------
         // Data
         //------------------------------------------------------------
         
         // special command option flags (input/output redirect & pipes)
         bool input_redirect;
         bool output_redirect;
         bool background_job;
         bool piped_job;
         
         // text of entire cmd line
         string command_text;
         
         // parsed command data
         string cmd_name;
         string input_file;
         string output_file;
         string error_reason;
         
         // space for arguments
         vector<string> cmd_arguments;
};

#endif

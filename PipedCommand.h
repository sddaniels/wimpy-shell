/* file: PipedCommand.cpp
   
   Piped Command Class Header File
   ==================================================
   Wimpy Shell Project - Com Sci 342
   Shea Daniels
   
   This class is based around the data and functions
   needed for storing and parsing piped commands.
   
   
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on constructors:                   !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   PipedCommand()
   --------------------------------------------------
      This is the basic constructor for the class.
   
      POST: The object has been initialized.
   
      
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ! Information on public methods:                 !
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   void setCommandText(string new_cmd_text)
   --------------------------------------------------
      Sets the text of the entire command to whatever
      string the user specifies.
   
      PRE:  new_cmd_text is a string containing the text
            of the command line.
         
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
   
   
   vector<Command> getCommands() const
   --------------------------------------------------
      Returns the vector of sub commands that are
      all part of this larger object.
      
      POST: Returns a vector of Command objects.
   

   bool checkForPiping()
   --------------------------------------------------
      Checks to see if the command is a piped command
      or if this object isn't needed. (looks for '|' chars)
         
      PRE:  The command text of this object has been set.
      
      POST: Returns true if the command is piped.
            Returns false if it is not.
   
   
   bool parsePipedCommand()
   --------------------------------------------------
      Tries to parse the piped command into separate
      sub commands and then parse those commands.
         
      PRE:  The command text of this object has been set
            and is known to be a command that is piped.
      
      POST: A boolean is returned based on whether the
            command was parsed correctly or not. If false
            is returned, the reason is stored by this
            object. If the command was parsed correctly
            and true is returned, all of the data
            will be set correctly and available using
            the public "get" commands.
         
*/

#ifndef PIPECMD_HEADER
#define PIPECMD_HEADER

#include <string>
#include <vector>
#include <iostream>
#include "Command.h"

using namespace std;

class PipedCommand {
   
    public:
    
         // constructor
         PipedCommand();
      
         // set functions
         void setCommandText(string new_cmd_text);
         
         // get functions
         string getCommandText() const;
         string getErrorReason() const;
         vector<Command> getCommands() const;
         
         // other functions
         bool checkForPiping();
         bool parsePipedCommand();
    
    private:
    
         //------------------------------------------------------------
         // Data
         //------------------------------------------------------------
         
         string command_text;
         string error_reason;
         
         // list of commands to be piped
         vector<Command> cmds;
    
};

#endif

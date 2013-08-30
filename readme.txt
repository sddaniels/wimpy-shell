file: readme.txt 
 
 _      _                                  ___
  |  |  /    ,                            /    )   /           /   /
  | /| /        _  _      __              \       /__    __   /   /
  |/ |/    /   / /  )   /   ) /   /        \     /   ) /___) /   /
__/  |____/___/ /  /___/___/_(___/_____(____/___/   /_(___ _/___/___
                      /         /
                     /      (_ / 
                     
Wimpy Shell (wsh) version 1.0 by Shea Daniels
=====================================================================

Welcome Professors, TA's, and all others who are interested in Wimpy
Shell! I hope you enjoy grading this finely crafted piece of code.

This readme file will get you started on the various files and
classes that make up the entirety of Wimpy Shell. Documentation for
the users of classes is included in the header files of those classes.
More detailed and implementation specific comments are included in
the actual implementation files (.cpp files).

Enjoy,
   Shea Daniels

=====================================================================


Help and Compilation
--------------------------------------------------
   Files:
      readme.txt
      Makefile
      
   Description:
      The file Makefile contains the compiling dependencies
      and commands needed for the UNIX make utility. Running
      the command "make" should compile Wimpy Shell. As for
      the file readme.txt, you're reading it right now.
      
      
Executable
--------------------------------------------------
   Files:
      wsh
      
   Description:
      When Wimpy Shell is compiled, the executable file will
      be named "wsh".


Program Entry Point
--------------------------------------------------
   Files: 
      main.cpp
   
   Description:
      This code contains the main method and simply creates an
      instance of Wimpy Shell and then starts the main control
      loop.
      
      
WimpyShell Class
--------------------------------------------------
   Files:
      wimpyshell.h
      wimpyshell.cpp
      
   Description:
      This class represents the entire Wimpy Shell. An
      instance of this class is essentially an entire
      shell. It's only public method is to start
      the main control loop of the shell.
      
      NOTE: There is a maximum limit on the number of
            characters that are supported for the current
            working directory (CWD). Extremely nested
            folders will probably break Wimpy Shell.
      
      
Command Class
--------------------------------------------------
   Files:
      Command.h
      Command.cpp
      
   Description:
      This class is based around the data and functions
      needed for storing and parsing command lines.
      
      
PipedCommand Class
--------------------------------------------------
   Files:
      PipedCommand.h
      PipedCommand.cpp
      
   Description:
      This class is based around the data and functions
      needed for storing and parsing piped commands. This
      class uses the Command class to store the sub commands
      in the pipe. It also has the capability to check
      whether a command is piped and needs to use this class.
      
      
ForeJob Class
--------------------------------------------------
   Files:
      ForeJob.h
      ForeJob.cpp
      
   Description:
      This class is based around the code needed to redirect,
      execute, and wait for a foreground job.
      
      
BackJob Class
--------------------------------------------------
   Files:
      BackJob.h
      BackJob.cpp
      
   Description:
      This class is based around the code needed to redirect,
      execute, check the status of, and wait for a background
      job.
      
      
JobManager Class
--------------------------------------------------
   Files:
      JobManager.h
      JobManager.cpp
      
   Description:
      This class manages and keeps track of all the background
      jobs that are being run. It is in charge of starting,
      managing the status of, and waiting for all of the
      background jobs spawned by the shell. Each background
      job is represented by an instance of the BackJob class.
      
      
PipeManager Class
--------------------------------------------------
   Files:
      PipeManager.h
      PipeManager.cpp
      
   Description:
      This class is based around the code needed to execute a
      series of piped commands. It handles creation and
      redirection to pipes of an instance of the PipedCommand
      class.
      
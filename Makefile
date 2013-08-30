wsh: main.o wimpyshell.o Command.o PipedCommand.o JobManager.o PipeManager.o ForeJob.o BackJob.o
	g++ -o wsh main.o wimpyshell.o Command.o PipedCommand.o JobManager.o PipeManager.o ForeJob.o BackJob.o

main.o: main.cpp wimpyshell.h
	g++ -c main.cpp

wimpyshell.o: wimpyshell.cpp wimpyshell.h	Command.h PipedCommand.h JobManager.h PipeManager.h ForeJob.h BackJob.h
	g++ -c wimpyshell.cpp
	
Command.o: Command.cpp Command.h
	g++ -c Command.cpp
	
PipedCommand.o: PipedCommand.cpp	PipedCommand.h	Command.h
	g++ -c PipedCommand.cpp
	
JobManager.o: JobManager.cpp JobManager.h BackJob.h Command.h
	g++ -c JobManager.cpp
	
PipeManager.o: PipeManager.cpp PipeManager.h PipedCommand.h Command.h
	g++ -c PipeManager.cpp
	
ForeJob.o: ForeJob.cpp ForeJob.h	Command.h
	g++ -c ForeJob.cpp
	
BackJob.o: BackJob.cpp BackJob.h Command.h
	g++ -c BackJob.cpp

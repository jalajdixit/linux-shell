jshell: shellmain.o cmdexec.o history.o
	cc shellmain.o cmdexec.o history.o -o jshell


shellmain.o: shellmain.c
	cc -c shellmain.c
       
cmdexec.o: cmdexec.c
	cc -c cmdexec.c

history.o: history.c
	cc -c history.c

#Makefile for Jshell

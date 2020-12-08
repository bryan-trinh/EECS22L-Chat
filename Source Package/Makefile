#Makefile for ZotChess, EECS22L Winter 2019 
#Author: Team 5 (4OUR) 
#Date: 01/30/2019

BIN = ./bin
SOURCE = ./src
OBJ = ./obj

CC	= gcc
DEBUG	= -g -DDEBUG
CFLAGS	= -ansi -std=c99 -Wall -c 
LFLAGS	= -Wall

GTKINC	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --cflags gtk+-2.0`
GTKLIBS	= `PKG_CONFIG_PATH=/usr/share/pkgconfig pkg-config --libs gtk+-2.0`
INC		= $(GTKINC)
LIBS	= $(GTKLIBS)
OPTS	= -g -std=c99

OBJS	= $(OBJ)/Client.o $(OBJ)/CreateLoginAndVerify.o $(OBJ)/chess.o $(OBJ)/IsValid.o $(OBJ)/Initialize.o $(OBJ)/PrintBoard.o $(OBJ)/ReadInput.o $(OBJ)/Move.o $(OBJ)/Check.o $(OBJ)/AI.o $(OBJ)/GUISTUFF.o $(OBJ)/Logfile.o $(OBJ)/IsValidCheck.o

DEPS 	=  $(SOURCE)/CreateLoginAndVerify.h $(SOURCE)/chess.h $(SOURCE)/IsValid.h $(SOURCE)/Initialize.h $(SOURCE)/PrintBoard.h $(SOURCE)/ReadInput.h $(SOURCE)/Move.h $(SOURCE)/Check.h $(SOURCE)/GUISTUFF.h $(SOURCE)/AI.h $(SOURCE)/Logfile.h $(SOURCE)/IsValidCheck.h

DESIGN	= Client Server

#basic client and server
all: $(DESIGN)
test: AppendFriend
test-gui: LoginRegis

#test
####t#################################################################

AppendFriend:
	$(CC) $(SOURCE)/AppendFriending.c -Wall -std=c99 -o $(BIN)/AppendFriend

#test-gui
##########################################################################

LoginRegis: $(OBJ)/LoginRegis.o $(OBJ)/CreateLoginAndVerify.o
	$(CC) $(LFLAGS) $(OBJS)/LoginRegis.o $(OBJS)/CreateLoginAndVerify.o $(LIBS) $(OPTS) -o $(BIN)/LoginRegis

$(OBJ)/LoginRegis.o : $(SOURCE)/LoginRegis.c $(SOURCE)/CreateLoginAndVerify.h
	$(CC) $(CFLAGS) $(SOURCE)/LoginRegis.c $(INC) $(OPTS) -o $(OBJ)/LoginRegis.o

##########################################################################

$(OBJ)/Client.o : $(SOURCE)/Client.c $(DEPS)
	$(CC) $(CFLAGS) $(SOURCE)/Client.c $(INC) $(OPTS) -o $(OBJ)/Client.o

$(OBJ)/CreateLoginAndVerify.o: $(SOURCE)/CreateLoginAndVerify.c $(SOURCE)/CreateLoginAndVerify.h
	$(CC) $(CFLAGS) $(SOURCE)/CreateLoginAndVerify.c -o $(OBJ)/CreateLoginAndVerify.o

$(OBJ)/GUISTUFF.o: $(SOURCE)/GUISTUFF.c $(SOURCE)/GUISTUFF.h $(SOURCE)/IsValid.h $(SOURCE)/Move.h $(SOURCE)/Check.h $(SOURCE)/Constants.h $(SOURCE)/AI.h $(SOURCE)/Logfile.h $(SOURCE)/IsValidCheck.h
		$(CC) $(CFLAGS) $(SOURCE)/GUISTUFF.c -o $(OBJ)/GUISTUFF.o

$(OBJ)/AI.o: $(SOURCE)/AI.c $(SOURCE)/AI.h $(SOURCE)/Move.h
		$(CC) $(CFLAGS) $(SOURCE)/AI.c -o $(OBJ)/AI.o

$(OBJ)/Check.o: $(SOURCE)/Check.c $(SOURCE)/Check.h $(SOURCE)/IsValid.h
		$(CC) $(CFLAGS) $(SOURCE)/Check.c -o $(OBJ)/Check.o

$(OBJ)/IsValid.o: $(SOURCE)/IsValid.c $(SOURCE)/IsValid.h $(SOURCE)/Check.h
		$(CC) $(CFLAGS) $(SOURCE)/IsValid.c -o $(OBJ)/IsValid.o

$(OBJ)/IsValidCheck.o: $(SOURCE)/IsValidCheck.c $(SOURCE)/IsValidCheck.h
		$(CC) $(CFLAGS) $(SOURCE)/IsValidCheck.c -o $(OBJ)/IsValidCheck.o

$(OBJ)/Initialize.o: $(SOURCE)/Initialize.c $(SOURCE)/Initialize.h
		$(CC) $(CFLAGS) $(SOURCE)/Initialize.c -o $(OBJ)/Initialize.o

$(OBJ)/Logfile.o: $(SOURCE)/Logfile.c $(SOURCE)/Logfile.h $(SOURCE)/Constants.h
		$(CC) $(CFLAGS) $(SOURCE)/Logfile.c -o $(OBJ)/Logfile.o

$(OBJ)/PrintBoard.o: $(SOURCE)/PrintBoard.c $(SOURCE)/PrintBoard.h
		$(CC) $(CFLAGS) $(SOURCE)/PrintBoard.c -o $(OBJ)/PrintBoard.o

$(OBJ)/ReadInput.o: $(SOURCE)/ReadInput.c $(SOURCE)/ReadInput.h
		$(CC) $(CFLAGS) $(SOURCE)/ReadInput.c -o $(OBJ)/ReadInput.o

$(OBJ)/Move.o: $(SOURCE)/Move.c $(SOURCE)/Move.h
		$(CC) $(CFLAGS) $(SOURCE)/Move.c -o $(OBJ)/Move.o

$(OBJ)/chess.o: $(SOURCE)/chess.c $(SOURCE)/chess.h
	$(CC) $(CFLAGS) $(SOURCE)/chess.c $(INC) $(OPTS) -o $(OBJ)/chess.o
##########################################################################
Server:
	$(CC) $(SOURCE)/OnlineServer.c $(LFLAGS) -std=c99 -o $(BIN)/Server
##########################################################################
Client : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(LIBS) $(OPTS) -o $(BIN)/Client
##########################################################################
clean:
	rm -f $(OBJ)/*.o $(BIN)/Client $(BIN)/Server $(BIN)/AppendFriend  $(BIN)/LoginRegis Chat_V1.0_src.tar.gz
#tar file
tar:
	tar -cvf Chat_V1.0_src.tar.gz src bin obj doc Users images COPYRIGHT.txt INSTALL.txt Makefile README.md QUICKSTART.txt

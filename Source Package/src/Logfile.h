#ifndef LOGFILE_H
#define LOGFILE_H

#define MOV_SIZE 5 
#define INVALID -10
typedef struct Log Log;
struct Log {
	
	Log* Next;

	int Piece;
	int* Moves; /* with promotion */
	int Check;
	int IsValid; /*contains castling, capture */
	int SameFile; /*if two pieces can move to the same square saves the file */
	int SameRowFile; /*if two pieces can move to the same square and are from the same file */
};

/*log file */
typedef struct {
	
	Log* First;
	Log* Last;
	int Length;
	
} LogFile;

Log* CreateLog();
void DeleteLog(Log *l);
Log* CreateLogArguments (int piece, int* positions, int check, int isvalid, int samefile, int samerowfile);	//Creates a Log but with arguments
LogFile* CreateLogFile();
void DeleteLogFile(LogFile* lf);
void AppendLogFile(LogFile *lf, Log* l);
void DeleteLastLog(LogFile *lf);																			//create delete last log for undo move
void PrintLogs(LogFile *lf, int turns);
void SaveLogs(LogFile *lf, int board[8][8], int turn,int check,int valid,int* positions);
char* RewriteToString(int currIsValid, int currPiece, int currCheck, int currMoves[5], char * str);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "Logfile.h"
#include "Constants.h"
#include "Move.h"

Log* CreateLog() {
	
	Log* l = malloc(sizeof(*l));
	(l->Moves) = malloc( MOV_SIZE * sizeof(int) );
	l->Next = NULL;

	l->Piece = INVALID;
	l->Check = INVALID;
	l->SameFile = INVALID;
	l->SameRowFile = INVALID;
	
	return l;
}

void DeleteLog(Log *l) {
	assert(l);
	l->Next = NULL;

	l->Piece = INVALID;
	l->Check = INVALID;
	l->SameFile = INVALID;
	l->SameRowFile = INVALID;

	free(l->Moves);
	l->Moves = NULL;
	free(l);
}
/*Creates a Log but with arguments */
Log* CreateLogArguments (int piece, int* positions, int check, int isvalid, int samefile, int samerowfile) {
	int i; 
	
	Log *l = CreateLog();
	(l->Piece) = piece;
	(l->Check) = check;
	(l->IsValid) = isvalid;
	(l->SameFile) = samefile;
	(l->SameRowFile) = samerowfile;	
	for(i = 0; i < MOV_SIZE; i++) {
		l->Moves[i] = positions[i];
	}
	
	return l;
}

LogFile* CreateLogFile() {
	LogFile *lf = malloc(sizeof(*lf));
	lf->First = NULL;
	lf->Last = NULL;
	lf->Length = 0;
	
	
	return lf;
}

void DeleteLogFile(LogFile* lf) {
	int i;
	
	Log* curr = lf->First;
	Log* temp = NULL;
	for(i=0; i< (lf->Length); i++ ) {
		temp = curr -> Next;
		DeleteLog(curr);
		curr = temp;
		temp = NULL;
	}
	lf->First = NULL;
	lf->Last = NULL;
	lf->Length = 0;
	free(lf);
}

void AppendLogFile(LogFile *lf, Log* l) {
	if( (lf->Length) ==0 ) {
		lf->First = l;
		lf->Last = l;
	} else {
		Log* PrevLast = lf->Last;
		PrevLast->Next = l;
		lf->Last = l;
	}
	lf->Length = lf->Length + 1; 
}
/*create delete last log for undo move */
void DeleteLastLog(LogFile *lf) {
	assert(lf);
	int l = lf-> Length;
	if(l>2) {
		int i,j;
		j = l - 1;
		Log *prev = lf->First;
		for(i =1; i<j; i++ ){
			prev = prev->Next;
		}
		DeleteLog(lf->Last);
		prev->Next = NULL;
		lf->Last = prev;
		lf->Length = l--;
	} else if(l == 2 ) {
		DeleteLog(lf->Last);
		lf->First->Next = NULL;
		
		lf->Last = lf->First;
		lf->Length = l--;
	} else if(l == 1) {
		DeleteLog(lf->First);
		
		lf->First = NULL;
		lf->Last = NULL;
		
		lf->Length = l--;
	} else if(l == 0) {
		/*nothing happens */
	}
	/*check how many there are 
	change length
	change last and or first */ 
}

void PrintLogs(LogFile *lf, int turns) {
	/*also need to calculate turns */
	int turnstart = turns - (lf->Length);
	int movenumber = turnstart/2;
	int blackorwhite = (turnstart %2);
	

	FILE *fp;
	fp = fopen("Logs.txt" ,"a");
	if(fp == NULL) {
		printf("An error has occured. Could not open file! \n");
	}
	
	if(blackorwhite == 1) {
		/*black moved first */
		fprintf(fp, "\n \n%d. ...", movenumber);
	}
	
	int i;
	Log *curr = lf->First;
	while(i< (lf->Length)){
		char str[STR_LEN] = {0};
		
		int currPiece = curr->Piece;
		int currCheck = curr->Check;
		int currIsValid = curr->IsValid;
		int* currMoves = curr->Moves;
		
		
		memcpy(str, RewriteToString(currIsValid, currPiece, currCheck, currMoves, str), STR_LEN);
		if( blackorwhite ==1) {
			/*type blacks move*/
			fprintf(fp, "%s \n",str);
			/*make a new line */
			movenumber++;
			blackorwhite = 0;
		} else {
			/*type the move number thing with a space after the move */
			fprintf(fp, "%d. %s     ",(movenumber+1), str);
			blackorwhite = 1;
		}
		i++;
		curr = curr->Next;
	}
	printf("Files have been printed!\n");
	
	fclose(fp);
	
}
void SaveLogs(LogFile *lf, int board[8][8], int turn,int check,int valid,int* positions) {
	
	Coordinate initial = get_set_coordinate(positions[0],positions[1]);
	int piece = board[initial.y][initial.x];
	
	
	Log *new = CreateLogArguments(piece, positions, check, valid, 0,0);
	AppendLogFile(lf,new);
	char str[STR_LEN] = {0};
    memcpy(str, RewriteToString(valid, piece, check, positions, str), STR_LEN);
	if((turn%2) == 0) {
		printf("White Played:");
	} else {
		printf("Black Played:");
	}
	printf("%s \n",str);
}


char* RewriteToString(int currIsValid, int currPiece, int currCheck, int currMoves[5], char * str) {
    char strtemp[STR_LEN];
    if(currIsValid ==3) {
        strcpy(str, "o-o");
    } else if (currIsValid ==4) {
        strcpy(str, "o-o-o");
    } else {
        if((currPiece == WhiteR) || (currPiece ==BlackR)) {strcat(str, "R");}
        if((currPiece == WhiteN) || (currPiece ==BlackN)) {strcat(str, "N");}
        if((currPiece == WhiteB) || (currPiece ==BlackB)) {strcat(str, "B");}
        if((currPiece == WhiteK) || (currPiece ==BlackK)) {strcat(str, "K");}
        if((currPiece == WhiteQ) || (currPiece ==BlackQ)) {strcat(str, "Q");}
        if((currPiece == WhiteP) || (currPiece == WhiteP)) {
            if(currMoves[0] == A) {strcat(str, "a");}
            if(currMoves[0] == B) {strcat(str, "b");}
            if(currMoves[0] == C) {strcat(str, "c");}
            if(currMoves[0] == D) {strcat(str, "d");}
            if(currMoves[0] == E) {strcat(str, "e");}
            if(currMoves[0] == F) {strcat(str, "f");}
            if(currMoves[0] == G) {strcat(str, "g");}
            if(currMoves[0] == H) {strcat(str, "h");}
        }
        if(currIsValid == 2) {
            strcat(str, "x");
        }
            if(currMoves[2] == A) {strcat(str, "a");}
            if(currMoves[2] == B) {strcat(str, "b");}
            if(currMoves[2] == C) {strcat(str, "c");}
            if(currMoves[2] == D) {strcat(str, "d");}
            if(currMoves[2] == E) {strcat(str, "e");}
            if(currMoves[2] == F) {strcat(str, "f");}
            if(currMoves[2] == G) {strcat(str, "g");}
            if(currMoves[2] == H) {strcat(str, "h");}
            
            sprintf(strtemp, "%d", abs(currMoves[3] -8));
            strcat(str,strtemp);
        /*promotion */    
        if(currMoves[4] != -1) {
            if((currMoves[4] == WhiteQ) || (currMoves[4] == BlackQ)) {strcat(str,"=Q");}
            if((currMoves[4] == WhiteN) || (currMoves[4] == BlackN)) {strcat(str,"=N");}
            if((currMoves[4] == WhiteB) || (currMoves[4] == BlackB)) {strcat(str,"=B");}
            if((currMoves[4] == WhiteR) || (currMoves[4] == BlackR)) {strcat(str,"=R");}
        }
        
        if(currCheck == 1) {
            strcat(str,"+");
        } else if (currCheck == 2) {
            strcat(str, "#");
        }
    }
    return str;
}
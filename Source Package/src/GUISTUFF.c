#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "IsValid.h"
#include "Move.h"
#include "Check.h"
#include "GUISTUFF.h"
#include "Constants.h"
#include "AI.h"
#include "Logfile.h"
#include "IsValidCheck.h"

void GUIMove(int moves[], int board[8][8], int* turn, int*check, LogFile*lf) {
	int valid = IsValid(*turn, board, moves);
	if(*check == 1)
	{
		valid = IsValidCheck(*turn, board, moves);
	}
	if(valid >0 ) {
	/*  if valid then move */
		Move(moves, board);
		*turn = *turn +1;
		*check = IsCheck(board, *turn);
		SaveLogs(lf,board,(*turn-1), *check, valid, moves);
	}

}

void AIMove(int board[8][8], int *turn, int *check, LogFile *lf){
    int curr_move[MOV_SIZE] = {0,0,0,0,-1}; curr_move[0] = 1; curr_move[0] = 0;
    int *positions = NULL; /*points to array of curr_move */
    int valid = 0;
	while(valid == 0) 
		{
            //PrintBoard(board);
			positions = FirstAI(board,*turn);
			//printf("AI suggestion: from x=%d, y=%d, to x=%d,y=%d\n",positions[0],positions[1],positions[2],positions[3]);
			//positions = ReadInput(*turn, board, curr_move);
			valid =  IsValid(*turn,board,positions);
			if(*check == 1)
			{
				valid = IsValidCheck(*turn, board, curr_move);
			}
            if(valid == 2)
			{
                printf("Castling\n\n");
            }
            else if(valid == 3)
			{
                printf("En Passant\n\n");
            }

        }
        /*check = isCheck */
        Move(positions,board);
        /* display increment turns, check flags, prev move update, update logs etc */
        *turn = *turn + 1;
		*check = IsCheck(board, *turn);
		SaveLogs(lf,board,(*turn-1), *check, valid, curr_move);
}
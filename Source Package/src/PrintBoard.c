#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> 
#include "PrintBoard.h"

void PrintBoard(int board[8][8]) {

    int i;
    int j;
    for (i =0; i <8; i ++ ) {
        printf ("+----+----+----+----+----+----+----+----+\n" );
        for(j = 0; j < 8; j ++ ) {
            if(j == 0 ) { printf("|"); }
            if(board[i][j] == 0) {
                printf("    ");
            } else if(board[i][j] == 1 ) {
                printf(" wP ");
            } else if(board[i][j] == 2 ) {
                printf(" wR ");
            } else if(board[i][j] ==  3 ) {
                printf(" wN ");
            } else if(board[i][j] ==  4 ) {
                printf(" wB ");
            } else if(board[i][j] ==  5 ) {
                printf(" wK ");
            } else if(board[i][j] ==  6 ) {
                printf(" wQ ");
            } else if(board[i][j] ==  7 ) {
                printf(" bP ");
            } else if(board[i][j] ==  8 ) {
                printf(" bR ");
            } else if(board[i][j] ==  9 ) {
                printf(" bN ");
            } else if(board[i][j] ==  10 ) {
                printf(" bB ");
            } else if(board[i][j] ==  11 ) {
                printf(" bK ");
            } else if(board[i][j] ==  12 ) {
                printf(" bQ ");
            }
            printf("|");
            if(j == (8 - 1)) {
                printf("   %d  ", abs(i-8));
            }
        }
        printf("\n");
    }
    printf ("+----+----+----+----+----+----+----+----+\n" );
    printf ("   A    B    C    D    E    F    G    H  \n" );
}

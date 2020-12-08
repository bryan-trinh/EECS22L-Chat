#include <stdio.h>
#include <stdlib.h>
#ifndef CHECK_H
#define CHECK_H

int IsCheck(int board[8][8], int turn);/*THe function will return 0 if not checking, return 1 if checking, and return 2 if checkmate */
int whitecheck(int board[8][8],int KingPosi_x, int KingPosi_y);
int blackcheck(int board[8][8],int KingPosi_x, int KingPosi_y);


#endif

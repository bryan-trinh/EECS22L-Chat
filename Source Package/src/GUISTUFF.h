#ifndef GUISTUFF_H
#define GUISTUFF_H
#include "Logfile.h"
#include "Constants.h"
void GUIMove(int moves[], int board[8][8], int* turn, int*check, LogFile *lf);
void AIMove(int board[8][8], int *turn, int *check, LogFile *lf);

#endif 
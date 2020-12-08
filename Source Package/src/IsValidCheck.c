#include "IsValidCheck.h"
#include "Check.h"
#include "IsValid.h"
#include "Move.h"
#include <stdio.h>

int IsValidCheck(int turn, int board[8][8], int *positions)
{
	int check = 0; 

	Coordinate initial = get_set_coordinate(positions[0],positions[1]);
    	Coordinate final = get_set_coordinate(positions[2],positions[3]);
    	int piece = board[initial.y][initial.x];

	int tboard[8][8]; 
	for(int i =0; i<8;i++){
	for(int j=0;j<8;j++){ 
		tboard[i][j] = board[i][j]; 
	}}
	tboard[final.y][final.x] = piece; 	
	tboard[initial.y][initial.x] = 0; 
	check = IsCheck(tboard,turn++);  
    if (check==1){
		return 0; 
	}
	return 1; 
	
}

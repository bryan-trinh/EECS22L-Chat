#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "AI.h"
#include "Move.h"
#include "IsValid.h"


int *FirstAI(int board[8][8], int turn){
        static int result[5];//what should return
        int i = 0;
        int j = 0;
        int k = 0;
        int l = 0;//Ix Iy Fx Fy

//        Coordinate initial = get_set_coordinate(positions[0],positions[1]);
//        Coordinate final = get_set_coordinate(positions[2],positions[3]);

        //following clears the result array
        for(i=0; i<5; i++){
                result[i] = 0;
        }

        bool ValidMove = false;

        if( turn % 2 == 0){ //white turn
                for(i= 0; i < 8; i++){
                        for(j= 0; j < 8; j++){
                                if(board[i][j] == 1 || board[i][j] == 2 || board[i][j] == 3 || board[i][j] == 4 ||board[i][j] == 5 ||board[i][j] == 6){
                                        for(k = 0; k < 8; k++){
                                                for(l = 0; l < 8; l++){
                                                        result[0] = j;
                                                        result[1] = i;
                                                        result[2] = l;
                                                        result[3] = k;
                                                        result[4] = -1;
                                                        if(IsValid(turn, board,result) == 0){
								
                                                                continue;
                                                        }
                                                        else{
							if(board[i][j] == 1 && k == 0){
								result[4] = 2;
							} 
							
                                                        ValidMove = true;
                                                        break;
                                                        }
                                                }
                                                if(ValidMove){
                                                        break;
                                                }
						else{
							continue;
						}
                                        }
                                        if(ValidMove){
                                                break;
                                        }
                                }
                                else if(ValidMove){
                                        break;
                                }
                                else{
                                        continue;
                                }
                        }
                        if(ValidMove){
                                break;
                        }
			else{
				continue;
			}
                }
        }

        if(turn % 2 == 1){//black turn
                for(i = 7; i > -1; i--){
                        for(j = 7 ; j > -1; j--){
                                if(board[i][j] == 7 || board[i][j] == 8 || board[i][j] == 9 || board[i][j] == 10 ||board[i][j] == 11 ||board[i][j] == 12){
                                        for(k = 7; k > -1; k--){
                                                for(l = 7; l > -1; l--){
                                                        result[0] = j;
                                                        result[1] = i;
                                                        result[2] = l;
                                                        result[3] = k;
                                                        result[4] = -1;
                                                        if(IsValid(turn, board, result) == 0){
								
                                                                continue;
                                                        }
                                                        else{
							if(board[i][j] == 7 && k == 7){
								result[4] = 2;
							} 

							
                                                        ValidMove = true;
                                                        break;
                                                        }
                                                }
                                                if(ValidMove){
                                                        break;
                                                }
						else{
							continue;
						}
                                        }
                                        if(ValidMove){
                                                break;
                                        }
                                }
                                else if(ValidMove){
                                        break;
                                }
                                else{
                                        continue;
                                }
                        }
                        if(ValidMove){
                                break;
                        }
			else{
				continue;
			}
                }
        }
        return result;
}


       

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Move.h"
#include "Initialize.h"
#include "Constants.h"

void Initialize(int board[8][8], int* game, int* turn){
	/*actual game creation */
	if(*game == 4 ) {
		char c;
		char buffer;
		int WorB, count =0 ;
		
		
		FILE *file;
		char fname[STR_LEN];
		/*read file and check .txt extension */
		printf("Please enter the text file you would like to read. Please do add the .txt extension. For example 'test.txt' \n");
		scanf(" %s%c" , fname, &buffer);
		if((fname[(int)strlen(fname) -1] != 't') || (fname[(int)strlen(fname) -2] != 'x') || (fname[(int)strlen(fname) -3] != 't') || (fname[(int)strlen(fname) -4] != '.')) { 
			*game = 0; 
			printf("Only supports .txt files\n "); 
		} 
	
		file = fopen(fname, "r");
	
	/*getting the turns */
		fscanf(file, "%d", turn);
		fscanf(file, "%d", &WorB);
		*turn = (*turn) *2 + WorB;
		printf("Turn: %d\n",(*turn));
	/*If file exists, then attempt to load the board and check all the valid values in the array */
	/*if file does not exist, then print out that file doesn't exist */
		if (file) {
			int r = 0, col=0;
			int temp = 0;
			int isUpper = 0;
			
			while ((c = getc(file)) != EOF) {
				if(count < LOAD_SIZE) { /*must be less than expected number of inputs */
					
					if(isUpper == 0) {
						if ((c != '\n') && (c != ' ') && (c!= '\r')) {
							temp = c - '0';
							isUpper = 1;
						}
					} else {
						if ((c != '\n') && (c != ' ') && (c!= '\r')) {
							board[r][col] = temp*10 + (c-'0');
							/*printf("%d ", temp*10 + (c-'0')); */
						} else {
							board[r][col] = temp;
							/*printf("%d ", temp); */ 
						}
						isUpper = 0; count++; col++;
						if(col ==8) {col =0; r++;}
						
					}
				}
			}
			if(isUpper == 1 ) {board[r][col] = temp; isUpper = 0 ; count ++;}
			/*double check that all the values are valid */
			for(r =0; r < 8; r++ ) { 
				for(col =0 ; col < 8; col++) {
					if((board[r][col] <0 ) || (board[r][col] > 12)) {
						printf("File loaded. However there are too few, too many , or values in \nthe text file are out of range.\n\n");
						printf("The first value should indicate the move in standard chess notation.\n");
						printf("The second number should be if it is white's or black's turn. \n0 or 1 respectively\n");
						printf("Please double check. There should be 64 values between the ranges of 0-12.\n\n");
						*game = 0;
						break;
					}
				}
				if(*game == 0) {break;}
			}
		} else {
			printf("File could not be read correctly. \n ");
			*game = 0;
		}



		fclose(file);
	}
}
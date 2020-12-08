#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum {A,B,C,D,E,F,G,H} E_XPosition;
typedef enum {R8,R7,R6,R5,R4,R3,R2,R1} E_YPosition;

typedef struct {
	E_XPosition x;
	E_YPosition y;
}Coordinate ;

Coordinate get_set_coordinate(int xpos, int ypos);
Coordinate get_set_coordinate(int xpos, int ypos) {
	Coordinate new;
	new.y = ypos;
	new.x = xpos;
	return new;
}

typedef enum { WhiteP=1, WhiteR, WhiteN, WhiteB, WhiteK, WhiteQ, BlackP, BlackR, BlackN, BlackB, BlackK, BlackQ } E_PieceType;
/* 		          1     , 2 ,     3,       4,     5,      6,      7,      8 ,     9,     10,    11,     12 */
#define PIECEOFFSET 6

typedef struct {
	E_PieceType type; 
	Coordinate coord;
	
	
}Piece;

Coordinate get_coordinate(Piece p);
void set_coordinate(Piece *p, int xpos, int ypos);
void set_piece_and_coordinate(Piece *p, int piece_value, int xpos, int ypos);


Coordinate get_coordinate(Piece p) {
	return p.coord;
}

void set_coordinate(Piece *p, int xpos, int ypos) {
	(*p).coord.y = ypos;
	(*p).coord.x = xpos;
}
void set_piece_and_coordinate(Piece *p, int piece_value, int xpos, int ypos) {
    (*p).type = piece_value;
    set_coordinate(p,xpos,ypos);
    
}


/* define class constants and global var */
#define STR_LEN 64
#define MOV_SIZE 5
#define LOAD_SIZE 64
/*define func */


/*initializes the board */
void Initialize( int board[8][8], int* game, int *turn); 

/*prints the 2d array */
void PrintBoard(int board[8][8]);

/*reads input and returns a pointer with the initial, final position, and promotion */
int* ReadInput(int turn, int board[8][8], int curr_move[MOV_SIZE]);

/*Moves the piece */
void Move(int*positions, int board[8][8]);

/*Checks if Move isValid, if it is then make the move, if not then print invalid move, 
print board and have them try again with input */
/*IsValid */

/*Is the King in Check after the move is valid, return a flag with check or not in check or checkmate */
/*IsCheck */ 

/*  Updates the display features: turn, flags, previous move, logs */
/*int Update(int turn, int prev_move[MOV_SIZE], int curr_move[MOV_SIZE], int check); */
/*main */

int main() {
	/*local variables */
	int game = 1;
	int board[8][8];	
	int curr_move[MOV_SIZE] = {0,0,0,0,0};
	int *positions; /*points to array of curr_move */
	int turn = 0; /*0 means white and 1 means black */
	int check = 0; 	check++; check--;
	int valid = 0;

	
	/*Create a New Game */
	Initialize(board, &game, &turn);
	
	printf("This the the numbe	r of turns taken place %d \n" , turn);
	
	
	while (game >0) {
		while(valid ==0 ) {
		    PrintBoard(board);
		    positions = ReadInput(turn, board, curr_move);
		    /*valid =  isValid */
			valid = 1;
		}
		/*check = isCheck */ 
		Move(positions,board);
		/* display increment turns, check flags, prev move update, update logs etc */
		turn++;
		
		valid = 0;
	}
	
/*	
Potential Game Logic
	while (true) {
		print board
		read input (careful for promotion, nbd7, and captures)  
		check valid (check castling, pins, and enpassant)
		isCheck? 
		make move
		record move (and add + if necessary) 
	
	}
	
*/


	return 0;
} 
 
void Initialize(int board[8][8], int* game, int* turn){
	char c;
	printf("Print game mode \n");
	printf("1- Human vs Human \n");
	printf("2- Human Vs Computer \n");
	printf("3- Computer Vs Computer \n");
	printf("4- Load Game \n");
	
	scanf(" %d%c" , game, &c);

	printf("%d is the game chosen \n", *game);
	
	
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
		printf("%d \n",*turn);
	/*If file exists, then attempt to load the board and check all the valid values in the array */
	/*if file does not exist, then print out that file doesn't exist */
		if (file) {
			int r = 0, col=0;
			
			while ((c = getc(file)) != EOF) {
				if(count < LOAD_SIZE) { /*must be less than expected number of inputs */
					if ((c != '\n') && (c != ' ') && (c!= '\r')) {
						board[r][col] = c - '0';
						/* printf("%d ",board[r][col]); */
						count++; col++;
						if(col ==8) {col =0; r++;}
					}
					
				}
			}
			
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
		printf("Game Mode 4 \n");
	} else {
		int tempBoard[8][8] = {
			{8,9,10,12,11,10,9,8}, 
			{7,7,7,7,7,7,7,7},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1},
			{2,3,4,6,5,4,3,2}
			};	
			
		int r, c;
		
		for (r = 0; r < 8; r++) {
			for (c = 0; c < 8; c++) {
				board[r][c] = tempBoard[r][c];
				/*8 is cols */ 
				printf("%d ", board[r][c]);
			}
			
			printf("\n");
		}
		 
	}
	
	
}

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
int* ReadInput(int turn, int board[8][8], int curr_move[MOV_SIZE]) {
	if(turn %2 ==0 ) {
		printf("White's turn to move: \n");
	} else {
		printf("Black's turn to move: \n");
	}
    char str[STR_LEN];
    char initial[STR_LEN/2];
    char final[STR_LEN/2];
    int i;
    char *pch;
	if( fgets (str, STR_LEN, stdin)!=NULL ) {
	    pch = strtok (str," ");
	    for(i =0; i < 2; i++) {
	        if(i ==0) {
	            strncpy(initial,pch , STR_LEN/2);
	        } else if (i ==1) {
	           strncpy(final, pch, STR_LEN/2);
	        }
	        /*printf ("%s\n",pch); */
            pch = strtok (NULL, " ");
	    }
	
   }
   
   /*currently only reads e2 e4 and such, no pieces in front no promotion  */
   /*
         char* ptr;

      for (ptr = str; *ptr != '\0'; ptr++)
         *ptr = toupper(*ptr);
      }
   
   */
   /*movement conversion */
   initial[0] = tolower(initial[0]);
   final[0] = tolower(final[0]);
   E_XPosition init = (int)(initial[0] - '0') -48 -1;
   E_XPosition fin = (int)(final[0] - '0') -48 -1;
   int yinit = abs((int)(initial[1] - '0') - 8);
   int yfinal= abs((int)(final[1] - '0') - 8);
   /*int piece = board[yinit][init]; */
   
   curr_move[0] =  init;
   curr_move[1] = yinit;
   curr_move[2] = fin;
   curr_move[3] = yfinal;
   curr_move[4] = -1;
/*   
   printf( "%s", initial);
   printf( "%s", final);
   printf("%d %d %d \n ", yinit, yfinal, piece);
   printf("%d %d", init , fin);

 */ 
   return curr_move;
}

void Move ( int *positions, int board[8][8]) {
	Coordinate initial = get_set_coordinate(positions[0],positions[1]);
	Coordinate final = get_set_coordinate(positions[2],positions[3]);
/*	
	printf("%d %d", initial.x, initial.y);
	printf("%d %d", final.x, final.y);
*/
	
	int piece = board[initial.y][initial.x];
	*(&board[initial.y][initial.x]) = 0;
	*(&board[final.y][final.x]) = piece;
	/*different for castle promotion and enpassant */ 
}
/* logs */
/* saves as date
Piece Value that Moved, Initial,  Final Position, Capture, Castle, Promotion, Check/ CheckMate , More than 1 piece there ?  (what if same col ) */

/* 
board temporary initialization testing 
#include <stdio.h>
#include <string.h>
int board[8][8];
int main()
{
    int i,j;
    for(i=0; i<8; i++) {
        for(j=0; j<8; j++) {
            board[i][j] = i*8 +j;
            printf("%d", board[i][j]);
        }
        printf("\n");
    }


    return 0;
}

*/

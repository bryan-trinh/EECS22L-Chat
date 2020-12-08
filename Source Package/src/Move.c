#include "Move.h"
#include "Constants.h"
#include <stdio.h>
int EnPassant;
int BlackCastleKingSide; 
int BlackCastleQueenSide;
int WhiteCastleKingSide; 
int WhiteCastleQueenSide;  
Coordinate get_set_coordinate(int xpos, int ypos) {
    Coordinate new;
    new.y = ypos;
    new.x = xpos;
    return new;
}
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

void Move ( int *positions, int board[8][8]) {
    Coordinate initial = get_set_coordinate(positions[0],positions[1]);
    Coordinate final = get_set_coordinate(positions[2],positions[3]);
    /*
     printf("%d %d", initial.x, initial.y);
     printf("%d %d", final.x, final.y);
     */

    int piece = board[initial.y][initial.x];
	/* promotion */
	if( ((piece == WhiteP) && (final.y == 0)) || ((piece ==BlackP) && (final.y == 7)) ) {
		//piece = positions[4];
		positions[4] = 1;
	}

	if(positions[4] == 2)
                {
                        if(piece == WhiteP)
                        {
                                piece = 6;
                        }
                        else
                        {
                                piece = 12;
                        }
                }
                else{
                        positions[4] = 1;
                }

	
	
    *(&board[initial.y][initial.x]) = 0;
    *(&board[final.y][final.x]) = piece;
    /*different for castle promotion and enpassant */

    if(EnPassant == 1&&(final.y==2||final.y==5)&&(piece==WhiteP||piece==BlackP)){
	if (final.y == 2&&piece==WhiteP){
	board[final.y+1][final.x] = 0;
	EnPassant = 0; 
	} 
	else if (final.y == 5&&piece==BlackP){
	board[final.y-1][final.x] = 0; 
	EnPassant = 0; 
	}
    }

   if(WhiteCastleKingSide == 1&&final.y==7&&final.x==6&&piece==WhiteK){
	board[7][7] = 0; 
	board[7][5] = WhiteR; 
	WhiteCastleKingSide = 0; 
   }
   if(WhiteCastleQueenSide == 1&&final.y==7&&final.x==2&&piece==WhiteK){
	board[7][0] = 0; 
	board[7][5] = WhiteR; 
	WhiteCastleQueenSide = 0; 
   }
   if(BlackCastleKingSide == 1&&final.y==0&&final.x==6&&piece==BlackK){
	board[0][7] = 0; 
	board[0][5] = BlackR; 
	BlackCastleKingSide = 0; 
   }
   if(BlackCastleQueenSide== 1&&final.y==0&&final.x==2&&piece==BlackK){
	board[0][0] = 0; 
	board[0][3] = BlackR; 
	BlackCastleQueenSide = 0; 
   }

}

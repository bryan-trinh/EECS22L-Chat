#ifndef MOVE_H
#define MOVE_H

void Move ( int *positions, int board[8][8]);

typedef enum {A,B,C,D,E,F,G,H} E_XPosition;
typedef enum {R8,R7,R6,R5,R4,R3,R2,R1} E_YPosition;
typedef enum { WhiteP=1, WhiteR, WhiteN, WhiteB, WhiteK, WhiteQ, BlackP, BlackR, BlackN, BlackB, BlackK, BlackQ } E_PieceType;
/*                   1     , 2 ,     3,       4,     5,      6,      7,      8 ,     9,     10,    11,     12 */

typedef struct{
	E_XPosition x; 
	E_YPosition y; 
}Coordinate; 

typedef struct {
    E_PieceType type;
    Coordinate coord;
}Piece;



Coordinate get_set_coordinate(int xpos, int ypos);
Coordinate get_coordinate(Piece p);
void set_coordinate(Piece *p, int xpos, int ypos);
void set_piece_and_coordinate(Piece *p, int piece_value, int xpos, int ypos);
#endif

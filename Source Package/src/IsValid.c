#include "IsValid.h"
#include "Move.h"
#include "Check.h"
#include "Constants.h"
extern int EnPassant;
extern int BlackCastleKingSide; 
extern int BlackCastleQueenSide; 
extern int WhiteCastleKingSide; 
extern int WhiteCastleQueenSide;
 
int IsValid(int turn, int board[8][8] , int *positions)
{
    static int KingSBlackRookCounter = 0;  
    static int QueenSBlackRookCounter = 0; 
    static int KingSWhiteRookCounter = 0; 
    static int QueenSWhiteRookCounter = 0; 
    static int WhiteKingCounter = 0; 
    static int BlackKingCounter = 0;

    Coordinate initial = get_set_coordinate(positions[0],positions[1]);
    Coordinate final = get_set_coordinate(positions[2],positions[3]);
    int piece = board[initial.y][initial.x];

    if (final.x == initial.x && final.y == initial.y){
        return 0;
    }
    if (piece == 0){
        return 0;
    }

    if (turn %2 == 0){
        if (piece > 6){
            return 0;
        }
        else if ( board[final.y][final.x] < 7 && board[final.y][final.x] != 0){
            return 0;
        }
    }
    else if (turn %2 == 1){

        if (piece < 7){
            return 0;
        }

        else if (board[final.y][final.x] > 6 && board[final.y][final.x] != 0){
            return 0;
        }
    }
	
    /*White Pawn*/
    if(piece == 1){
	if (initial.y == 6 && final.y == initial.y - 1 && final.x == initial.x && board[final.y][final.x] == 0){
		if(EnPassant==1){
		 EnPassant = 0; 
		}
		return 1; 
	}
        /*starting move*/
         if(initial.y == 6 &&final.y==initial.y-2 && final.x == initial.x && board[final.y][final.x] == 0 && board[final.y+1][final.x]==0){
            if(initial.x!=0 && final.y == initial.y -2 && (board[final.y][final.x-1]==7||board[final.y][final.x+1]==7)&& EnPassant==0){
                EnPassant =1;
                return 1;
            }
            else if(initial.x == 0 && final.y == initial.y + 2 && board[final.y][final.x+1] == 7 && EnPassant == 0){
                EnPassant = 1;
                return 1;
            }
            else if(initial.x == 7 && final.y == initial.y + 2 && board[final.y][final.x-1] == 7 && EnPassant == 1){
                EnPassant =1;
                return 1;
            }
            else if (EnPassant == 1){
                EnPassant = 0;
                return 1;
            }
            return 1;
        }
        /*move up 1*/
        else if(initial.y != 6 && final.y == initial.y -1 && final.x == initial.x && board[final.y][final.x] == 0){
            if(EnPassant ==1){
                EnPassant = 0;
                return 1;
            }
            return 1;
        }
        /*regular capture*/
        else if (board[final.y][final.x] > 6 && (final.y == initial.y - 1) && (final.x==initial.x+1||final.x==initial.x-1)){
            if (EnPassant == 1){
                EnPassant =0;
                return 1;
            }
            return 1;
        }
        /*en passant*/
        else if(/*EnPassant == 1 &&*/ board[final.y][final.x] == 0 && board[final.y+1][final.x] == 7&&initial.y==3&&final.y==2){
            /*board[final.y+1][final.x] = 0;*/
            EnPassant = 1;
            return 5;
        }
        else{
            return 0;
        }
    }
    
    /*black pawn*/
    else if(piece == 7){
	if (initial.y == 1 && final.y == initial.y+1 && final.x==initial.x && board[final.y][final.x] == 0 ){
		if (EnPassant == 1)
		{ EnPassant =0; }
		return 1; 
	}
        /*starting move*/
        if (initial.y == 1 && final.y==initial.y+2  && final.x == initial.x && board[final.y][final.x] == 0 && board[final.y-1][final.x]==0){
            if (initial.x != 0 && final.y == initial.y + 2 && (board[final.y][final.x+1] ==1||board[final.y][final.x-1]==1) && EnPassant == 0){
                EnPassant = 1;
                return 1;
            }
            else if (initial.x == 0 && final.y == initial.y + 2 && board[final.y][final.x+1] == 1 && EnPassant == 0){
                EnPassant = 1;
                return 1;
            }
            else if(initial.x == 7 && final.y == initial.y + 2 && board[final.y][final.x-1] == 1 && EnPassant == 0){
                EnPassant = 1;
                return 1;
            }
            else if (EnPassant == 1){
                EnPassant = 0;
            }
            return 1;
        }
        /*move up 1*/
        else if (initial.y != 1 && final.y == initial.y+1 && final.x == initial.x && board[final.y][final.x]==0){
            if(EnPassant ==1 ){
                EnPassant = 0;
                return 1;
            }
            return 1;
        }
        /*regular capture*/
        else if (board[final.y][final.x] < 7&&board[final.y][final.x]!=0 &&(final.y == initial.y+1) &&(final.x==initial.x+1||final.x==initial.x-1)){
            if(EnPassant == 1){
                EnPassant = 0;
                return 1;
            }
            return 1;
        }
        /*En Passant*/
        else if(/*EnPassant == 1 && */board[final.y][final.x] == 0 && board[final.y-1][final.x] == 1&&initial.y==4&&final.y==5){
            EnPassant = 1;
            return 5;
        }
        else{
            return 0;
        }
    }

    /*White Rook*/
    else if ( piece == 2)
    {
        if (EnPassant == 1){
            EnPassant = 0;
        }

        if (initial.x == final.x || initial.y == final.y ){
            if (initial.x == final.x){
                if (final.y > initial.y){
                    for (int i = initial.y + 1; i < final.y; i++){
                        if ( board[i][initial.x] != 0){
                            return 0;
                        }
                    }
                }
                else
                {
                    for (int i = initial.y - 1; i > final.y; i--){
                        if ( board[i][initial.x] != 0){
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if (final.x > initial.x){
                    for (int i = initial.x + 1 ; i < final.x; i++){
                        if ( board[initial.y][i] != 0)
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    for (int i = initial.x -1; i > final.x; i--){
                        if ( board[initial.y][i] != 0){
                            return 0;
                        }
                    }
                }
            }
            if((initial.y == 0 && initial.x==0) && (final.y!=initial.y || final.x!=initial.x)){
                QueenSWhiteRookCounter++;
            }
            else if((initial.y ==0 && initial.x==7)&&(final.y!=initial.y||final.x!=initial.x)){
                KingSWhiteRookCounter++;
            }
            return 1;
        }
    }
    /*Black Rook*/
    else if ( piece == 8)
    {
       if (EnPassant ==1 ){
            EnPassant = 0;
        }

        if (initial.x == final.x || initial.y == final.y ){
            if (initial.x == final.x){
                if (final.y > initial.y){
                    for (int i = initial.y + 1; i < final.y; i++){
                        if ( board[i][initial.x] != 0){
                            return 0;
                        }
                    }
                }
                else
                {
                    for (int i = initial.y - 1; i > final.y; i--){
                        if ( board[i][initial.x] != 0){
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if (final.x > initial.x){
                    for (int i = initial.x + 1 ; i < final.x; i++){
                        if ( board[initial.y][i] != 0)
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    for (int i = initial.x -1; i > final.x; i--){
                        if ( board[initial.y][i] != 0){
                            return 0;
                        }
                    }
                }
            }
            if((initial.y == 0 && initial.x==0) && (final.y!=initial.y || final.x!=initial.x)){
                QueenSBlackRookCounter++;
            }
            else if((initial.y ==0 && initial.x==7)&&(final.y!=initial.y||final.x!=initial.x)){
                KingSBlackRookCounter++;
            }
            return 1;
        }
      
    }


    /*Black and White Bishop*/
    else if (piece == 4||piece == 10)
    {
        if(EnPassant == 1){
            EnPassant = 0;
        }
        int diagcheck = 0;

        for (int i = 1; i <= 7; i++){
            if ((final.y == initial.y + i || final.y == initial.y -i)&&(final.x==initial.x+i||final.x==initial.x-i)){
                diagcheck = 1;
            }
        }
        if (diagcheck == 0){
            return 0;
        }else if (diagcheck == 1)
        {
            if (initial.y < final.y)  {
                if (initial.x < final.x){
                    for (int i = 1; i < final.y - initial.y; i++){
                        if (board[initial.y+i][initial.x+i] != 0){
                            return 0;
                        }
                    }
                }else{
                    for (int i = 1; i < final.y - initial.y; i++ ){
                        if ( board[initial.y+i][initial.x-i] != 0){
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if (initial.x < final.x){
                    for (int i = 1; i < initial.y - final.y; i++){
                        if (board[initial.y -i][initial.x+i] != 0){
                            return 0;
                        }
                    }
                }
                else
                {
                    for (int i = 1; i < initial.y - final.y; i++){
                        if (board[initial.y -i][initial.x -i] != 0){
                            return 0;
                        }
                    }
                }
            }
            return 1;
        }
    }

    /*Black and White Knight*/
    else if (piece == 3 || piece == 9)
    {
        if (EnPassant == 1){
            EnPassant = 0;
        }
        if ((final.y == initial.y - 2) && (final.x == initial.x + 1)){
            return 1;
        }
        else if ((final.y == initial.y - 1) && (final.x == initial.x + 2)){
            return 1;
        }
        else if ((final.y == initial.y + 1) && (final.x == initial.x + 2)){
            return 1;
        }
        else if ((final.y == initial.y+2) && (final.x == initial.x + 1)){
            return 1;
        }
        else if ((final.y == initial.y + 2) && (final.x == initial.x -1)){
            return 1;
        }
        else if ((final.y == initial.y + 1) && (final.x == initial.x - 2)){
            return 1;
        }
        else if ((final.y == initial.y - 1) && (final.x == initial.x - 2)){
            return 1;
        }
        else if ((final.y == initial.y - 2) && (final.x == initial.x -1)){
            return 1;
        }
        else{
            return 0;
        }
    }

    /*Black and White Queen*/
    else if (piece == 6 || piece == 12)
    {
        if (EnPassant ==1 ){
            EnPassant = 0;
        }

        if (initial.x == final.x || initial.y == final.y ){
            if (initial.x == final.x){
                if (final.y > initial.y){
                    for (int i = initial.y + 1; i < final.y; i++){
                        if ( board[i][initial.x] != 0){
                            return 0;
                        }
                    }
                }
                else
                {
                    for (int i = initial.y - 1; i > final.y; i--){
                        if ( board[i][initial.x] != 0){
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if (final.x > initial.x){
                    for (int i = initial.x + 1 ; i < final.x; i++){
                        if ( board[initial.y][i] != 0)
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    for (int i = initial.x -1; i > final.x; i--){
                        if ( board[initial.y][i] != 0){
                            return 0;
                        }
                    }
                }
            }
            return 1;
        }

        int diagcheck = 0;
        for (int i = 1; i <= 7; i++){
            if ((final.y == initial.y + i || final.y == initial.y -i)&&(final.x==initial.x+i||final.x==initial.x-i)){
                diagcheck = 1;
            }
        }
        if (diagcheck == 1){
            if (initial.y < final.y){
                if (initial.x < final.x){
                    for (int i = 1; i < final.y - initial.y; i++){
                        if (board[initial.y+i][initial.x+i] != 0){
                            return 0;
                        }
                    }
                }
                else
                {
                    for (int i = 1; i < final.y - initial.y; i++ ){
                        if ( board[initial.y+i][initial.x-i] != 0){
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if (initial.x < final.x){
                    for (int i = 1; i < initial.y - final.y; i++){
                        if (board[initial.y -i][initial.x+i] != 0){
                            return 0;
                        }
                    }
                }
                else
                {
                    for (int i = 1; i < initial.y - final.y; i++){
                        if (board[initial.y -i][initial.x -i] != 0){
                            return 0;
                        }
                    }
                }
            }return 1;
        }return 0;
    }

    /*White King*/
    else if (piece == 5 /*|| piece == 11*/)
    {
        if (EnPassant == 1){
            EnPassant = 0;
        }
	    /*when in check*/
        int tempboard[8][8];
        for(int i =0;i<8;i++){
            for(int j=0;j<8;j++){
                tempboard[i][j]=board[i][j];
            }
        }
        tempboard[final.y][final.x] = WhiteK;
        tempboard[initial.y][initial.x] = 0;
        int check = IsCheck(tempboard,turn++);
        if(check>0){
            return 0;
        }
       
        /*King Side Castling*/
        if ((initial.y == 7 && initial.x == 4) && (board[7][5] == 0 && board[7][6] == 0 && board[7][7] == 2) && (final.x == 6 &&final.y==7)&&KingSWhiteRookCounter==0&&WhiteKingCounter==0){
       
	  int tempboard2[8][8]; 
	  for (int i =0;i<8;i++){
	  	for (int j =0;j<8;j++){
			tempboard2[i][j] = board[i][j];
		}
	  }
	  tempboard2[7][5] = WhiteK; 
	  tempboard2[7][4] = 0; 
	  int check = IsCheck(tempboard2,turn++); 
	  if(check>0){
		return 0; 
        }
	  tempboard2[7][6] = WhiteK; 
	  tempboard2[7][5] = 0; 
	  int check2 =  IsCheck(tempboard2,turn++); 
	  if(check2>0){
		return 0;
	  }
	  WhiteCastleKingSide = 1; 
	  return 3;
	}      
	    /*Queen Side Castling*/
        else if((initial.y==7 && initial.x == 4) && (board[7][1] == 0 && board[7][2] == 0 && board[7][3] == 0 && board[7][0] == 2) && (final.x == 2 && final.y == 7)&&QueenSWhiteRookCounter==0&&WhiteKingCounter==0){
            int tempboard3[8][8];
            for (int i =0;i<8;i++){
                for (int j =0;j<8;j++){
                    tempboard3[i][j] = board[i][j];
                }
            }
            tempboard3[7][3] = WhiteK;
            tempboard3[7][4] = 0;
            int check3 = IsCheck(tempboard3,turn++);
            if(check3>0){
                return 0;
            }
            tempboard3[7][2] = WhiteK;
            tempboard3[7][3] = 0;
            int check4 =  IsCheck(tempboard3,turn++);
            if(check4>0){
                return 0;
            }
	    WhiteCastleQueenSide = 1; 
            return 4;
        }

	if(board[final.y+1][final.x] == BlackK||board[final.y-1][final.x] == BlackK||board[final.y][final.x+1] == BlackK||board[final.y][final.x-1] == BlackK||board[final.y+1][final.x+1] == BlackK||board[final.y-1][final.x+1] == BlackK || board[final.y-1][final.x-1] == BlackK||board[final.y+1][final.x-1] == BlackK){
		return 0; 
	}

        if ((final.y == initial.y + 1) && (final.x == initial.x)){
            WhiteKingCounter++;
            return 1;
        }
        else if ((final.y == initial.y -1) && (final.x == initial.x)){
            WhiteKingCounter++;
            return 1;
        }

        else if ((final.y == initial.y - 1) && (final.x == initial.x +1)){
            WhiteKingCounter++;
            return 1;
        }

        else if ((final.y == initial.y) && (final.x == initial.x +1)){
            WhiteKingCounter++;
            return 1;
        }

        else if ((final.y == initial.y +1)&&(final.x == initial.x +1) ){
            WhiteKingCounter++;
            return 1;
        }

        else if ((final.x == initial.x) && (final.y == initial.y + 1)){
            WhiteKingCounter++;
            return 1;
        }

        else if ((final.y == initial.y + 1)&&(final.x == initial.x - 1)){
            WhiteKingCounter++;
            return 1;
        }

        else if ((final.y == initial.y) && (final.x == initial.x -1)){
            WhiteKingCounter++;
            return 1;
        }

        else if ((final.x == initial.x - 1) && (final.y == initial.y -1)){
            WhiteKingCounter++;
            return 1;
        }
        else{
            return 0;
        }
    }
    /*Black King*/
    else if (piece == 11)
    {
        if (EnPassant == 1){
            EnPassant = 0;
        }
	    /* when in check*/
        int tempboard[8][8];
        for(int i =0;i<8;i++){
            for(int j=0;j<8;j++){
                tempboard[i][j]=board[i][j];
            }
        }
        tempboard[final.y][final.x] = BlackK;
        tempboard[initial.y][initial.x] = 0;
        int check = IsCheck(tempboard,turn++);
        if(check>0){
            return 0;
        }
        
        /*King Side castling*/
        if ((initial.y == 0 && initial.x == 4) && (board[0][5] == 0 && board[0][6] == 0 && board[0][7] == 8) && (final.x == 6 &&final.y==0)&&KingSBlackRookCounter==0&&BlackKingCounter==0){
            int tempboard3[8][8];
            for (int i =0;i<8;i++){
                for (int j =0;j<8;j++){
                    tempboard3[i][j] = board[i][j];
                }
            }
            tempboard3[0][5] = BlackK;
            tempboard3[0][4] = 0;
            int check4 = IsCheck(tempboard3,turn++);
            if(check4>0){
                return 0;
            }
            tempboard3[0][6] = BlackK;
            tempboard3[0][5] = 0;
            int check5 =  IsCheck(tempboard3,turn++);
            if(check5>0){
                return 0;
            }
	    BlackCastleKingSide = 1; 
            return 3;
        }
       /*Queen Side Castling*/ 
        else if((initial.y==0 && initial.x == 4) && (board[0][1] == 0 && board[0][2] == 0 && board[0][3] == 0 && board[0][0] == 8)&&(final.x == 2 && final.y == 0)&&QueenSBlackRookCounter==0&&BlackKingCounter==0){
            int tempboard4[8][8];
            for (int i =0;i<8;i++){
                for (int j =0;j<8;j++){
                    tempboard4[i][j] = board[i][j];
                }
            }
            tempboard4[0][3] = BlackK;
            tempboard4[0][4] = 0;
            int check6 = IsCheck(tempboard4,turn++);
            if(check6>0){
                return 0;
            }
            tempboard4[0][2] = BlackK;
            tempboard4[0][3] = 0;
            int check7 =  IsCheck(tempboard4,turn++);
            if(check7>0){
                return 0;
            }
	    BlackCastleQueenSide = 1;
            return 4;
        }
        if(board[final.y+1][final.x] == WhiteK||board[final.y-1][final.x] == WhiteK||board[final.y][final.x+1] == WhiteK||board[final.y][final.x-1] == WhiteK||board[final.y+1][final.x+1] == WhiteK||board[final.y-1][final.x+1] == WhiteK || board[final.y-1][final.x-1] == WhiteK||board[final.y+1][final.x-1] == WhiteK){
		return 0; 
	}
        if ((final.y == initial.y + 1) && (final.x == initial.x)){
            BlackKingCounter++;
            return 1;
        }
        else if ((final.y == initial.y -1) && (final.x == initial.x)){
            BlackKingCounter++;
            return 1;
        }
        
        else if ((final.y == initial.y - 1) && (final.x == initial.x +1)){
            BlackKingCounter++;
            return 1;
        }
        
        else if ((final.y == initial.y) && (final.x == initial.x +1)){
            BlackKingCounter++;
            return 1;
        }
        
        else if ((final.y == initial.y +1)&&(final.x == initial.x +1) ){
            BlackKingCounter++;
            return 1;
        }
        
        else if ((final.x == initial.x) && (final.y == initial.y + 1)){
            BlackKingCounter++;
            return 1;
        }
        
        else if ((final.y == initial.y + 1)&&(final.x == initial.x - 1)){
            BlackKingCounter++;
            return 1;
        }
        
        else if ((final.y == initial.y) && (final.x == initial.x -1)){
            BlackKingCounter++;
            return 1;
        }
        
        else if ((final.x == initial.x - 1) && (final.y == initial.y -1)){
            BlackKingCounter++;
            return 1;
        }
        else{
            return 0;
        }
        
    }
    return 0;
}

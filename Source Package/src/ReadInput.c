#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ReadInput.h"
#include "Move.h"
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
	int promotion = -1;
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
		if(pch != NULL) {
			promotion = atoi(pch);
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
   curr_move[4] = promotion;
/*   
   printf( "%s", initial);
   printf( "%s", final);
   printf("%d %d %d \n ", yinit, yfinal, piece);
   printf("%d %d", init , fin);

 */ 
   return curr_move;
}

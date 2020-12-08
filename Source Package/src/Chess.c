#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "chess.h"
#include "GUISTUFF.h"
#include "AI.h"
#include "string.h"
#include "Initialize.h"
#include "PrintBoard.h"
#include "ReadInput.h"
#include "Move.h"
#include "IsValid.h"
#include "Constants.h"
#include "Check.h"
#include "Logfile.h"

/* Functions Used */
void InitBoard();
void ResetBoard();
void ReverseGridColor(int g_x, int g_y);
void newMove(int g_x, int g_y);
void CheckSpace(int x, int y);
enum GRID returnPiece(int x, int y);
void DrawBoard();
void CoordToGrid(int c_x, int c_y, int *g_x, int *g_y);
// static gboolean on_delete_event (GtkWidget *widget, GdkEvent  *event, gpointer data);
gint area_click (GtkWidget *widget, GdkEvent *event, gpointer data);
int CheckPromo(int g_x, int g_y);
void UpdateMove(int oldX, int oldY, int newX, int newY, int promo);
int checkWhiteOrBlackSquare(int g_x, int g_y);
void ResetGame();
void badReset();
void closeChess(GtkWidget *widget, void *user_data);
void closeChessTitle(GtkWidget *widget, void *user_data);
void removeChess(GtkWidget *widget, gpointer data);

/* Gtk variables */
static GtkWidget *window ;
static GtkWidget *fixed ;
static GtkWidget *chess_icon ;
static GtkWidget *table;
static GtkWidget *newgmbtn;
static GtkWidget *logo;
static GtkWidget *resbtn;
static GtkWidget *offerdraw;
static GtkWidget *titlewin;
GtkWidget *queslay; 

/* Variables */
static enum GRID Board[BOARD_SIZE][BOARD_SIZE];        //board
static int moveFlag;                                //select and deselect flag
static int tempSpaceOld[2];                            //stores the space clicked
static int tempSpaceNew[2];                            //stores the space moved to
static int moveErrorFlag;                            //make sure dont click out of bounds
static int WhiteOrBlack ;                            //turn counter
static int promoFlag;                                //yes or no promo    flag
static int checkFlag;                                //check or no flag
int pieceMoveArr[5];                                //Translated units from GUI to ASCII
int nBoard[8][8];                                    //ASCII board
static int colorChoice = 0;                            //whatever side player chooses
static int successfulMove;                            //counts the sucessful move with WhiteOrBlack - used for promo
static int gameSelector = 0;                        //game mode selector

/* Pointers */
int *counter = &WhiteOrBlack;
int *promo = &promoFlag;
int *check = &checkFlag;

/* Log file */
LogFile *lf;

int nBoard[8][8];
/* Functions for the title screen */
GdkPixbuf *create_pixbuf(const gchar * filename);
void VsPlayer (GtkButton *button, void *user_data);
void VsAI (GtkButton *button, void *user_data);
void WhitePiece(GtkButton *button, void *user_data);
void BlackPiece(GtkButton *button, void *user_data);
gboolean title_screen_end(GtkWidget *widget, gpointer data);
gboolean title_screen_load(GtkWidget *widget, gpointer data);


/* Functions for buttons on game screen */
gboolean NewGameReset(GtkWidget *widget, gpointer user_data);
void Resign(GtkButton *button, void *user_data);
void OfferDraw(GtkButton *button, void *user_data);
void WhiteWins();
void BlackWins();

void *homeWindow;

void chess_window(GtkButton *button,void *user_data)
{
	homeWindow = user_data;
	gtk_widget_hide(user_data);
    lf = CreateLogFile();
    
    /* ---MAIN WINDOW INITIALIZATION--- */
    titlewin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(titlewin), GTK_WIN_POS_CENTER);
    
    /* ---TITLE OF THE WINDOW--- */
    gtk_window_set_title(GTK_WINDOW(titlewin), "ZotChess");
    gtk_widget_set_size_request(titlewin, 350, 300);
    
    /* ---ICON ON THE UPPER LEFT OF THE WINDOW--- */
    GdkPixbuf *icon;
    icon = create_pixbuf("./images/logo.png");
    gtk_window_set_icon(GTK_WINDOW(titlewin), icon);
    
    
    //widget for the label of the title screen
    GtkWidget *label;
    label = gtk_label_new("Welcome to ZotChess!");
    
    //changes the formatting of the "Welcome to ZotChess" string using Pango Attributes
    char *string = g_strdup_printf ("<span font=\"18\" color=\"black\">" "<b>Welcome to ZotChess!</b>" "</span>");
    gtk_label_set_markup (GTK_LABEL (label), string);
    g_free (string);
	
	GtkWidget *dlabel;
    dlabel = gtk_label_new("Please select which piece color you would like to start with");
    char *stringQues = g_strdup_printf ("<span font=\"10\" color=\"black\">" "Please select which piece color you would like to start with" "</span>");
	gtk_label_set_markup (GTK_LABEL (dlabel), stringQues);
	g_free (stringQues);
	
	GtkWidget *vslabel = gtk_label_new("Please select the game mode");
	char *stringQues1 = g_strdup_printf ("<span font=\"12\" color=\"black\">" "Please select the game mode" "</span>");
	gtk_label_set_markup (GTK_LABEL (vslabel), stringQues1);
	g_free (stringQues1);
	
    //declarations and initializations of the 4 buttons on the title screen
    GtkWidget *btn1, *btn2, *btn3, *btn4;
    btn1 = gtk_button_new_with_label("Vs Player");
    btn2 = gtk_button_new_with_label("Vs A.I.");
    btn3 = gtk_button_new_with_label("White");
    btn4 = gtk_button_new_with_label("Black");
    
	g_signal_connect(btn1, "clicked", G_CALLBACK(VsPlayer), &gameSelector);
	gtk_widget_set_size_request(btn1, 70, 40);
	
	g_signal_connect(btn2, "clicked", G_CALLBACK(VsAI), &gameSelector);
	gtk_widget_set_size_request(btn2, 70, 40);
	
	GdkColor blackcolor1, whitecolor1;
	gdk_color_parse("black", &blackcolor1);
	gdk_color_parse("white", &whitecolor1);
	
	gtk_widget_modify_bg(GTK_WIDGET(btn3), GTK_STATE_NORMAL, &whitecolor1);
	GtkWidget *whbtnChild = gtk_bin_get_child(GTK_BIN(btn3));
	gtk_widget_modify_fg(GTK_WIDGET(whbtnChild), GTK_STATE_NORMAL, &blackcolor1);
	gtk_widget_set_size_request(btn3, 70, 40);
	g_signal_connect(btn3, "clicked", G_CALLBACK(WhitePiece), &colorChoice);
	
	gtk_widget_modify_bg(GTK_WIDGET(btn4), GTK_STATE_NORMAL, &blackcolor1);
	GtkWidget *blbtnChild = gtk_bin_get_child(GTK_BIN(btn4));
	gtk_widget_modify_fg(GTK_WIDGET(blbtnChild), GTK_STATE_NORMAL, &whitecolor1);
	gtk_widget_set_size_request(btn4, 70, 40);
    g_signal_connect(btn4, "clicked", G_CALLBACK(BlackPiece), &colorChoice);
	
	//"Load Game" button
    GtkWidget *loadgmbtn;
    loadgmbtn = gtk_button_new_with_label("Load Game");
	gtk_widget_set_size_request(loadgmbtn, 80, 40);
    g_signal_connect(loadgmbtn, "clicked", G_CALLBACK(title_screen_load), titlewin);
    
    //"Let's Play" button
    GtkWidget *playbtn;
    playbtn = gtk_button_new_with_label("Let's Play!");
	gtk_widget_set_size_request(playbtn, 70, 40);
    g_signal_connect(playbtn, "clicked", G_CALLBACK(title_screen_end), titlewin);
	
	GtkWidget *backchessques = gtk_image_new_from_file("./images/background.jpg");
	
	queslay = gtk_layout_new(NULL,NULL);
	gtk_layout_put(GTK_LAYOUT(queslay), backchessques, 0, 0);
	gtk_layout_put(GTK_LAYOUT(queslay), label, 60, 20);
	gtk_layout_put(GTK_LAYOUT(queslay), vslabel, 87, 65);
	gtk_layout_put(GTK_LAYOUT(queslay), btn1, 100, 85);
	gtk_layout_put(GTK_LAYOUT(queslay), btn2, 190, 85);
	gtk_layout_put(GTK_LAYOUT(queslay), dlabel, 20, 150);
	gtk_layout_put(GTK_LAYOUT(queslay), btn3, 100, 180);
	gtk_layout_put(GTK_LAYOUT(queslay), btn4, 190, 180);
	// gtk_layout_put(GTK_LAYOUT(queslay), loadgmbtn, 195, 250);
	gtk_layout_put(GTK_LAYOUT(queslay), playbtn, 275, 250);
	gtk_container_add(GTK_CONTAINER(titlewin), queslay);
	
    
    InitBoard();
    
    /*create a new window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;
    gtk_widget_set_size_request(window, 530, 575) ;
    gtk_container_set_border_width (GTK_CONTAINER(window), WINDOW_BORDER) ;
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER) ;
    gtk_window_set_title(GTK_WINDOW(window), "ZotChess") ;
    gtk_window_set_icon(GTK_WINDOW(window), icon);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE) ;
    
    /*register event handlers*/
    g_signal_connect(window, "delete_event", G_CALLBACK(closeChess), user_data) ;
    gtk_widget_set_events(window, GDK_BUTTON_PRESS_MASK) ;
    g_signal_connect(window, "button_press_event", G_CALLBACK(area_click), NULL) ;
    
    /*create a table and draw the board*/
    table = gtk_table_new (BOARD_SIZE, BOARD_SIZE, TRUE) ;
    gtk_widget_set_size_request (table, BOARD_WIDTH, BOARD_HEIGHT) ;
    DrawBoard();
    
    logo = gtk_image_new_from_file("./images/logo.png");
    newgmbtn = gtk_button_new_with_label("New Game");
    offerdraw = gtk_button_new_with_label("Offer Draw");
    resbtn = gtk_button_new_with_label("Resign");
    g_signal_connect(newgmbtn, "clicked", G_CALLBACK(NewGameReset), titlewin);
    
    fixed = gtk_fixed_new() ;
    // gtk_fixed_put(GTK_FIXED(fixed), resbtn, 230, 535);
    gtk_fixed_put(GTK_FIXED(fixed), newgmbtn , 420, 510);
    // gtk_fixed_put(GTK_FIXED(fixed), offerdraw, 220, 510);
    gtk_fixed_put(GTK_FIXED(fixed), table, 0, 0) ;
    gtk_fixed_put(GTK_FIXED(fixed), logo, 5, 505) ;
    gtk_container_add(GTK_CONTAINER(window), fixed) ;
    
    // g_signal_connect(resbtn, "clicked", G_CALLBACK(Resign), (gpointer)window);
    // g_signal_connect(offerdraw, "clicked", G_CALLBACK(OfferDraw), (gpointer)window);
    
    
    g_signal_connect(titlewin, "delete_event", G_CALLBACK(closeChessTitle), user_data) ;
    /*show the window*/
    gtk_widget_show_all(titlewin);
	
}
/*Initialize the Board*/
void InitBoard()
{
    /* If load game */
    if(gameSelector == 4)
    {
        g_print("Loading game...\n");
        if(WhiteOrBlack % 2 == 0){
            g_print("White's Turn\n");
        }
        else if(WhiteOrBlack % 2 == 1){
            g_print("Black's Turn\n");
        }
    }
    else if(gameSelector != 4)
    {
        for(int i = 1; i < BOARD_SIZE-1; i++){
            if(i % 2 == 1){
                nBoard[1][i-1] = 7;        //black pawn
                nBoard[6][i-1] = 1;        //white pawn
            }
            else if (i % 2 == 0){
                nBoard[1][i-1] = 7;        //black pawn
                nBoard[6][i-1] = 1;        //white pawn
            }
        }
        // nBoard[y][x]
        
        nBoard[7][7] = 2;                //white
        nBoard[7][6] = 3;
        nBoard[7][5] = 4;
        nBoard[7][4] = 5;
        nBoard[7][3] = 6;
        nBoard[7][2] = 4;
        nBoard[7][1] = 3;
        nBoard[7][0] = 2;
        
        nBoard[0][7] = 8;                //black
        nBoard[0][6] = 9;
        nBoard[0][5] = 10;
        nBoard[0][4] = 11;
        nBoard[0][3] = 12;
        nBoard[0][2] = 10;
        nBoard[0][1] = 9;
        nBoard[0][0] = 8;
    }
    if(gameSelector == 2 && colorChoice == 2)
    {
        AIMove(nBoard, counter, check, lf);
        g_print("Turn: %d\nBlack's Turn\n", WhiteOrBlack);
    }
    ResetBoard();
    if(gameSelector == 2 && colorChoice == 2)
    {
        gtk_widget_hide(window);
        badReset();
        gtk_widget_show_all(window);
    }
}

/* Prints the board based on the code */
void ResetBoard(){
    if(checkFlag != 2 && (gameSelector == 2 && (((WhiteOrBlack %2)+1) != colorChoice)))
    {
        if(WhiteOrBlack %2 == 0)
        {
            g_print("Turn: %d\nWhite's Turn\n", WhiteOrBlack);
        }
        else
        {
            g_print("Turn: %d\nBlack's Turn\n", WhiteOrBlack);
        }
        AIMove(nBoard, counter, check, lf);
    }
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(checkWhiteOrBlackSquare(i, j) == 0){
                Board[i][j] = BLACK;
            }
            else if(checkWhiteOrBlackSquare(i , j) == 1){
                Board[i][j] = WHITE;
            }
            
            if(i == 0 || i == BOARD_SIZE-1 || j == 0 || j == BOARD_SIZE-1){
                Board[i][j] = BROWN;
                if(i == 0){                                        //prints out the letters and number corresponding to each grid
                    if(j == 1){
                        Board[i][j] = BROWN_8;
                    }
                    else if(j == 2){
                        Board[i][j] = BROWN_7;
                    }
                    else if(j == 3){
                        Board[i][j] = BROWN_6;
                    }
                    else if(j == 4){
                        Board[i][j] = BROWN_5;
                    }
                    else if(j == 5){
                        Board[i][j] = BROWN_4;
                    }
                    else if(j == 6){
                        Board[i][j] = BROWN_3;
                    }
                    else if(j == 7){
                        Board[i][j] = BROWN_2;
                    }
                    else if(j == 8){
                        Board[i][j] = BROWN_1;
                    }
                }
                if(j == 9){
                    if(i == 1){
                        Board[i][j] = BROWN_A;
                    }
                    else if(i == 2){
                        Board[i][j] = BROWN_B;
                    }
                    else if(i == 3){
                        Board[i][j] = BROWN_C;
                    }
                    else if(i == 4){
                        Board[i][j] = BROWN_D;
                    }
                    else if(i == 5){
                        Board[i][j] = BROWN_E;
                    }
                    else if(i == 6){
                        Board[i][j] = BROWN_F;
                    }
                    else if(i == 7){
                        Board[i][j] = BROWN_G;
                    }
                    else if(i == 8){
                        Board[i][j] = BROWN_H;
                    }
                }
                if(checkFlag == 1){                        //if the player is in check
                    Board[9][0] = BROWN_CHECK;
                }
                if(promoFlag == 1){                        //if a promotion is available
                    Board[2][0] = BROWN_PROMO;
                    Board[3][0] = BROWN_PROMOQ;
                    Board[4][0] = BROWN_PROMOR;
                    Board[5][0] = BROWN_PROMON;
                    Board[6][0] = BROWN_PROMOB;
                }
            }
        }
    }
    
    for(int i = 0; i < 8; i++){                            //prints out the piece with the respective board color
        for(int j = 0; j < 8; j++)
        {
            if(checkWhiteOrBlackSquare(j+1, i+1) == 0){
                if(nBoard[j][i] == 1){
                    Board[i+1][j+1] = WPAWNB;
                }
                else if(nBoard[j][i] == 2){
                    Board[i+1][j+1] = WROOKB;
                }
                else if(nBoard[j][i] == 3){
                    Board[i+1][j+1] = WKNIGHTB;
                }
                else if(nBoard[j][i] == 4){
                    Board[i+1][j+1] = WBISHOPB;
                }
                else if(nBoard[j][i] == 5){
                    Board[i+1][j+1] = WKINGB;
                }
                else if(nBoard[j][i] == 6){
                    Board[i+1][j+1] = WQUEENB;
                }
                else if(nBoard[j][i] == 7){
                    Board[i+1][j+1] = BPAWNB;
                }
                else if(nBoard[j][i] == 8){
                    Board[i+1][j+1] = BROOKB;
                }
                else if(nBoard[j][i] == 9){
                    Board[i+1][j+1] = BKNIGHTB;
                }
                else if(nBoard[j][i] == 10){
                    Board[i+1][j+1] = BBISHOPB;
                }
                else if(nBoard[j][i] == 11){
                    Board[i+1][j+1] = BKINGB;
                }
                
                else if(nBoard[j][i] == 12){
                    Board[i+1][j+1] = BQUEENB;
                }
            }
            else if(checkWhiteOrBlackSquare(j+1, i+1) == 1){
                if(nBoard[j][i] == 1){
                    Board[i+1][j+1] = WPAWNW;
                }
                else if(nBoard[j][i] == 2){
                    Board[i+1][j+1] = WROOKW;
                }
                else if(nBoard[j][i] == 3){
                    Board[i+1][j+1] = WKNIGHTW;
                }
                else if(nBoard[j][i] == 4){
                    Board[i+1][j+1] = WBISHOPW;
                }
                else if(nBoard[j][i] == 5){
                    Board[i+1][j+1] = WKINGW;
                }
                else if(nBoard[j][i] == 6){
                    Board[i+1][j+1] = WQUEENW;
                }
                else if(nBoard[j][i] == 7){
                    Board[i+1][j+1] = BPAWNW;
                }
                else if(nBoard[j][i] == 8){
                    Board[i+1][j+1] = BROOKW;
                }
                else if(nBoard[j][i] == 9){
                    Board[i+1][j+1] = BKNIGHTW;
                }
                else if(nBoard[j][i] == 10){
                    Board[i+1][j+1] = BBISHOPW;
                }
                else if(nBoard[j][i] == 11){
                    Board[i+1][j+1] = BKINGW;
                }
                
                else if(nBoard[j][i] == 12){
                    Board[i+1][j+1] = BQUEENW;
                }
            }
        }
    }
}

/*Reverse the Grid Colors*/
void ReverseGridColor(int g_x, int g_y){
    if(Board[g_x][g_y] == BLACK){
        Board[g_x][g_y] = WHITE;
    }
    else{
        Board[g_x][g_y] = BLACK;
    }
}

/*Select and Move*/
void newMove(int g_x, int g_y){
    /* Checks for promotion available */
    if(promoFlag == 1){
        g_print("Select the square above for a piece to promote your pawn into!\n");
        int x = g_x;
        int y = g_y;
        
        if(Board[x][y] == BROWN_PROMOQ){
            if(WhiteOrBlack % 2 == 0){
                if(Board[tempSpaceNew[0]][tempSpaceNew[1]] == WPAWNW || Board[tempSpaceNew[0]][tempSpaceNew[1]] == WPAWNB){
                    nBoard[0][tempSpaceNew[0]-1] = WhiteQ;
                    g_print("Set pawn to White Queen\n");
                }
            }
            else if(WhiteOrBlack % 2 == 1){
                if(Board[tempSpaceNew[0]][tempSpaceNew[1]] == BPAWNW || Board[tempSpaceNew[0]][tempSpaceNew[1]] == BPAWNB){
                    nBoard[7][tempSpaceNew[0]-1] = BlackQ;
                    g_print("Set pawn to Black Queen\n");
                }
            }
            promoFlag = 0;
        }
        else if(Board[x][y] == BROWN_PROMOR){
            if(WhiteOrBlack % 2 == 0){
                if(Board[tempSpaceNew[0]][tempSpaceNew[1]] == WPAWNW || Board[tempSpaceNew[0]][tempSpaceNew[1]] == WPAWNB){
                    nBoard[0][tempSpaceNew[0]-1] = WhiteR;
                    g_print("Set pawn to White Rook\n");
                }
            }
            else if(WhiteOrBlack % 2 == 1){
                if(Board[tempSpaceNew[0]][tempSpaceNew[1]] == BPAWNW || Board[tempSpaceNew[0]][tempSpaceNew[1]] == BPAWNB){
                    nBoard[7][tempSpaceNew[0]-1] = BlackR;
                    g_print("Set pawn to Black Rook\n");
                }
            }
            promoFlag = 0;
        }
        else if(Board[x][y] == BROWN_PROMON){
            if(WhiteOrBlack % 2 == 0){
                if(Board[tempSpaceNew[0]][tempSpaceNew[1]] == WPAWNW || Board[tempSpaceNew[0]][tempSpaceNew[1]] == WPAWNB){
                    nBoard[0][tempSpaceNew[0]-1] = WhiteN;
                    g_print("Set pawn to White Knight\n");
                }
            }
            else if(WhiteOrBlack % 2 == 1){
                if(Board[tempSpaceNew[0]][tempSpaceNew[1]] == BPAWNW || Board[tempSpaceNew[0]][tempSpaceNew[1]] == BPAWNB){
                    nBoard[7][tempSpaceNew[0]-1] = BlackN;
                    g_print("Set pawn to Black Knight\n");
                }
            }
            promoFlag = 0;
        }
        else if(Board[x][y] == BROWN_PROMOB){
            if(WhiteOrBlack % 2 == 0){
                if(Board[tempSpaceNew[0]][tempSpaceNew[1]] == WPAWNW || Board[tempSpaceNew[0]][tempSpaceNew[1]] == WPAWNB){
                    nBoard[0][tempSpaceNew[0]-1] = WhiteB;
                    g_print("Set pawn to White Bishop\n");
                }
            }
            else if(WhiteOrBlack % 2 == 1){
                if(Board[tempSpaceNew[0]][tempSpaceNew[1]] == BPAWNW || Board[tempSpaceNew[0]][tempSpaceNew[1]] == BPAWNB){
                    nBoard[7][tempSpaceNew[0]-1] = BlackB;
                    g_print("Set pawn to Black Bishop\n");
                }
            }
            promoFlag = 0;
        }
        else{
            g_print("Invalid promotion selection, please try again!\n");            //doesn't break out of loop until user selects a promotion
        }
        if(promoFlag == 0){
            WhiteOrBlack++;
            successfulMove = WhiteOrBlack;
            checkFlag = IsCheck(nBoard, WhiteOrBlack);
            if(WhiteOrBlack % 2 == 0){
                g_print("White's Turn\n");
            }
            else{
                g_print("Black's Turn\n");
            }
        }
        ResetBoard();
    }
    /* if no promotion */
    else if(promoFlag == 0){
        if(moveFlag == 0){
            CheckSpace(g_x, g_y);
            tempSpaceOld[0] = g_x;
            tempSpaceOld[1] = g_y;
        }
        else if(moveFlag == 1){
            if((tempSpaceOld[0] == g_x) && (tempSpaceOld[1] == g_y)){
                moveErrorFlag = 1;
                g_print("It is the same spot!\n");
            }
            if((g_x < 1) || (g_x > 8) || (g_y < 1) || (g_y > 8)){
                moveErrorFlag = 1;
                g_print("You can't click out of bounds!\n");
            }
            /* If the movements are legal, allow piece to Move */
            /* Can't move on top of another piece */
            if(moveErrorFlag == 0){
                tempSpaceNew[0] = g_x;
                tempSpaceNew[1] = g_y;
                
                /* Check for promotion */
                
                UpdateMove(tempSpaceOld[0], tempSpaceOld[1], tempSpaceNew[0], tempSpaceNew[1], promoFlag);        // updates the move into array
                //g_print("x0:%d, y0:%d\nx1:%d, y1:%d\npromo:%d\n",pieceMoveArr[0], pieceMoveArr[1], pieceMoveArr[2], pieceMoveArr[3], pieceMoveArr[4]);
                GUIMove(pieceMoveArr, nBoard, counter, check, lf);                                                    // ASCII board does its stuff
            }
            if(checkFlag == 1){
                if(WhiteOrBlack == 0){
                    g_print("The White King is in Check!\n");
                }
                else if(WhiteOrBlack == 1){
                    g_print("The Black King is in Check!\n");
                }
            }
            ResetBoard();
            /* Deselects Move */
            promoFlag = CheckPromo(tempSpaceNew[0], tempSpaceNew[1]);
            
            if((promoFlag == 1) && (successfulMove < WhiteOrBlack)){
                WhiteOrBlack--;            //goes back to previous player's turn to implement promotion
                Board[2][0] = BROWN_PROMO;
                Board[3][0] = BROWN_PROMOQ;
                Board[4][0] = BROWN_PROMOR;
                Board[5][0] = BROWN_PROMON;
                Board[6][0] = BROWN_PROMOB;
                g_print("Select the square above for a piece to promote your pawn into!\n");
            }
            else
            {
                promoFlag = 0;            //if promo isn't legit, turns off flag
            }
            //g_print("x1: %d, x2: %d\n", tempSpaceNew[0], tempSpaceNew[1]);
            //ResetBoard();
            g_print("Piece Deselected!\n");
            moveFlag = 0;
            moveErrorFlag = 0;
            g_print("Turn: %d\n", (WhiteOrBlack));
            if(WhiteOrBlack % 2 == 0){
                g_print("White's Turn\n");
            }
            else if(WhiteOrBlack % 2 == 1){
                g_print("Black's Turn\n");
            }
            
            if(successfulMove != WhiteOrBlack){
                successfulMove = WhiteOrBlack;
            }
            
            if(checkFlag == 2)
            {
                ResetBoard();
                PrintLogs(lf, WhiteOrBlack);
                DeleteLogFile(lf);
                if (WhiteOrBlack%2==0){
                    //gtk_widget_hide(window);
                    //badReset();
                    //gtk_widget_hide(window);
                    //gtk_widget_show_all(window);
                    printf("Black side wins\n");
                    gtk_main_quit();
                    BlackWins();
                }
                else{
                    printf("White side wins\n");
                    WhiteWins();
                    //badReset();
                    //gtk_widget_hide(window);
                    //gtk_widget_show_all(window);
                }
            }
        }
    }
}

/* Check the space for a certain piece */
void CheckSpace(int x, int y){
    if(WhiteOrBlack % 2 == 0){
        if(Board[x][y] == WKINGW || Board[x][y] == WKINGB){
            g_print("White King Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == WQUEENW || Board[x][y] == WQUEENB){
            g_print("White Queen Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == WROOKW || Board[x][y] == WROOKB){
            g_print("White Rook Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == WBISHOPW || Board[x][y] == WBISHOPB){
            g_print("White Bishop Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == WKNIGHTW || Board[x][y] == WKNIGHTB){
            g_print("White Knight Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == WPAWNW || Board[x][y] == WPAWNB){
            g_print("White Pawn Pressed!\n");
            moveFlag = 1;
        }
    }
    else if(WhiteOrBlack % 2 == 1){
        if(Board[x][y] == BKINGW || Board[x][y] == BKINGB){
            g_print("Black King Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == BQUEENW || Board[x][y] == BQUEENB){
            g_print("Black Queen Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == BROOKW || Board[x][y] == BROOKB){
            g_print("Black Rook Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == BBISHOPW || Board[x][y] == BBISHOPB){
            g_print("Black Bishop Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == BKNIGHTW || Board[x][y] == BKNIGHTB){
            g_print("Black Knight Pressed!\n");
            moveFlag = 1;
        }else if(Board[x][y] == BPAWNW || Board[x][y] == BPAWNB){
            g_print("Black Pawn Pressed!\n");
            moveFlag = 1;
        }
    }
    if(moveFlag == 0 || Board[x][y] == WHITE || Board[x][y] == BLACK){
        moveFlag = 0;
        g_print("Invalid Piece Selection\n");
        if(WhiteOrBlack % 2 == 0){
            g_print("White's Turn\n");
        }
        else if(WhiteOrBlack % 2 == 1){
            g_print("Black's Turn\n");
        }
    }
}

/* returns the piece color and board color */
enum GRID returnPiece(int x, int y){
    return Board[x][y];
}

/*Draw the board*/
void DrawBoard()
{
    int i, j;
    
    for(i = 0; i < BOARD_SIZE; i ++)
    {
        for(j = 0; j < BOARD_SIZE; j ++)
        {
            switch(Board[i][j])
            {
                case BLACK:
                    chess_icon = gtk_image_new_from_file("./chess_icon/Bsquare.jpg") ;
                    break;
                case WHITE:
                    chess_icon = gtk_image_new_from_file("./chess_icon/Wsquare.jpg") ;
                    break;
                case WKINGW:                                                                // draws the white king white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WKingW.jpg") ;
                    break;
                case BKINGW:                                                                // draws the black king white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BKingW.jpg") ;
                    break;
                case WQUEENW:                                                            // draws the white queen white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WQueenW.jpg");
                    break;
                case BQUEENW:                                                            // draws the black queen white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BQueenW.jpg");
                    break;
                case WROOKW:                                                                // draws the white rook white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WRookW.jpg");
                    break;
                case BROOKW:                                                                // draws the black rook white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BRookW.jpg");
                    break;
                case WKNIGHTW:                                                            // draws the white knight white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WKnightW.jpg");
                    break;
                case BKNIGHTW:                                                            // draws the black knight white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BKnightW.jpg");
                    break;
                case WBISHOPW:                                                            // draws the white bishop white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WBishopW.jpg");
                    break;
                case BBISHOPW:                                                            // draws the black bishop white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BBishopW.jpg");
                    break;
                case WPAWNW:                                                                // draws the white pawn white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WPawnW.jpg");
                    break;
                case BPAWNW:                                                                // draws the black pawn white space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BPawnW.jpg");
                    break;
                    /*---------------------------------------------------------------------------------------------------------------------------*/
                case WKINGB:                                                                // draws the white king black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WKingB.jpg") ;
                    break;
                case BKINGB:                                                                // draws the black king black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BKingB.jpg") ;
                    break;
                case WQUEENB:                                                            // draws the white queen black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WQueenB.jpg");
                    break;
                case BQUEENB:                                                            // draws the black queen black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BQueenB.jpg");
                    break;
                case WROOKB:                                                                // draws the white rook black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WRookB.jpg");
                    break;
                case BROOKB:                                                                // draws the black rook black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BRookB.jpg");
                    break;
                case WKNIGHTB:                                                            // draws the white knight black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WKnightB.jpg");
                    break;
                case BKNIGHTB:                                                            // draws the black knight black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BKnightB.jpg");
                    break;
                case WBISHOPB:                                                            // draws the white bishop black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WBishopB.jpg");
                    break;
                case BBISHOPB:                                                            // draws the black bishop black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BBishopB.jpg");
                    break;
                case WPAWNB:                                                                // draws the white pawn black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/WPawnB.jpg");
                    break;
                case BPAWNB:                                                                // draws the black pawn black space
                    chess_icon = gtk_image_new_from_file("./chess_icon/BPawnB.jpg");
                    break;
                    /*---------------------------------------------------------------------------------------------------------------------------*/
                case BROWN:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare.jpg");
                    break;
                case BROWN_1:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_1.jpg");
                    break;
                case BROWN_2:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_2.jpg");
                    break;
                case BROWN_3:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_3.jpg");
                    break;
                case BROWN_4:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_4.jpg");
                    break;
                case BROWN_5:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_5.jpg");
                    break;
                case BROWN_6:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_6.jpg");
                    break;
                case BROWN_7:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_7.jpg");
                    break;
                case BROWN_8:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_8.jpg");
                    break;
                    
                case BROWN_A:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_a.jpg");
                    break;
                case BROWN_B:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_b.jpg");
                    break;
                case BROWN_C:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_c.jpg");
                    break;
                case BROWN_D:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_d.jpg");
                    break;
                case BROWN_E:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_e.jpg");
                    break;
                case BROWN_F:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_f.jpg");
                    break;
                case BROWN_G:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_g.jpg");
                    break;
                case BROWN_H:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_h.jpg");
                    break;
                case BROWN_CHECK:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_Check.jpg");
                    break;
                case BROWN_PROMO:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_Promo.jpg");
                    break;
                case BROWN_PROMOQ:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_PromoQ.jpg");
                    break;
                case BROWN_PROMOR:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_PromoR.jpg");
                    break;
                case BROWN_PROMON:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_PromoN.jpg");
                    break;
                case BROWN_PROMOB:
                    chess_icon = gtk_image_new_from_file("./chess_icon/BrownSquare_PromoB.jpg");
                    break;
                default:
                    break;
                    
            }
            gtk_table_attach(GTK_TABLE(table), chess_icon, i, i + 1, j, j + 1, GTK_FILL, GTK_FILL, 0, 0) ;
        }
    }
}
/* Converts clicking coordinate to grid coordinate for chess board */
void CoordToGrid(int c_x, int c_y, int *g_x, int *g_y){
    *g_x = (c_x - BOARD_BORDER+5) / SQUARE_SIZE;
    *g_y = (c_y - BOARD_BORDER+5) / SQUARE_SIZE;
}

// static gboolean on_delete_event (GtkWidget *widget, GdkEvent  *event, gpointer data){
    // /* If you return FALSE in the "delete_event" signal handler,
     // * GTK will emit the "destroy" signal. Returning TRUE means
     // * you don't want the window to be destroyed.
     // *
     // * This is useful for popping up 'are you sure you want to quit?'
     // * type dialogs.
     // */
    // g_print("Thanks for playing!\n");
    // g_print ("delete event occurred\n");
    // PrintLogs(lf, WhiteOrBlack);
    // DeleteLogFile(lf);
    // gtk_main_quit();
    // return TRUE;
// }
/* converts clicking event to grid coordinates */
gint area_click (GtkWidget *widget, GdkEvent *event, gpointer data){
    //int x1, y1 ;
    //char words[MAX_MSGLEN] ;
    
    int coord_x, coord_y, grid_x, grid_y;
    
    //struct BOARD *chess_board ;
    //struct SQUARE *chess_piece ;
    //struct SQUARE *piece_dest ;
    
    GdkModifierType state ;
    
    gdk_window_get_pointer(widget->window, &coord_x, &coord_y, &state) ;
    
    CoordToGrid(coord_x, coord_y, &grid_x, &grid_y);
    
    //printf("coord_x = %d, coord_y = %d, grid_x = %d, grid_y = %d \n", coord_x, coord_y, grid_x, grid_y);
    
    newMove(grid_x, grid_y);
    
    
    gtk_container_remove(GTK_CONTAINER(window), fixed) ;
    table = gtk_table_new (BOARD_SIZE, BOARD_SIZE, TRUE) ;
    gtk_widget_set_size_request (table, BOARD_WIDTH, BOARD_HEIGHT) ;
    DrawBoard();
    
    /*set fixed*/
    
    fixed = gtk_fixed_new() ;
    gtk_fixed_put(GTK_FIXED(fixed), table, 0, 0) ;
    
    newgmbtn = gtk_button_new_with_label("New Game");
    logo = gtk_image_new_from_file("./images/logo.png");
    resbtn = gtk_button_new_with_label("Resign");
    offerdraw = gtk_button_new_with_label("Offer Draw");
    
    /*signal connect */
    g_signal_connect(newgmbtn, "clicked", G_CALLBACK(NewGameReset), titlewin);
    g_signal_connect(resbtn, "clicked", G_CALLBACK(Resign), (gpointer)window);
    g_signal_connect(offerdraw, "clicked", G_CALLBACK(OfferDraw), (gpointer)window);
    
    //220 510 for center
    
    // gtk_fixed_put(GTK_FIXED(fixed), resbtn, 230, 535);
    gtk_fixed_put(GTK_FIXED(fixed), newgmbtn , 420, 510);
    // gtk_fixed_put(GTK_FIXED(fixed), offerdraw, 220, 510);
    gtk_fixed_put(GTK_FIXED(fixed), logo, 5, 505) ;
    gtk_container_add(GTK_CONTAINER(window), fixed) ;
    gtk_widget_show_all(window) ;
    
    return TRUE ;
}
/* checks pawn piece for promotion */
int CheckPromo(int x, int y){
    if(WhiteOrBlack % 2 == 1){
        if(Board[x][y] == WPAWNW || Board[x][y] == WPAWNB){
            if(y == 1){
                g_print("White Pawn reached the other side!\nPreparing promotion...\n");
                return 1;
            }
        }
    }
    else if(Board[x][y] == BPAWNW || Board[x][y] == BPAWNB){
        if(y == 8){
            g_print("Black Pawn reached the other side!\nPreparing promotion...\n");
            return 1;
        }
    }
    return 0;
}

/* updates the move to be sent to ASCII board */
void UpdateMove(int oldX, int oldY, int newX, int newY, int promo){
    pieceMoveArr[0] = oldX - 1;
    pieceMoveArr[1] = oldY - 1;
    pieceMoveArr[2] = newX - 1;
    pieceMoveArr[3] = newY - 1;
    pieceMoveArr[4] = promo;
    
}

/* returns 1 if white, 0 if black, -1 for error */
int checkWhiteOrBlackSquare(int g_x, int g_y){
    if(g_x % 2 == 0){
        if(g_y % 2 == 0){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        if(g_y % 2 == 0){
            return 0;
        }
        else{
            return 1;
        }
    }
    return -1;
}

/* resets all values to their initial */
void ResetGame(){
    moveFlag = 0;
    tempSpaceOld[0] = 0;
    tempSpaceOld[1] = 0;
    tempSpaceNew[0] = 0;
    tempSpaceNew[1] = 0;
    moveErrorFlag = 0;
    WhiteOrBlack = 0;
    promoFlag = 0;
    checkFlag = 0;
    pieceMoveArr[0] = 0;
    pieceMoveArr[1] = 0;
    pieceMoveArr[2] = 0;
    pieceMoveArr[3] = 0;
    pieceMoveArr[4] = 0;
    successfulMove = 0;
    colorChoice = 0;
    gameSelector = 0;
    
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            nBoard[i][j] = 0;
        }
    }
}


//turns on the VS Player flag
void VsPlayer (GtkButton *button, void *user_data)
{
    if(gameSelector == 0)
    {
        gameSelector = 1;
        g_print("Player versus Player mode selected!\n");
    }
    else if(gameSelector == 1)
    {
        gameSelector = 0;
        g_print("Player versus Player mode deselected!\n");
    }
    else if(gameSelector == 2)
    {
        g_print("ERROR! You must deselect A.I. mode first!\n");
    }
}

//turns on the VS AI flag
void VsAI (GtkButton *button, void *user_data)
{
    if(gameSelector == 0)
    {
        g_print("Player versus A.I. mode selected!\n");
        gameSelector = 2;
    }
    else if(gameSelector == 2)
    {
        gameSelector = 0;
        g_print("Player versus A.I. mode deselected!\n");
    }
    else if(gameSelector == 1)
    {
        g_print("ERROR! You must deselect PVP mode first!\n");
    }
}

//turns on the White Piece flag
void WhitePiece(GtkButton *button, void *user_data)
{
    if(colorChoice == 0)
    {
        colorChoice = 1;
        g_print("You have selected to control white pieces.\n");
    }
    else if(colorChoice == 1)
    {
        colorChoice = 0;
        g_print("You have deselected white pieces.\n");
    }
    else if(colorChoice == 2)
    {
        g_print("ERROR! You must deselect black pieces first!\n");
    }
}

//turns on the Black Piece flag
void BlackPiece(GtkButton *button, void *user_data)
{
    if(colorChoice == 0)
    {
        colorChoice = 2;
        g_print("You have selected to control black pieces.\n");
    }
    else if(colorChoice == 2)
    {
        colorChoice = 0;
        g_print("You have deselected black pieces.\n");
    }
    else if(colorChoice == 1)
    {
        g_print("ERROR! You must deselect white pieces first!\n");
    }
}

// New game function
gboolean NewGameReset(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *dialog1, *dialog2;
    
    //creates the dialog question
    dialog1 = gtk_message_dialog_new(GTK_WINDOW(window),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_QUESTION,
                                     GTK_BUTTONS_YES_NO,
                                     "Are you sure you want to reset the game?");
    gtk_window_set_title(GTK_WINDOW(dialog1), "New Game");
    gint response = gtk_dialog_run(GTK_DIALOG(dialog1));
    
    
    if(response == -8)
    {
        dialog2 = gtk_message_dialog_new(GTK_WINDOW(window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_INFO,
                                         GTK_BUTTONS_OK,
                                         "Starting New Game!\n");
        gtk_window_set_title(GTK_WINDOW(dialog2), "Draw");
        gtk_dialog_run(GTK_DIALOG(dialog2));
        gtk_widget_destroy(dialog2);
        
        PrintLogs(lf, WhiteOrBlack);
        DeleteLogFile(lf);
        
        ResetGame();
        InitBoard();
        ResetBoard();
        gtk_widget_hide(window);
        badReset();
        
        lf = CreateLogFile();
    }
    gtk_widget_hide(window);
    gtk_widget_show_all(user_data);
    
    gtk_widget_destroy(dialog1);
    return TRUE;
}


//function that closes the title screen when the Let's Play button is clicked
gboolean title_screen_end(GtkWidget *widget, gpointer data)
{
    if( (gameSelector != 0)  && (colorChoice != 0))
    {
        gtk_widget_hide(data);
        gtk_widget_show_all(window) ;
        g_print("Turn: %d\n", (WhiteOrBlack));
        g_print("White's Turn\n");
        InitBoard();
    }
    else{
        GtkWidget *dialog2;
        dialog2 = gtk_message_dialog_new(GTK_WINDOW(window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_INFO,
                                         GTK_BUTTONS_OK,
                                         "You still have missing selections!\n");
        gtk_window_set_title(GTK_WINDOW(dialog2), "ERROR! Missing Selections!\n");
        gtk_dialog_run(GTK_DIALOG(dialog2));
        gtk_widget_destroy(dialog2);
    }
    return TRUE;
}

//Load Game function
gboolean title_screen_load(GtkWidget *widget, gpointer data)
{
    gameSelector = 4;
    gtk_widget_hide(data);
    g_print("Load Game selected!\n");
    Initialize(nBoard, &gameSelector, counter);
    ResetBoard();
    InitBoard();
    g_print("Turn: %d\n", *counter);
    badReset();
    gtk_widget_show_all(window);
    return TRUE;
}

//Resign function
void Resign(GtkButton *button, void *user_data)
{
    GtkWidget *dialog1, *dialog2;
    dialog1 = gtk_message_dialog_new(GTK_WINDOW(window),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_QUESTION,
                                     GTK_BUTTONS_YES_NO,
                                     "Are you sure you want to resign?");
    gtk_window_set_title(GTK_WINDOW(dialog1), "Resign");
    gint response = gtk_dialog_run(GTK_DIALOG(dialog1)); //stores the enum value for the response
    
    //if yes, another dialog box will be open
    //if no, returns back to the screen
    if(response == -8)
    {
        dialog2 = gtk_message_dialog_new(GTK_WINDOW(window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_INFO,
                                         GTK_BUTTONS_OK,
                                         "You have chosen to resign. Thank you for playing!\n");
        gtk_window_set_title(GTK_WINDOW(dialog2), "Resign");
        gtk_dialog_run(GTK_DIALOG(dialog2));
        gtk_widget_destroy(dialog2);
        
        PrintLogs(lf, WhiteOrBlack);
        DeleteLogFile(lf);
    }
    
    
    gtk_widget_destroy(dialog1);
    if(response == -8)
    {
        gtk_main_quit();
    }
}

//Offer Draw function
void OfferDraw(GtkButton *button, void *user_data)
{
    GtkWidget *dialog1, *dialog2;
    
    //creates the dialog question
    dialog1 = gtk_message_dialog_new(GTK_WINDOW(window),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_QUESTION,
                                     GTK_BUTTONS_YES_NO,
                                     "Are you sure you want to Offer a Draw?");
    gtk_window_set_title(GTK_WINDOW(dialog1), "Draw");
    gint response = gtk_dialog_run(GTK_DIALOG(dialog1));
    
    
    if(response == -8)
    {
        dialog2 = gtk_message_dialog_new(GTK_WINDOW(window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_INFO,
                                         GTK_BUTTONS_OK,
                                         "Tie game! Best of both worlds.\n");
        gtk_window_set_title(GTK_WINDOW(dialog2), "Draw");
        gtk_dialog_run(GTK_DIALOG(dialog2));
        gtk_widget_destroy(dialog2);
        
        PrintLogs(lf, WhiteOrBlack);
        DeleteLogFile(lf);
    }
    
    
    gtk_widget_destroy(dialog1);
    if(response == -8)
    {
        gtk_main_quit();
    }
}

/* resets the board whenever it is loaded with a new move, basically a pseudo "click" on the board */
void badReset(){
    GdkPixbuf *icon;
    icon = create_pixbuf("./images/logo.png");
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;
    gtk_widget_set_size_request(window, 530, 575) ;
    gtk_container_set_border_width (GTK_CONTAINER(window), WINDOW_BORDER) ;
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER) ;
    gtk_window_set_title(GTK_WINDOW(window), "ZotChess") ;
    gtk_window_set_icon(GTK_WINDOW(window), icon);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE) ;
    
    /*register event handlers*/
    g_signal_connect(window, "delete_event", G_CALLBACK(closeChess), NULL) ;
    gtk_widget_set_events(window, GDK_BUTTON_PRESS_MASK) ;
    g_signal_connect(window, "button_press_event", G_CALLBACK(area_click), NULL) ;
    
    /*create a table and draw the board*/
    table = gtk_table_new (BOARD_SIZE, BOARD_SIZE, TRUE) ;
    gtk_widget_set_size_request (table, BOARD_WIDTH, BOARD_HEIGHT) ;
    DrawBoard();
    
    
    logo = gtk_image_new_from_file("./images/logo.png");
    newgmbtn = gtk_button_new_with_label("New Game");
    offerdraw = gtk_button_new_with_label("Offer Draw");
    resbtn = gtk_button_new_with_label("Resign");
    g_signal_connect(newgmbtn, "clicked", G_CALLBACK(NewGameReset), titlewin);
    
    fixed = gtk_fixed_new() ;
    gtk_fixed_put(GTK_FIXED(fixed), table, 0, 0) ;
    gtk_fixed_put(GTK_FIXED(fixed), logo, 5, 505) ;
    // gtk_fixed_put(GTK_FIXED(fixed), resbtn, 230, 535);
    gtk_fixed_put(GTK_FIXED(fixed), newgmbtn , 420, 510);
    // gtk_fixed_put(GTK_FIXED(fixed), offerdraw, 220, 510);
    gtk_container_add(GTK_CONTAINER(window), fixed) ;
    
    g_signal_connect(resbtn, "clicked", G_CALLBACK(Resign), (gpointer)window);
    g_signal_connect(offerdraw, "clicked", G_CALLBACK(OfferDraw), (gpointer)window);
}

void WhiteWins()
{
    GtkWidget *dialog1;
    
    //creates the dialog popup
    dialog1 = gtk_message_dialog_new(GTK_WINDOW(window),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK,
                                     "White Wins!\nThanks for Playing.");
    gtk_window_set_title(GTK_WINDOW(dialog1), "White Victory");
    gtk_dialog_run(GTK_DIALOG(dialog1));
    gtk_widget_destroy(dialog1);
    gtk_main_quit();
    
}

void BlackWins()
{
    GtkWidget *dialog1;
    
    //creates the dialog popup
    dialog1 = gtk_message_dialog_new(GTK_WINDOW(window),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK,
                                     "Black Wins!\nThanks for Playing.");
    gtk_window_set_title(GTK_WINDOW(dialog1), "Black Victory");
    gtk_dialog_run(GTK_DIALOG(dialog1));
    gtk_widget_destroy(dialog1);
    gtk_main_quit();
    
}

void closeChess(GtkWidget *widget, void *user_data)
{
	PrintLogs(lf, WhiteOrBlack);
    DeleteLogFile(lf);
	gtk_widget_hide(window);
	gtk_widget_show_all(titlewin);
}


void closeChessTitle(GtkWidget *widget, void *user_data)
{
	ResetGame();
	gtk_widget_hide(titlewin);
	gtk_container_foreach(GTK_CONTAINER(window),removeChess, window);
	gtk_container_foreach(GTK_CONTAINER(titlewin),removeChess, titlewin);
	gtk_widget_show_all(homeWindow);
}

void removeChess(GtkWidget *widget, gpointer data)
{
	gtk_container_remove(GTK_CONTAINER(data), widget);
}
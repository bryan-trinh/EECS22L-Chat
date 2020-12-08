/***********************************************************
*
*	Log-in /Register screen
*	LoginRegis.c
*	author: Bryan Trinh
*	initial version: 02/21/19 EECS22L Winter 2019
*
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "CreateLoginAndVerify.h"

/* functions */
static gboolean on_delete_event (GtkWidget *widget, GdkEvent  *event, gpointer data);
void LoginPressed (GtkButton *button, void *user_data);
void RegisterPressed (GtkButton *button, void *user_data);
void returnToLoginRegis (GtkButton *button, void *user_data);
void returnHome (GtkButton *button, void *user_data); 
void OpenAddFriend(GtkButton *button, void *user_data); 
void OpenFriendRequest(GtkButton *button, void *user_data); 
void Accept(GtkButton *button, void *user_data); 
void Decline(GtkButton *button, void *user_data);
void LogOut(GtkButton *button, void *user_data);  
static void PressEnter_Login( GtkWidget *widget, GtkWidget *entry );
static void entry_toggle_visibility( GtkWidget *checkbutton, GtkWidget *entry );
void resetTextEntry();
static void PressEnter_Register( GtkWidget *widget, GtkWidget *entry );
static void PressEnter_Login( GtkWidget *widget, GtkWidget *entry );
void returnFromRegis (GtkButton *button, void *user_data);
GdkPixbuf *create_pixbuf(const gchar * filename);
void setUserHomeText(char *name);
void addFriend(GtkWidget *button, GtkWidget *table);
void removeFriend(GtkWidget *button, GtkWidget *table);
void logOutFunction(GtkWidget *button, GtkWidget *widget);
void FriendReq(GtkButton *button, void *user_data);

void copyTable1(GtkWidget *widget, gpointer data);
void removeWidgets(GtkWidget *widget, gpointer data);
void copyTable2(GtkWidget *widget, gpointer data);
void printTest(GtkWidget *widget, gpointer data);

/* global vars */
GtkWidget *loginRegisWindow;				//main loginRegisWindow
GtkWidget *loginInputWindow;				//secondary loginInputWindow
GtkWidget *registerWindow;					//secondary registerWindow
GtkWidget *homeWindow;						//main homeWindow
GtkWidget *homeLabel; 						//label text
GtkWidget *fixed;
GtkWidget *username;
GtkWidget *password;
GtkWidget *AddFriendWindow;
GtkWidget *AddFriendvbox;
GtkWidget *AddFriendhbox; 
GtkWidget *FriendRequestButton; 
GtkWidget *FriendRequestWindow; 
GtkWidget *FriendRequestLabel; 
GtkWidget *AcceptButton;
GtkWidget *DeclineButton; 
GtkWidget *IgnoreButton; 
GtkWidget *FriendRequestFixed;
GtkWidget *LogOutButton;  
GtkWidget *FRList;
GtkWidget *table;
GtkWidget *button;


//GtkWidget *table2;
GtkWidget *newTable;
const char *emptyString = "";
const char *secretUsername = "bryan";
const char *secretPassword = "password123";
GtkWidget *CreateUsername;
GtkWidget *CreatePassword;
GtkWidget *CreatePasswordConfirmation;
GtkWidget *AddUsername;
char *loggedInUsername = "";
int addX = 8;
int removeX = 0;
int testInt = 0;
char buffer[32];
int i, j;

static void username_enter(GtkWidget *widget, GtkWidget *entry)
{
	const gchar *entry1; 
	entry1 = gtk_entry_get_text(GTK_ENTRY(AddUsername)); 
	if(strcmp(entry1,emptyString) == 0)
	{
		g_print("You did not eneter a username\n"); 
	}
	else
	{
		g_print("You've added: %s\n",entry1); 
		gtk_widget_hide(AddFriendWindow);
		gtk_widget_show_all(homeWindow); 
	}
}

/* main */
int main( int   argc, char *argv[] )
{
	gtk_init (&argc, &argv);

	GtkWidget *loginBtn;			//login btn
	GtkWidget *registerBtn;			//register btn
	
	GtkWidget *label; 				//label text
	label = gtk_label_new("Welcome to ZotChat!");
	
	/* creates icon */
	GdkPixbuf *icon;
	icon = create_pixbuf("./images/logo.png");
	
	/* creates title image */
	GtkWidget *titleLogo;
	titleLogo = gtk_image_new_from_file("./images/logo_150x150.png");
	
	/* changes the formatting of the "Welcome to ZotChat" string using Pango Attributes */
	char *string = g_strdup_printf ("<span font=\"24\" color=\"black\">" "<b>Welcome to ZotChat!</b>" "</span>");
	gtk_label_set_markup (GTK_LABEL (label), string);
	g_free (string);
	
	/* naming btn */
	loginBtn = gtk_button_new_with_label("Log In");
	gtk_widget_set_size_request(loginBtn, 70, 35);
	registerBtn = gtk_button_new_with_label("Register");
	gtk_widget_set_size_request(registerBtn, 70, 35);

    /* create a new loginRegisWindow */
    loginRegisWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(loginRegisWindow, 350, 350);
	gtk_window_set_position(GTK_WINDOW(loginRegisWindow), GTK_WIN_POS_CENTER) ; 
	gtk_window_set_title(GTK_WINDOW(loginRegisWindow), "ZotChat") ;
	gtk_window_set_resizable(GTK_WINDOW(loginRegisWindow), FALSE) ;
	
	/* connecting signals - loginRegisWindow */
	g_signal_connect(loginRegisWindow, "delete_event", G_CALLBACK(on_delete_event), NULL) ; 
	gtk_widget_set_events(loginRegisWindow, GDK_BUTTON_PRESS_MASK) ;
	
	/* connecting signals - btns */
	g_signal_connect(loginBtn, "clicked", G_CALLBACK(LoginPressed), loginRegisWindow);
	g_signal_connect(registerBtn, "clicked", G_CALLBACK(RegisterPressed), loginRegisWindow);
	
	/* fixed placement of widgets */
	fixed = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(fixed), label, 25, 50);
	gtk_fixed_put(GTK_FIXED(fixed), titleLogo, 100, 100);
	gtk_fixed_put(GTK_FIXED(fixed), loginBtn, 95, 275);
	gtk_fixed_put(GTK_FIXED(fixed), registerBtn, 195, 275);
	gtk_container_add(GTK_CONTAINER(loginRegisWindow), fixed);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/* login screen */
    GtkWidget *vbox_Login, *hbox_Login;
    GtkWidget *button_Login, *button_LoginReturn;
    GtkWidget *check;
	GtkWidget *usernameText_Login, *passwordText_Login;

    /* create a new loginInputWindow */
    loginInputWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (GTK_WIDGET (loginInputWindow), 300, 200);
	gtk_window_set_position(GTK_WINDOW(loginInputWindow), GTK_WIN_POS_CENTER) ; 
    gtk_window_set_title (GTK_WINDOW (loginInputWindow), "ZotChat");

    vbox_Login = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (loginInputWindow), vbox_Login);
    gtk_widget_show (vbox_Login);

	/* username label */
	usernameText_Login = gtk_label_new("Username");
	gtk_box_pack_start(GTK_BOX(vbox_Login), usernameText_Login, 1, 1, 0);

	/* username entry box */
    username = gtk_entry_new ();
    gtk_entry_set_max_length (GTK_ENTRY (username), 50);
    g_signal_connect (username, "activate", G_CALLBACK (PressEnter_Login), username);
    gtk_box_pack_start (GTK_BOX (vbox_Login), username, TRUE, TRUE, 0);
    gtk_widget_show (username);
	
	/* password label */
	passwordText_Login = gtk_label_new("Password");
	gtk_box_pack_start(GTK_BOX(vbox_Login), passwordText_Login, 1, 1, 0);
	
	/* password entry box */
	password = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (password), 50);
    g_signal_connect (password, "activate", G_CALLBACK (PressEnter_Login), password);
	gtk_box_pack_start (GTK_BOX (vbox_Login), password, TRUE, TRUE, 0);
    gtk_widget_show (password);
	
	/* packing all boxes */
    hbox_Login = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (vbox_Login), hbox_Login);
    gtk_widget_show (hbox_Login);
	
	/* visible check box */
    check = gtk_check_button_new_with_label ("Visible");
    g_signal_connect (check, "toggled", G_CALLBACK (entry_toggle_visibility), password);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);			//makes the password entry box visible
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), FALSE);		//makes the password entry box invisible
	
	/* login click box */
    button_Login = gtk_button_new_with_label("Login");
    g_signal_connect(button_Login, "clicked", G_CALLBACK (PressEnter_Login), loginInputWindow);
    gtk_box_pack_start (GTK_BOX (vbox_Login), button_Login, TRUE, TRUE, 0);
    gtk_widget_set_can_default (button_Login, TRUE);
    gtk_widget_grab_default (button_Login);
    gtk_widget_show (button_Login);
	
	/* close click box */
    button_LoginReturn = gtk_button_new_with_label("Return");
    g_signal_connect(button_LoginReturn, "clicked", G_CALLBACK (returnToLoginRegis), loginInputWindow);
    gtk_box_pack_start (GTK_BOX (vbox_Login), button_LoginReturn, TRUE, TRUE, 0);
    gtk_widget_set_can_default (button_LoginReturn, TRUE);
    gtk_widget_grab_default (button_LoginReturn);
    gtk_widget_show (button_LoginReturn);
	
	/* connecting signals - loginInputWindow */
	g_signal_connect(loginInputWindow, "delete_event", G_CALLBACK(on_delete_event), NULL) ; 
	gtk_widget_set_events(loginInputWindow, GDK_BUTTON_PRESS_MASK) ;
	
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/* register screen */
    GtkWidget *vbox_Register, *hbox_Register;
    GtkWidget *button_Register, *button_toLogin;
	GtkWidget *usernameText_Register, *passwordText_Register, *passwordConfirmText_Register;

    /* create a new registerWindow */
	registerWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (GTK_WIDGET (registerWindow), 400, 300);
	gtk_window_set_position(GTK_WINDOW(registerWindow), GTK_WIN_POS_CENTER) ; 
    gtk_window_set_title (GTK_WINDOW (registerWindow), "ZotChat");

    vbox_Register = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (registerWindow), vbox_Register);
    gtk_widget_show (vbox_Register);
	
	/* username label */
	usernameText_Register = gtk_label_new("Username");
	gtk_box_pack_start(GTK_BOX(vbox_Register), usernameText_Register, 1, 1, 0);

	/* username entry box */
    CreateUsername = gtk_entry_new ();
    gtk_entry_set_max_length (GTK_ENTRY (CreateUsername), 50);
    g_signal_connect (CreateUsername, "activate", G_CALLBACK (PressEnter_Register), CreateUsername);
    gtk_box_pack_start (GTK_BOX (vbox_Register), CreateUsername, TRUE, TRUE, 0);
    gtk_widget_show (CreateUsername);
	
	/* password label */
	passwordText_Register = gtk_label_new("Password");
	gtk_box_pack_start(GTK_BOX(vbox_Register), passwordText_Register, 1, 1, 0);
	
	/* password entry box */
	CreatePassword = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (CreatePassword), 50);
    g_signal_connect (CreatePassword, "activate", G_CALLBACK (PressEnter_Register), CreatePassword);
	gtk_box_pack_start (GTK_BOX (vbox_Register), CreatePassword, TRUE, TRUE, 0);
    gtk_widget_show (CreatePassword);
	
	/* password confirmation label */
	passwordConfirmText_Register = gtk_label_new("Re-enter password");
	gtk_box_pack_start(GTK_BOX(vbox_Register), passwordConfirmText_Register, 1, 1, 0);
	
	/* password confirmation entry box */
	CreatePasswordConfirmation = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (CreatePasswordConfirmation), 50);
	g_signal_connect(CreatePasswordConfirmation, "activate", G_CALLBACK(PressEnter_Register), CreatePasswordConfirmation);
	gtk_box_pack_start(GTK_BOX(vbox_Register), CreatePasswordConfirmation, TRUE, TRUE, 0);
	
	/* packing all boxes */
    hbox_Register = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (vbox_Register), hbox_Register);
    gtk_widget_show (hbox_Register);
	
	/* register click box */
    button_Register = gtk_button_new_with_label("Register");
    g_signal_connect(button_Register, "clicked", G_CALLBACK (PressEnter_Register), registerWindow);
    gtk_box_pack_start (GTK_BOX (vbox_Register), button_Register, TRUE, TRUE, 0);
    gtk_widget_set_can_default (button_Register, TRUE);
    gtk_widget_grab_default (button_Register);
    gtk_widget_show (button_Register);
	
	/* return to login click box */
	button_toLogin = gtk_button_new_with_label("Return");
	g_signal_connect(button_toLogin, "clicked", G_CALLBACK (returnFromRegis), registerWindow);
	gtk_box_pack_start (GTK_BOX (vbox_Register), button_toLogin, TRUE, TRUE, 0);
    gtk_widget_set_can_default (button_toLogin, TRUE);
    gtk_widget_grab_default (button_toLogin);
    gtk_widget_show (button_toLogin);
	
	
	/* connecting signals - registerWindow */
	g_signal_connect(registerWindow, "delete_event", G_CALLBACK(on_delete_event), NULL) ; 
	gtk_widget_set_events(registerWindow, GDK_BUTTON_PRESS_MASK) ;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/* home screen */
	homeLabel = gtk_label_new("Welcome [username]");
	
	/* changes the formatting of the "Welcome to ZotChat" string using Pango Attributes */ 	
	char *stringHome = g_strdup_printf ("<span font=\"22\" color=\"black\">" "<b>Welcome [username]</b>" "</span>");
	gtk_label_set_markup (GTK_LABEL (homeLabel), stringHome);
	g_free (stringHome);
	
    /* create a new homeWindow */
	homeWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(homeWindow, 350, 350);
	gtk_window_set_position(GTK_WINDOW(homeWindow), GTK_WIN_POS_CENTER) ; 
	gtk_window_set_title(GTK_WINDOW(homeWindow), "ZotChat") ;
	gtk_window_set_resizable(GTK_WINDOW(homeWindow), FALSE) ;
	
	/* create log out button */
	GtkWidget *logOutBtn = gtk_button_new_with_label("Log out");
	
	/* testing add friend */
	GtkWidget *addFriendBtn = gtk_button_new_with_label("add");
	gtk_widget_set_size_request(addFriendBtn, 95, 30);
	
	
	/* create a scrollable window for friends list */
	GtkWidget *friendList= gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_size_request(friendList, 150, 250);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(friendList), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);	//only shows vertical scrollbar
	
	/* test button packing */
	GtkWidget *buttonFriend1 = gtk_button_new_with_label("test 1");
	GtkWidget *buttonFriend2 = gtk_button_new_with_label("test 2");
	GtkWidget *buttonFriend3 = gtk_button_new_with_label("test 3");
	GtkWidget *buttonFriend4 = gtk_button_new_with_label("test 4");
	GtkWidget *buttonFriend5 = gtk_button_new_with_label("test 5");
	GtkWidget *buttonFriend6 = gtk_button_new_with_label("test 6");
	GtkWidget *buttonFriend7 = gtk_button_new_with_label("test 7");
	GtkWidget *buttonFriend8 = gtk_button_new_with_label("test 8");
	
	table = gtk_table_new (10, 1, FALSE);
	//gtk_table_set_row_spacings (GTK_TABLE (table), 10);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (friendList), table);		//scrollable table on scrollable window
	
	newTable = gtk_table_new (10, 1, FALSE);						//experimenting
	gtk_table_set_row_spacings (GTK_TABLE (newTable), 10);
	
	gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend1, 0, 1, 0, 1);
	gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend2, 0, 1, 1, 2);
	gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend3, 0, 1, 2, 3);
	gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend4, 0, 1, 3, 4);
	gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend5, 0, 1, 4, 5);
	gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend6, 0, 1, 5, 6);
	gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend7, 0, 1, 6, 7);
	gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend8, 0, 1, 7, 8);
	
	gtk_widget_show(buttonFriend1);
	gtk_widget_show(buttonFriend1);
	gtk_widget_show(buttonFriend3);
	gtk_widget_show(buttonFriend4);
	gtk_widget_show(buttonFriend5);
	gtk_widget_show(buttonFriend6);
	gtk_widget_show(buttonFriend7);
	gtk_widget_show(buttonFriend8);
	
/*--------------------------------------------Johnathan's Edit------------------------------------*/ 
	/************Add  Friend********************/
	GtkWidget *AddFriendButton;
	GtkWidget *mybutton;  
	AddFriendWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
	gtk_widget_set_size_request(GTK_WIDGET(AddFriendWindow), 300,100); 
	gtk_window_set_title (GTK_WINDOW(AddFriendWindow),"Add");
	gtk_window_set_position (GTK_WINDOW(AddFriendWindow), GTK_WIN_POS_CENTER);  
	AddUsername = gtk_entry_new(); 
	gtk_entry_set_max_length(GTK_ENTRY(AddUsername),50); 
	g_signal_connect(AddUsername,"activate",G_CALLBACK(username_enter),AddUsername); 
	
	AddFriendvbox = gtk_vbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(AddFriendWindow),AddFriendvbox); 
	gtk_widget_show(AddFriendvbox);
	gtk_box_pack_start(GTK_BOX(AddFriendvbox),AddUsername,TRUE,TRUE,0); 

	AddFriendhbox = gtk_hbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(AddFriendvbox),AddFriendhbox);
	gtk_widget_show(AddFriendhbox);  
	
	mybutton = gtk_button_new_with_label("Add");
	g_signal_connect(mybutton,"clicked",G_CALLBACK(returnHome),AddFriendWindow); 
	gtk_box_pack_start(GTK_BOX(AddFriendvbox),mybutton,TRUE,TRUE,0); 
	gtk_widget_set_can_default(mybutton,TRUE);
	gtk_widget_grab_default(mybutton); 
	gtk_widget_show(mybutton); 
	gtk_box_pack_start(GTK_BOX(AddFriendvbox),AddUsername,TRUE,TRUE,0); 
	gtk_widget_show(AddUsername);
 
	AddFriendButton = gtk_button_new_with_label("Add a Friend"); 
	gtk_widget_set_size_request(AddFriendButton, 95, 30);
	g_signal_connect(AddFriendButton, "clicked",G_CALLBACK(OpenAddFriend),homeWindow); 
	gtk_widget_show(AddFriendButton);
	g_signal_connect(AddFriendWindow,"delete_event",G_CALLBACK(returnHome),AddFriendWindow); 


	/*****************Accept or Decline********************/

	//FriendRequestLabel = gtk_label_new("NULL"); 
	//char *FRstring = g_strdup_printf("<span font=\"12\" color=\"black\">" "<b>Here are your friend requests:</b>" "</span>"); 
	//gtk_label_set_markup(GTK_LABEL(FriendRequestLabel),FRstring); 
	//g_free(FRstring); 

	FriendRequestWindow = gtk_dialog_new(); 
	gtk_window_set_title(GTK_WINDOW(FriendRequestWindow),"Requests"); 
	gtk_widget_set_size_request(FriendRequestWindow, 250, 150); 
	gtk_window_set_position(GTK_WINDOW(FriendRequestWindow),GTK_WIN_POS_CENTER); 
	
	
	/*AcceptButton = gtk_button_new_with_label("Accept"); 
	g_signal_connect(AcceptButton,"clicked",G_CALLBACK(Accept),"Accept"); 

	DeclineButton = gtk_button_new_with_label("Decline"); 
	g_signal_connect(DeclineButton,"clicked",G_CALLBACK(Decline),"Decline"); 
	
	IgnoreButton = gtk_button_new_with_label("Ignore"); 	
	g_signal_connect(IgnoreButton,"clicked",G_CALLBACK(returnHome),FriendRequestWindow); 
	*/
	
	
	FRList = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_set_border_width (GTK_CONTAINER (FRList), 10);
	
	/* the policy is one of GTK_POLICY AUTOMATIC, or GTK_POLICY_ALWAYS.
     * GTK_POLICY_AUTOMATIC will automatically decide whether you need
     * scrollbars, whereas GTK_POLICY_ALWAYS will always leave the scrollbars
     * there.  The first one is the horizontal scrollbar, the second, 
     * the vertical. */
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (FRList),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    /* The dialog window is created with a vbox packed into it. */								
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG(FriendRequestWindow)->vbox), FRList, 
			TRUE, TRUE, 0);
    gtk_widget_show (FRList);
	
	/* create a table of 10 by 10 squares. */
    table = gtk_table_new (1, 10, FALSE);
    
    /* set the spacing to 10 */
    gtk_table_set_row_spacings (GTK_TABLE (table), 10);
    
    /* pack the table into the scrolled window */
    gtk_scrolled_window_add_with_viewport (
                   GTK_SCROLLED_WINDOW (FRList), table);
    gtk_widget_show (table);
	
	/* this simply creates a grid of toggle buttons on the table
     * to demonstrate the scrolled window. */
    for (i = 0; i < 1; i++)
       for (j = 0; j < 10; j++) {
          sprintf (buffer, "request %d\n", j+1);
		  button = gtk_button_new_with_label (buffer);
		  gtk_table_attach_defaults (GTK_TABLE (table), button,
	                             i, i+1, j, j+1);
		  g_signal_connect(button, "clicked", G_CALLBACK(FriendReq), (gpointer)FriendRequestWindow);
          gtk_widget_show (button);
       }
	
	
	//FriendRequestFixed = gtk_fixed_new(); 
	//gtk_fixed_put(GTK_FIXED(FriendRequestFixed), FriendRequestLabel, 15,20); 
	//gtk_fixed_put(GTK_FIXED(FriendRequestFixed), AcceptButton,50,150); 
	//gtk_fixed_put(GTK_FIXED(FriendRequestFixed), DeclineButton,110,150); 
	//gtk_fixed_put(GTK_FIXED(FriendRequestFixed), IgnoreButton,75,175); 
	//gtk_container_add(GTK_CONTAINER(FriendRequestWindow),FriendRequestFixed); 
	
	/*FRhbox = gtk_hbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(FriendRequestWindow),FRhbox); 
	gtk_widget_show(FRhbox); 
	gtk_box_pack_start(GTK_BOX(FRhbox),AcceptButton,TRUE,TRUE,0); 
	*/
	//gtk_widget_show(AcceptButton); 
	FriendRequestButton = gtk_button_new_with_label("Friend Requests"); 
	gtk_widget_set_size_request(FriendRequestButton, 98, 35);
	g_signal_connect(FriendRequestButton,"clicked",G_CALLBACK(OpenFriendRequest),homeWindow); 
	gtk_widget_show(FriendRequestButton); 
	g_signal_connect(FriendRequestWindow,"delete_event",G_CALLBACK(returnHome),FriendRequestWindow); 

	/*****************Logoff Button **************************/
	
	LogOutButton = gtk_button_new_with_label("Log Out"); 
	gtk_widget_set_size_request(LogOutButton, 70, 35);
	g_signal_connect(LogOutButton,"clicked",G_CALLBACK(LogOut),homeWindow); 

	/*-------------------------------------------------------------------------------------------------*/
	
	/* connecting signals - homeWindow */
	g_signal_connect(homeWindow, "delete_event", G_CALLBACK(on_delete_event), NULL) ; 
	gtk_widget_set_events(homeWindow, GDK_BUTTON_PRESS_MASK) ;
	
	/* connecting signals - logOut */
	g_signal_connect(logOutBtn, "clicked", G_CALLBACK(logOutFunction), NULL);
	
	/* connecting signals - btns */
	g_signal_connect(addFriendBtn, "clicked", G_CALLBACK(addFriend), table);
	g_signal_connect(buttonFriend1, "clicked", G_CALLBACK(removeFriend), table);
	g_signal_connect(buttonFriend2, "clicked", G_CALLBACK(removeFriend), table);
	g_signal_connect(buttonFriend3, "clicked", G_CALLBACK(removeFriend), table);
	g_signal_connect(buttonFriend4, "clicked", G_CALLBACK(removeFriend), table);
	g_signal_connect(buttonFriend5, "clicked", G_CALLBACK(removeFriend), table);
	g_signal_connect(buttonFriend6, "clicked", G_CALLBACK(removeFriend), table);
	g_signal_connect(buttonFriend7, "clicked", G_CALLBACK(removeFriend), table);
	g_signal_connect(buttonFriend8, "clicked", G_CALLBACK(removeFriend), table);
	
	/* fixed placement of widgets */
	fixed = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(fixed), homeLabel, 5, 20);
	gtk_fixed_put(GTK_FIXED(fixed), friendList, 5, 75);
	gtk_fixed_put(GTK_FIXED(fixed), AddFriendButton, 250, 20);
	gtk_fixed_put(GTK_FIXED(fixed), FriendRequestButton, 175,300); 
	gtk_fixed_put(GTK_FIXED(fixed), LogOutButton, 275,300); 
	gtk_container_add(GTK_CONTAINER(homeWindow), fixed);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/* places icon on top left of window */  
	gtk_window_set_icon(GTK_WINDOW(loginInputWindow), icon);
	gtk_window_set_icon(GTK_WINDOW(loginRegisWindow), icon);
	gtk_window_set_icon(GTK_WINDOW(registerWindow), icon);
	gtk_window_set_icon(GTK_WINDOW(homeWindow), icon);
	
	/* displays the loginRegisWindow */
	gtk_widget_show_all(loginRegisWindow);
	
    gtk_main();

    return 0;
}



/* delete event */
static gboolean on_delete_event (GtkWidget *widget, GdkEvent  *event, gpointer data)
{
   /* If you return FALSE in the "delete_event" signal handler,
   * GTK will emit the "destroy" signal. Returning TRUE means
   * you don't want the window to be destroyed.
   *
   * This is useful for popping up 'are you sure you want to quit?'
   * type dialogs.
   */
	printf("delete event occurred\n");
	gtk_main_quit();
	return TRUE;
}

/* if login btn is pressed */
void LoginPressed (GtkButton *button, void *user_data)
{	
	printf("Log In Button Pressed\n");
	resetTextEntry();
	gtk_widget_hide(loginRegisWindow);
	gtk_widget_show_all(loginInputWindow);
}

/* if register btn is pressed */
void RegisterPressed (GtkButton *button, void *user_data)
{
	printf("Register Button Pressed\n");
	resetTextEntry();
	gtk_widget_hide(loginRegisWindow);
	gtk_widget_show_all(registerWindow);
}

void OpenAddFriend(GtkButton *button, void *user_data) 
{
	printf("Add Friend Button Pressed\n");
	gtk_widget_show_all(AddFriendWindow); 
}
void OpenFriendRequest(GtkButton *button, void *user_data)
{
	printf("Friend Request Button Pressed\n"); 
	gtk_widget_show_all(FriendRequestWindow); 
}
void Accept(GtkButton *button, void *user_data)
{
	g_print("Accept Pressed\n"); 
	
}
void LogOut(GtkButton *button, void *user_data) 
{
	gtk_widget_hide(homeWindow); 
	gtk_widget_show_all(loginRegisWindow); 
}
void Decline(GtkButton *button, void *user_data)
{
	g_print("Decline Pressed\n"); 
}

/* if return btn is pressed */
void returnToLoginRegis (GtkButton *button, void *user_data)
{	
	printf("Return Button Pressed\n");
	gtk_widget_hide(loginInputWindow);
	gtk_widget_show_all(loginRegisWindow);
}

void returnHome(GtkButton *button, void *user_data)
{
	gtk_widget_hide(FriendRequestWindow); 
	gtk_widget_hide(AddFriendWindow); 
	gtk_widget_show_all(homeWindow); 
}

/* whenever you press enter on login text entry box */
static void PressEnter_Login( GtkWidget *widget, GtkWidget *entry )
{
  const gchar *entry_text1, *entry_text2;
  entry_text1 = gtk_entry_get_text (GTK_ENTRY (username));
  entry_text2 = gtk_entry_get_text (GTK_ENTRY (password));
  
  char *user = (char*)entry_text1;
  char *pass = (char*)entry_text2;
  if(strcmp(entry_text1, emptyString) == 0 && strcmp(entry_text2, emptyString) == 0)
  {
	  printf("You have not entered a username and password!\n");
  }
  else if(strcmp(entry_text1, emptyString) == 0)
  {
	  printf("You have not entered a username!\n");
  }
  else if(strcmp(entry_text2, emptyString) == 0)
  {
	  printf("You have not entered a password!\n");
  }
  else{
	  printf ("Username: %s\nPassword: %s\n", entry_text1, entry_text2);
	  if(VerifyUserName(user) == 1)
	  {
		if(VerifyPassword(user, pass) == 1)
		{
			printf("You have logged in!\n");
			setUserHomeText(user);
		}
		else
		{
			printf("Incorrect password. Please try again.\n");
		}
	  }
	  else if(strcmp(entry_text1, secretUsername) == 0 && strcmp(entry_text2, secretPassword) == 0)
	  {
		  printf("You have logged in!\n");
		  gtk_main_quit();
	  }
	  else
	  {
		  printf("Incorrect username and/or password. Please try again.\n");
	  }
  }
}

/* if return btn is pressed */
void returnFromRegis (GtkButton *button, void *user_data)
{	
	printf("Return Button Pressed\n");
	gtk_widget_hide(registerWindow);
	gtk_widget_show_all(loginRegisWindow);
}

/* toggle visibility of entry text */
static void entry_toggle_visibility( GtkWidget *checkbutton, GtkWidget *entry )
{
  gtk_entry_set_visibility (GTK_ENTRY (entry), GTK_TOGGLE_BUTTON (checkbutton)->active);
}

/* resetting text entry box */
void resetTextEntry()
{
	/* reset username text entry box */
    gtk_entry_set_text (GTK_ENTRY (username), "");
	
	/* reset password text entry box */
	gtk_entry_set_text (GTK_ENTRY (password), "");
	
	/* reset ALL registration text entry box */
	gtk_entry_set_text (GTK_ENTRY (CreateUsername) , "");
	gtk_entry_set_text (GTK_ENTRY (CreatePassword) , "");
	gtk_entry_set_text (GTK_ENTRY (CreatePasswordConfirmation) , "");
}

/* whenever you press enter on register text entry box */
static void PressEnter_Register( GtkWidget *widget, GtkWidget *entry )
{
  const gchar *entry_text1, *entry_text2, *entry_text3;
  entry_text1 = gtk_entry_get_text (GTK_ENTRY (CreateUsername));
  entry_text2 = gtk_entry_get_text (GTK_ENTRY (CreatePassword));
  entry_text3 = gtk_entry_get_text (GTK_ENTRY (CreatePasswordConfirmation));
  
  char *user = (char*)entry_text1;
  char *pass = (char*)entry_text2;
  
  if(strcmp(entry_text1, emptyString) == 0 && strcmp(entry_text2, emptyString) == 0)
  {
	  printf("You have not entered a username and password!\n");
  }
  else if(strcmp(entry_text1, emptyString) == 0)
  {
	  printf("You have not entered a username!\n");
  }
  else if(strcmp(entry_text2, emptyString) == 0)
  {
	  printf("You have not entered a password!\n");
  }
  else if(strcmp(entry_text2, entry_text3) != 0)
  {
	  printf("Your passwords do not match!\n");
  }
  else{
	  if(VerifyUserName(user) == 1)
	  {
		  printf("This username is taken!\nPlease try another username.\n");
	  }
	  else if(VerifyUserName(user) == 0)
	  {
		  AppendUser(user, pass);
		  printf("Username: %s\nPassword: %s\n", entry_text1, entry_text2);
		  printf("Your account has been successfully created!\n");
		  gtk_widget_hide(registerWindow);
		  gtk_widget_show_all(loginRegisWindow);
	  }
  }
}

/* creates an image from file */
GdkPixbuf *create_pixbuf(const gchar * filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	
	if (!pixbuf) { 
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}

	return pixbuf;
}

/* set the user-specific homeWindow text */
void setUserHomeText(char *name)
{
	loggedInUsername = name;
	
	/* changes the formatting of the "Welcome to ZotChat" string using Pango Attributes */
	char *stringHome = g_strdup_printf ("<span font=\"22\" color=\"black\">" "<b>Welcome %s!</b>" "</span>", loggedInUsername);
	gtk_label_set_markup (GTK_LABEL (homeLabel), stringHome);
	gtk_label_set_justify(GTK_LABEL(homeLabel), GTK_JUSTIFY_CENTER);
    gtk_label_set_line_wrap(GTK_LABEL(homeLabel), TRUE);
	g_free (stringHome);
	
	gtk_widget_hide(loginInputWindow);
	gtk_widget_show_all(homeWindow);
}

/* append friend to contact list */
void addFriend(GtkWidget *button, GtkWidget *table)
{
	char *tmp;																			//testing purposes to print out numbers	
	tmp = (char*)calloc(255, sizeof(char));												//allocs mem for pointer
	sprintf(tmp, "%d", testInt);														//writes int as a *char
	GtkWidget *buttonFriendNew = gtk_button_new_with_label(tmp);						//create a new btn
	gtk_table_attach_defaults (GTK_TABLE (table), buttonFriendNew, 0, 1, addX, addX+1); //adds the btn on the table
	addX++;																				//increments the counter
	testInt++;																			//increments test counter
	free(tmp);																			//free pointer
	g_signal_connect(buttonFriendNew, "clicked", G_CALLBACK(removeFriend), table);		//connect delete friend signal
	gtk_widget_show(buttonFriendNew);
	
}

/* removes friend from contact list */
void removeFriend(GtkWidget *button, GtkWidget *table)
{
	//const gchar *remove = gtk_button_get_label((GtkButton*)button);
	//printf("removing button\n");
	gtk_widget_destroy(button);
	//gtk_container_remove(GTK_CONTAINER(table), button);
	//printf("sucessful btn remove\n");
	//printf("copying tbl\n");
	//gtk_container_foreach(GTK_CONTAINER(table), printTest, table);
	//gtk_container_foreach(GTK_CONTAINER(table), copyTable1, table);
	//printf("sucessful tbl copy\n");
	//printf("removing widgets from old table\n");
	//gtk_container_foreach(GTK_CONTAINER(table), removeWidgets, table);
	//printf("sucessful widget remove\n");
	//removeX = 0;
	//addX = 0;
	//printf("copying table copy over\n");
	//gtk_container_foreach(GTK_CONTAINER(newTable),copyTable2, table);
	//printf("succesfull table copy over\n");
	//printf("removing widgets from temp table\n");
	//gtk_widget_destroy(button+7);
	gtk_widget_show_all(table);
	//gtk_container_foreach(GTK_CONTAINER(newTable), removeWidgets, newTable);
	//printf("successful widget removal END\n");
}

/* test print */
void printTest(GtkWidget *widget, gpointer data)
{
	printf("test %d\n", testInt);
	testInt++;
}

/* log out of homeWindow to loginInputWindow */
void logOutFunction(GtkWidget *button, GtkWidget *widget)
{
	resetTextEntry();
	gtk_widget_hide(homeWindow);
	gtk_widget_show_all(loginInputWindow);
}

void FriendReq(GtkButton *button, void *user_data)
{
  GtkWidget *dialog1;
  dialog1 = gtk_message_dialog_new(GTK_WINDOW(FriendRequestWindow),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_YES_NO,
            "Would you like to accept this friend request?");
  gtk_window_set_title(GTK_WINDOW(dialog1), "Friend Request");
  gint response = gtk_dialog_run(GTK_DIALOG(dialog1)); //stores the enum value for the response
  
  //if yes, another dialog box will be open
  //if no, returns back to the screen
  
  /*if(response == -8)
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
  }*/
}

/* not used */
void copyTable1(GtkWidget *widget, gpointer data)
{
	
	//GtkButton *wid2Btn = (GtkButton*)widget;
	//const gchar *textInsideWidget = gtk_button_get_label(wid2Btn);
	//GtkWidget *tmpWidget = gtk_button_new_with_label(gtk_button_get_label((GtkButton*)widget));
	//if(textInsideWidget != data)
	//{
		gtk_table_attach_defaults(GTK_TABLE(newTable), widget, 0, 1, removeX, removeX+1);
	//}
	//GtkWidget *widget1 = gtk_button_new_with_label("asdf");
	//gtk_table_attach_defaults(GTK_TABLE(table), widget1, 0, 1, removeX, removeX+1);
	removeX++;
}

/* not used */
void removeWidgets(GtkWidget *widget, gpointer data)
{
	gtk_container_remove(GTK_CONTAINER(data), widget);
}

/* not used */
void copyTable2(GtkWidget *widget, gpointer data)
{
	gtk_table_attach_defaults(GTK_TABLE(table), widget, 0, 1, removeX, removeX+1);
	g_signal_connect(widget, "clicked", G_CALLBACK(removeFriend), table);
	gtk_widget_show(widget);
}

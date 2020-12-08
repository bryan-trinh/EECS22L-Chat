/***********************************************************
*
*	Client screen
*	Client.c
*	author: Bryan Trinh
*	initial version: 02/21/19 EECS22L Winter 2019
*
*	Adjusted by Roy, Johnathan, Justin
***********************************************************/
#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "CreateLoginAndVerify.h"
#include "chess.h"

#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/select.h>
#include <arpa/inet.h>

#include <glib.h>
#include <gio/gio.h>

#include <sys/wait.h>
#include <sys/user.h>

/* kevins code */
//#define DEBUG
#define STR_LEN 64
#define MSG_LEN 1024

int Exit = 0;
int Login=0;
pid_t pid;
char message[MSG_LEN] = {0};
int addFriendFlagSucc = 0;
int chatFlagSucc = 0;

/* functions */
static gboolean on_delete_event (GtkWidget *widget, GdkEvent  *event, gpointer data);
void LoginPressed (GtkButton *button, void *user_data);
void RegisterPressed (GtkButton *button, void *user_data);
void returnToLoginRegis (GtkButton *button, void *user_data);
void returnHome (GtkButton *button, void *user_data); 
void OpenAddFriend(GtkButton *button, void *user_data); 
void OpenFriendChat(GtkButton *button, void *user_data); 
void OpenFriendRequest(GtkButton *button, void *user_data); 
void Accept(GtkButton *button, void *user_data); 
void Decline(GtkButton *button, void *user_data);
void LogOutBtnFunct(GtkButton *button, void *user_data);  
static void PressEnter_Login( GtkWidget *widget, GtkWidget *entry );
static void msg_enter(GtkWidget *widget, GtkWidget *entry);
static void entry_toggle_visibility( GtkWidget *checkbutton, GtkWidget *entry );
void resetTextEntry();
void resetMsgEntry();
static void PressEnter_Register( GtkWidget *widget, gpointer user_data );
void returnFromRegis (GtkButton *button, void *user_data);
GdkPixbuf *create_pixbuf(const gchar * filename);
void setUserHomeText(char *name);
// void addFriend(const gchar *text);
void removeFriend(GtkWidget *button, GtkButton *user_data);
// void logOutFunction(GtkWidget *button, GtkWidget *widget);
void FriendReq(GtkButton *button, void *user_data);
void ExitFriendReqDialog(GtkWidget *widget, gpointer *user_data);
void OpenChessWindow(GtkButton *button, void *user_data); 
void copyTable1(GtkWidget *widget, gpointer data);
void removeWidgets(GtkWidget *widget, gpointer data);
void copyTable2(GtkWidget *widget, gpointer data);
void printTest(GtkWidget *widget, gpointer data);
void receive_chat(char msgrec[MSG_LEN]);
void changePassword(GtkWidget *widget, gpointer data);
void returnFromChangePassword(GtkWidget *widget, gpointer data);

void Chat();
int AddFriend();
void LogOut();
int DeleteFriend();
void ChangePassword();
void HandlePulling();
void stupidReset();


gboolean
network_read(GIOChannel *source,
             GIOCondition cond,
             gpointer data);
			 gboolean
new_connection(GSocketService *service,
              GSocketConnection *connection,
              GObject *source_object,
              gpointer user_data);


/* global vars */
GtkWidget *loginRegisWindow;				//main loginRegisWindow
GtkWidget *loginInputWindow;				//secondary loginInputWindow
GtkWidget *registerWindow;					//secondary registerWindow
GtkWidget *homeWindow;						//main homeWindow
GtkWidget *homeLabel; 						//label text
GtkWidget *fixedtitle;
GtkWidget *fixedhome;
GtkWidget *usernameTextEntry;
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
GtkWidget *changePasswordWindow;
GtkWidget *changePassEText;


GtkWidget *FriendChatFixed;
/* list of moved functions from global to local


GtkWidget *text_view;
GtkWidget *scrolled_window; 
//GtkWidget *mtable; not used somehow
//GtkWidget *receivemsg; not used
*/
GtkWidget *FriendRequestFixed;
GtkWidget *LogOutButton;  
GtkWidget *FRList;
GtkWidget *table;
GtkWidget *button;
GtkWidget *FRbutton;
GtkWidget *FriendRequestDialog;
GtkWidget *FriendChatWindow;

GtkWidget *homelay;
GtkWidget *titlelay;


GtkTextBuffer *SendBuffer; 
GtkTextBuffer *ReceiveBuffer;
GtkTextMark *mark;
GtkTextIter iter; 
GtkTextIter start;
GtkTextIter end; 

GtkWidget *send_view; 
GtkWidget *receive_view; 
GtkWidget *send_scrolled_window; 
GtkWidget *receive_scrolled_window; 

//GtkWidget *table2;
GtkWidget *newTable;
const char *emptyString = "";
const char *secretUsername = "bryan";
const char *secretPassword = "password123";
GtkWidget *CreateUsername;
GtkWidget *CreatePassword;
GtkWidget *CreatePasswordConfirmation;
GtkWidget *AddUsername;
GtkWidget *msg;
GtkWidget *friendName;
GtkButton *lastFriendClicked;

GtkTextMark *receivemark;
	GtkTextIter receiveiter; 

int flag = 0;
int msgFlag = 0;
int addFriendFlag = 0;

char *loggedInUsername = "";
int addX = 8;
int removeX = 0;
int testInt = 0;
char bufferGTK[32];
int i, j;
int ChatX = 4;

char friend[STR_LEN] = {0};
const gchar *tmpFriend;

char usernameH[STR_LEN] = {0};

  char username[STR_LEN] = {0};
  char p[STR_LEN] = {0};
  char success_fail[STR_LEN] = {0};
  char reason[STR_LEN] = {0};
  int found;
  
  char usernameMSG[STR_LEN] = {0};
  char messageSent[STR_LEN] = {0};
  
  char addFriendEntry[STR_LEN] = {0};
  char changePassEntry[STR_LEN] = {0};
  
  char regisUser[STR_LEN] = {0};
  char regisPass[STR_LEN] = {0};
  
  char buffer[MSG_LEN];
	char buffer2[MSG_LEN] = "this would be the username";
	char buffer3[MSG_LEN] = "this would be the password";

int client_fd, connect_fd;
    int portno;
    struct hostent *server_name;
	struct sockaddr_in server_addr; /* the server address going to connect with */
	
const gchar *lastUserClicked;
void stall(GtkWidget *widget, gpointer data);
void HandleServer(int client_fd, char* buffer, int buffer_size);
static void password_change_enter(GtkWidget *widget, gpointer data);

fd_set active_fds, read_fds;
int select_res;
int w_or_r;
void test(GtkWidget *widget, gpointer data);


static void username_enter(GtkWidget *widget, GtkWidget *entry)
{
	const gchar *entry1; 
	entry1 = gtk_entry_get_text(GTK_ENTRY(AddUsername));
	if(strcmp(entry1,emptyString) == 0)
	{
		g_print("You did not enter a username\n"); 
	}
	else
	{
		memset(addFriendEntry, '\0', STR_LEN);
		memset(buffer,'\0',MSG_LEN);
		memset(buffer2,'\0',MSG_LEN);
		memset(buffer3,'\0',MSG_LEN);
		
		char *user = (char*)entry1;
		
		size_t userLength = strlen(entry1);
		
		strncpy(buffer2, user, sizeof buffer2 - 1);
		buffer2[userLength] = '\0';
		
		char *temp = "$addfriend";
		strncpy(buffer, temp, sizeof buffer -1);
		buffer[10] = '\0';
		//strncpy(addFriendEntry, user, sizeof addFriendEntry - 1);
		//addFriendEntry[userLength] = '\0';
		int succOrNot = AddFriend();
		succOrNot++;
		succOrNot--;
		flag = 1;
		
			GtkWidget *buttonFriendNew = gtk_button_new_with_label(entry1);							//create a new btn
			gtk_widget_set_size_request(buttonFriendNew, 130, 20);									//resizes
			gtk_table_attach_defaults (GTK_TABLE (table), buttonFriendNew, 0, 1, addX, addX+1); 	//adds the btn on the table
			addX++;																					//increments the counter
			testInt++;																				//increments test counter
			g_signal_connect(buttonFriendNew, "clicked", G_CALLBACK(OpenFriendChat), table);		//connect delete friend signal
			gtk_widget_show(buttonFriendNew);
			
			
			g_print("You've added: %s\n",entry1); 
			gtk_widget_hide(AddFriendWindow);
			gtk_widget_show_all(homeWindow);
		//stupidReset();
	}
	resetTextEntry();
}

void chat_enter() 
{
	const gchar *chatentry; 
	chatentry = gtk_entry_get_text(GTK_ENTRY(msg)); 
	if (strcmp(chatentry,emptyString) == 0)
	{
		g_print("You did not type a chat\n"); 
	}
	else 
	{
		
		mark = gtk_text_buffer_get_insert(ReceiveBuffer); 
		gtk_text_buffer_get_iter_at_mark(ReceiveBuffer,&iter,mark); 
		if(gtk_text_buffer_get_char_count(ReceiveBuffer))
		{
			gtk_text_buffer_insert(ReceiveBuffer,&iter,"\n",1);
		}
		gtk_text_buffer_get_end_iter(SendBuffer,&end); 
		gtk_text_buffer_insert(ReceiveBuffer,&iter,chatentry,-1); 
		gtk_text_buffer_insert(SendBuffer,&end,"You:\n",-1); 
		
	}

}
int doOnce = 0;
/* main */
int main( int   argc, char *argv[] )
{
	g_thread_init(NULL);
	gdk_threads_init();
	gtk_init (&argc, &argv);
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	char* program_name = argv[0];
	program_name++;
	program_name--;
#ifdef DEBUG
    printf("Number of Argc : %d \n", argc);
    printf(" Starting %s...\n", program_name);
    int i = 0;
    for(i= 1; i <argc; i++) {
        printf("The %d argument is : %s \n", i,argv[i]);
    }
#endif
    
	/*************************** CONNECTING TO SERVER ********************************************************************/
	
	if(doOnce == 0)
	{
		/*Autobinds port to 5050 if not defined */
		/*If defined then define to port number set */
		if(argc < 3 ) {
			printf("Autobinding port to #5050 and using local computer ip \n");
			portno = 5050;
		} else {
			server_name = gethostbyname(argv[1]);
			portno = atoi(argv[2]);
			if(portno <= 2000) { fprintf(stderr,"Port Number should be greater than 2000. Closing\n"); return 0;}
		}
		
		/*initialize server ports and location */  
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(portno); 
		/*server address declared or assume that server addr = local addr */
		if(server_name == NULL )
		{
			server_addr.sin_addr = *(struct in_addr*)server_name->h_addr_list[0];
		} else {
			server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			printf("local ip address: 127.0.0.1 was used \n");
		};
		printf("ip binding\n"); /***/
		/* Create Client Socket */
		
		client_fd = socket(AF_INET, SOCK_STREAM, 0);
		
		/*connecting*/
		connect_fd = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
		if(connect_fd < 0) { perror("cant connect with server quitting..."); exit(2);}
		
		
		// printf("new service\n");
		// GSocketService *service = g_socket_service_new();
		// printf("new InetAddr\n");
		//GInetAddress *address = server_addr.sin_addr.s_addr;
		// GInetAddress *address = g_inet_address_new_from_string("127.0.0.1");
		// printf("new socket addr\n");
		//GSocketAddress *socket_address = client_fd;
		// GSocketAddress *socket_address = g_inet_socket_address_new(address, 5004);
		// printf("listner proc\n");
	// g_socket_listener_add_address(G_SOCKET_LISTENER(service), socket_address, G_SOCKET_TYPE_STREAM,
			  // G_SOCKET_PROTOCOL_TCP, NULL, NULL, NULL);

	  // printf("unref sock addr\n");
	  // g_object_unref(socket_address);
	  // printf("unref addr\n");
	  // g_object_unref(address);
	  // printf("start service\n");
	  // g_socket_service_start(service);

	  // g_signal_connect(service, "incoming", G_CALLBACK(new_connection), NULL);
		/********************************************************now connected ************************************************/
	#ifdef DEBUG	
		printf("I AM NOW WITH THE SERVER!!!! \n");
	#endif	
		doOnce++;
	}
	
	/********************************** RECIEVING WELCOME MESSAGE (SHOULD EVENTUALLY GO IN A FUNCTION -KEVIN) *****************************************/
	
	/*chatting */
	int w_or_r; /*integer to see if there is an invalid output or input from write or read */
	
	/* READING WELCOME MESSAGE*/
	w_or_r = read(client_fd, buffer, MSG_LEN);
	if((w_or_r)<0) { perror("recieve "); exit(10);}
	
	if(buffer[0] != '\n') {
		buffer[strcspn(buffer, "\r\n")] = 0;
	}
	
	printf("The welcome message recieved was %s \n \n \n \n",buffer);
	
/********************************GTK STUFF*************************************************/
	
	//gtk_init (&argc, &argv);

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
	g_signal_connect(loginRegisWindow, "delete_event", G_CALLBACK(on_delete_event), loop) ; 
	gtk_widget_set_events(loginRegisWindow, GDK_BUTTON_PRESS_MASK) ;
	
	/* connecting signals - btns */
	g_signal_connect(loginBtn, "clicked", G_CALLBACK(LoginPressed), loginRegisWindow);
	g_signal_connect(registerBtn, "clicked", G_CALLBACK(RegisterPressed), loginRegisWindow);
	
		GtkWidget *background1 = gtk_image_new_from_file("./images/background.jpg");
	
	GdkColor blackcolor, whitecolor;
	gdk_color_parse("black", &blackcolor);
	gdk_color_parse("white", &whitecolor);
	
	gtk_widget_modify_bg(GTK_WIDGET(loginBtn), GTK_STATE_PRELIGHT, &blackcolor);
	GtkWidget *loginChild = gtk_bin_get_child(GTK_BIN(loginBtn));
	gtk_widget_modify_fg(GTK_WIDGET(loginChild), GTK_STATE_PRELIGHT, &whitecolor);
	
	gtk_widget_modify_bg(GTK_WIDGET(registerBtn), GTK_STATE_PRELIGHT, &blackcolor);
	GtkWidget *registerChild = gtk_bin_get_child(GTK_BIN(registerBtn));
	gtk_widget_modify_fg(GTK_WIDGET(registerChild), GTK_STATE_PRELIGHT, &whitecolor);
	
	/* fixed placement of widgets */
	titlelay = gtk_layout_new(NULL,NULL);
	gtk_layout_put(GTK_LAYOUT(titlelay), background1, 0, 0);
	gtk_layout_put(GTK_LAYOUT(titlelay), label, 25, 50);
	gtk_layout_put(GTK_LAYOUT(titlelay), titleLogo, 100, 100);
	gtk_layout_put(GTK_LAYOUT(titlelay), loginBtn, 95, 275);
	gtk_layout_put(GTK_LAYOUT(titlelay), registerBtn, 195, 275);
	gtk_container_add(GTK_CONTAINER(loginRegisWindow), titlelay);


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
    usernameTextEntry = gtk_entry_new ();
    gtk_entry_set_max_length (GTK_ENTRY (usernameTextEntry), 50);
    g_signal_connect (usernameTextEntry, "activate", G_CALLBACK (PressEnter_Login), NULL);
    gtk_box_pack_start (GTK_BOX (vbox_Login), usernameTextEntry, TRUE, TRUE, 0);
    gtk_widget_show (usernameTextEntry);
	
	/* password label */
	passwordText_Login = gtk_label_new("Password");
	gtk_box_pack_start(GTK_BOX(vbox_Login), passwordText_Login, 1, 1, 0);
	
	/* password entry box */
	password = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (password), 50);
    g_signal_connect (password, "activate", G_CALLBACK (PressEnter_Login), NULL);
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
	g_signal_connect(loginInputWindow, "delete_event", G_CALLBACK(on_delete_event), loop) ; 
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
    g_signal_connect (CreateUsername, "activate", G_CALLBACK (PressEnter_Register), NULL);
    gtk_box_pack_start (GTK_BOX (vbox_Register), CreateUsername, TRUE, TRUE, 0);
    gtk_widget_show (CreateUsername);
	
	/* password label */
	passwordText_Register = gtk_label_new("Password");
	gtk_box_pack_start(GTK_BOX(vbox_Register), passwordText_Register, 1, 1, 0);
	
	/* password entry box */
	CreatePassword = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (CreatePassword), 50);
    g_signal_connect (CreatePassword, "activate", G_CALLBACK (PressEnter_Register), NULL);
	gtk_box_pack_start (GTK_BOX (vbox_Register), CreatePassword, TRUE, TRUE, 0);
    gtk_widget_show (CreatePassword);
	
	/* password confirmation label */
	passwordConfirmText_Register = gtk_label_new("Re-enter password");
	gtk_box_pack_start(GTK_BOX(vbox_Register), passwordConfirmText_Register, 1, 1, 0);
	
	/* password confirmation entry box */
	CreatePasswordConfirmation = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (CreatePasswordConfirmation), 50);
	g_signal_connect(CreatePasswordConfirmation, "activate", G_CALLBACK(PressEnter_Register), NULL);
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
	g_signal_connect(registerWindow, "delete_event", G_CALLBACK(on_delete_event), loop) ; 
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
	
	// /* create log out button */
	// GtkWidget *logOutBtn = gtk_button_new_with_label("Log out");
	
	/* testing add friend */
	// GtkWidget *addFriendBtn = gtk_button_new_with_label("add");
	// gtk_widget_set_size_request(addFriendBtn, 95, 30);
	
	
	/* create a scrollable window for friends list */
	GtkWidget *friendList= gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_size_request(friendList, 150, 250);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(friendList), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);	//only shows vertical scrollbar
	
	/* test button packing */
	// GtkWidget *buttonFriend1 = gtk_button_new_with_label("hellolad");
	// GtkWidget *buttonFriend2 = gtk_button_new_with_label("potato");
	// GtkWidget *buttonFriend3 = gtk_button_new_with_label("STALL");
	// GtkWidget *buttonFriend4 = gtk_button_new_with_label("");
	// GtkWidget *buttonFriend5 = gtk_button_new_with_label("");
	// GtkWidget *buttonFriend6 = gtk_button_new_with_label("");
	// GtkWidget *buttonFriend7 = gtk_button_new_with_label("");
	// GtkWidget *buttonFriend8 = gtk_button_new_with_label("");
	
	/*
	gtk_widget_set_size_request(buttonFriend1, 130, 20);
	gtk_widget_set_size_request(buttonFriend2, 130, 20);
	gtk_widget_set_size_request(buttonFriend3, 130, 20);
	gtk_widget_set_size_request(buttonFriend4, 130, 20);
	gtk_widget_set_size_request(buttonFriend5, 130, 20);
	gtk_widget_set_size_request(buttonFriend6, 130, 20);
	gtk_widget_set_size_request(buttonFriend7, 130, 20);
	gtk_widget_set_size_request(buttonFriend8, 130, 20);
	*/
	table = gtk_table_new (10, 1, FALSE);
	//gtk_table_set_row_spacings (GTK_TABLE (table), 10);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (friendList), table);		//scrollable table on scrollable window
	
	// gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend1, 0, 1, 0, 1);
	// gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend2, 0, 1, 1, 2);
	// gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend3, 0, 1, 2, 3);
	// gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend4, 0, 1, 3, 4);
	// gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend5, 0, 1, 4, 5);
	// gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend6, 0, 1, 5, 6);
	// gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend7, 0, 1, 6, 7);
	// gtk_table_attach_defaults (GTK_TABLE (table), buttonFriend8, 0, 1, 7, 8);
	
	// gtk_widget_show(buttonFriend1);
	// gtk_widget_show(buttonFriend2);
	// gtk_widget_show(buttonFriend3);
	// gtk_widget_show(buttonFriend4);
	// gtk_widget_show(buttonFriend5);
	// gtk_widget_show(buttonFriend6);
	// gtk_widget_show(buttonFriend7);
	// gtk_widget_show(buttonFriend8);
	
/*--------------------------------------------Johnathan's Edit------------------------------------*/ 
	/************Add  Friend********************/
	GtkWidget *AddFriendButton;
	GtkWidget *mybutton;  
	
	/* creating AddFriendWindow */
	AddFriendWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL); 
	gtk_widget_set_size_request(GTK_WIDGET(AddFriendWindow), 300,100); 
	gtk_window_set_title (GTK_WINDOW(AddFriendWindow),"Add");
	gtk_window_set_position (GTK_WINDOW(AddFriendWindow), GTK_WIN_POS_CENTER);  
	
	/* creating AddFriend vbox */
	AddFriendvbox = gtk_vbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(AddFriendWindow),AddFriendvbox); 
	gtk_widget_show(AddFriendvbox);
	
	/* AddFriendWindow entry text */
	AddUsername = gtk_entry_new(); 
	gtk_entry_set_max_length(GTK_ENTRY(AddUsername),50); 
	g_signal_connect(AddUsername,"activate",G_CALLBACK(username_enter),AddUsername); 
	gtk_box_pack_start(GTK_BOX(AddFriendvbox),AddUsername,TRUE,TRUE,0); 
	gtk_widget_show(AddUsername);
	
	/* AddFriendWindow add btn */
	mybutton = gtk_button_new_with_label("Add");
	g_signal_connect(mybutton,"clicked",G_CALLBACK(username_enter),AddFriendWindow); 
	gtk_box_pack_start(GTK_BOX(AddFriendvbox),mybutton,TRUE,TRUE,0); 
	gtk_widget_set_can_default(mybutton,TRUE);
	gtk_widget_grab_default(mybutton); 
	gtk_widget_show(mybutton);
 
	/* addFriend btn for homeWindow */
	AddFriendButton = gtk_button_new_with_label("Add a Friend"); 
	gtk_widget_set_size_request(AddFriendButton, 80, 35);
	g_signal_connect(AddFriendButton, "clicked",G_CALLBACK(OpenAddFriend),homeWindow); 
	gtk_widget_show(AddFriendButton);
	
	/* connecting delete signal for AddFriendWindow */
	g_signal_connect(AddFriendWindow,"delete_event",G_CALLBACK(returnHome),AddFriendWindow); 

	AddFriendhbox = gtk_hbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(AddFriendvbox),AddFriendhbox);
	gtk_widget_show(AddFriendhbox);
	
/*--------------------------------------------Justin's Edit------------------------------------*/ 
		
		/**************** Friend Chat Window ****************/

			/* setting up the Friend Chat pop-up window*/


	
	


	FriendChatWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(FriendChatWindow,420,420); 
	gtk_window_set_title (GTK_WINDOW(FriendChatWindow),"Friend");
	gtk_window_set_position (GTK_WINDOW(FriendChatWindow), GTK_WIN_POS_CENTER);  
    gtk_window_set_resizable(GTK_WINDOW(FriendChatWindow), FALSE) ;

	/*gtk_window_set_icon(GTK_WINDOW(FriendChatWindow), icon);*/

	/* create button printouts */ 
	GtkWidget *SendButton = gtk_button_new_with_label("Send"); 
	gtk_widget_set_size_request(SendButton, 75, 40);
	
	GtkWidget *UnfriendButton = gtk_button_new_with_label("Unfriend");
	gtk_widget_set_size_request(UnfriendButton, 65, 30);

	GtkWidget *ReturnButton = gtk_button_new_with_label("Return");
	gtk_widget_set_size_request(ReturnButton, 65, 35);

	// GtkWidget *ChessRequestButton = gtk_button_new_with_label("Chess\nRequest");
	// gtk_widget_set_size_request(ChessRequestButton, 60, 50);

	/* create scroll window for message (ChatWindow)*/
	// GtkWidget *ChatList = gtk_scrolled_window_new(NULL,NULL);
	// gtk_widget_set_size_request(ChatList, 330, 290);
	// gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(ChatList), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	
	/* message entry text */
	msg = gtk_entry_new(); 
	gtk_entry_set_max_length(GTK_ENTRY(msg),500); 
	gtk_widget_set_size_request(msg, 330, 40);
	g_signal_connect(msg,"activate",G_CALLBACK(msg_enter),msg);
	gtk_widget_show(msg);

	//gtk_table_set_row_spacings (GTK_TABLE (newTable), 4);

	/* button signal connections */
	g_signal_connect(FriendChatWindow, "delete_event", G_CALLBACK(returnHome), FriendChatWindow);
	gtk_widget_set_events(FriendChatWindow, GDK_BUTTON_PRESS_MASK) ;

	//Send button not connected currently
	gtk_widget_show(SendButton);
	g_signal_connect(SendButton, "clicked", G_CALLBACK(msg_enter), msg);
	
	g_signal_connect(UnfriendButton, "clicked", G_CALLBACK(removeFriend), lastFriendClicked);
	gtk_widget_show(UnfriendButton);
	//unfriend not connected
	g_signal_connect(ReturnButton, "clicked", G_CALLBACK(returnHome), FriendChatWindow);
	gtk_widget_show(ReturnButton);
	//Chessrequest not connected
	// g_signal_connect(ChessRequestButton, "clicked", G_CALLBACK(chess_window), FriendChatWindow);
	// gtk_widget_show(ChessRequestButton);
	
	send_view = gtk_text_view_new_with_buffer (SendBuffer);
	gtk_widget_set_size_request(send_view,50,300); 
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (send_view), GTK_WRAP_WORD); 
	
	send_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (send_scrolled_window), 
                                  GTK_POLICY_AUTOMATIC, 
                                  GTK_POLICY_AUTOMATIC); 
								  
		gtk_text_view_set_editable(GTK_TEXT_VIEW(send_view), FALSE);
		
		gtk_text_view_set_justification(GTK_TEXT_VIEW(send_view),GTK_JUSTIFY_RIGHT);

		gtk_container_add (GTK_CONTAINER (send_scrolled_window), 
                                        send_view);
		gtk_container_set_border_width (GTK_CONTAINER (send_view), 1);
	SendBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (send_view));

	receive_view = gtk_text_view_new_with_buffer(ReceiveBuffer); 
		gtk_widget_set_size_request(receive_view,250,300); 
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(receive_view),GTK_WRAP_WORD); 
		receive_scrolled_window = gtk_scrolled_window_new(NULL,NULL); 
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (receive_scrolled_window), 
                                  GTK_POLICY_AUTOMATIC, 
                                  GTK_POLICY_AUTOMATIC); 
		
		gtk_text_view_set_editable(GTK_TEXT_VIEW(receive_view),FALSE); 
		gtk_container_add (GTK_CONTAINER(receive_scrolled_window),receive_view);
		gtk_container_set_border_width(GTK_CONTAINER(receive_scrolled_window),1); 
		
		ReceiveBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(receive_view)); 
	
	
    FriendChatFixed = gtk_layout_new(NULL, NULL);
	GtkWidget *StallButton = gtk_button_new_with_label("STALL"); 
	g_signal_connect(StallButton,"clicked",G_CALLBACK(stall),FriendChatWindow);
	gtk_widget_set_size_request(StallButton, 50, 30);
	
	
	friendName = gtk_label_new("Insert Name");
   
	GtkWidget *backgroundFriendChat = gtk_image_new_from_file("./images/bigback.jpg");
    
	gtk_layout_put(GTK_LAYOUT(FriendChatFixed), backgroundFriendChat, 0,0); 
	gtk_layout_put(GTK_LAYOUT(FriendChatFixed), friendName, 5, 10);
	gtk_layout_put(GTK_LAYOUT(FriendChatFixed), send_scrolled_window, 5, 60);
	gtk_layout_put(GTK_LAYOUT(FriendChatFixed), receive_scrolled_window,70,60); 
	//gtk_layout_put(GTK_LAYOUT(FriendChatFixed), StallButton, 355, 200); 
	gtk_layout_put(GTK_LAYOUT(FriendChatFixed), UnfriendButton, 345, 7);
	// gtk_layout_put(GTK_LAYOUT(FriendChatFixed), ChessRequestButton, 347,150); 
	gtk_layout_put(GTK_LAYOUT(FriendChatFixed), SendButton, 335,370); 
	gtk_layout_put(GTK_LAYOUT(FriendChatFixed), msg, 5,370); 
	gtk_container_add(GTK_CONTAINER(FriendChatWindow), FriendChatFixed); 

	/*****************Accept or Decline Friend Request Button********************/

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
    newTable = gtk_table_new (1, 10, FALSE);
    
    /* set the spacing to 10 */
    gtk_table_set_row_spacings (GTK_TABLE (newTable), 10);
    
    /* pack the table into the scrolled window */
    gtk_scrolled_window_add_with_viewport (
                   GTK_SCROLLED_WINDOW (FRList), newTable);
    gtk_widget_show (newTable);
	
	/* this simply creates a grid of toggle buttons on the table
     * to demonstrate the scrolled window. */
    for (i = 0; i < 1; i++)
       for (j = 0; j < 10; j++) {
          sprintf (bufferGTK, "testing%d", j+1);
		  FRbutton = gtk_button_new_with_label (bufferGTK);
		  gtk_table_attach_defaults (GTK_TABLE (newTable), FRbutton,
	                             i, i+1, j, j+1);
		  g_signal_connect(FRbutton, "clicked", G_CALLBACK(FriendReq), NULL);
          gtk_widget_show (FRbutton);
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
	g_signal_connect(LogOutButton,"clicked",G_CALLBACK(LogOutBtnFunct),homeWindow); 

	/*-------------------------------------------------------------------------------------------------*/
	
	/* connecting signals - homeWindow */
	g_signal_connect(homeWindow, "delete_event", G_CALLBACK(on_delete_event), loop) ; 
	gtk_widget_set_events(homeWindow, GDK_BUTTON_PRESS_MASK) ;
	
	/* connecting signals - logOut */
	// g_signal_connect(logOutBtn, "clicked", G_CALLBACK(logOutFunction), NULL);
	
	/* connecting signals - btns */
	// g_signal_connect(addFriendBtn, "clicked", G_CALLBACK(addFriend), table);
	// g_signal_connect(buttonFriend1, "clicked", G_CALLBACK(OpenFriendChat), table);
	// g_signal_connect(buttonFriend2, "clicked", G_CALLBACK(OpenFriendChat), table);
	// g_signal_connect(buttonFriend3, "clicked", G_CALLBACK(stall), table);
	
	GtkWidget *chessbtn, *cheimg;
	cheimg = gtk_image_new_from_file("./images/che.png");
	chessbtn = gtk_button_new();
	gtk_widget_set_size_request(chessbtn, 105, 95);
	
	//gtk_button_set_always_show_image(GTK_BUTTON(chessbtn), TRUE);
	gtk_button_set_image(GTK_BUTTON(chessbtn), cheimg);
	g_signal_connect(chessbtn, "clicked", G_CALLBACK(chess_window), homeWindow);
	
	GtkWidget *backgroundHome = gtk_image_new_from_file("./images/background.jpg");
	
	/* changePasswordWindow */
	GtkWidget *changePasswordBtn = gtk_button_new_with_label("Change\nPassword");
	gtk_widget_set_size_request(changePasswordBtn, 70, 35);
	g_signal_connect(changePasswordBtn, "clicked", G_CALLBACK(changePassword), NULL);
	
	changePasswordWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(changePasswordWindow),"Change Password"); 
	gtk_widget_set_size_request(changePasswordWindow, 250, 150); 
	gtk_window_set_position(GTK_WINDOW(changePasswordWindow),GTK_WIN_POS_CENTER); 
	
	/* creating changePass vbox */
	GtkWidget *changePassvbox, *changePasshbox;
	changePassvbox = gtk_vbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(changePasswordWindow),changePassvbox); 
	gtk_widget_show(changePassvbox);
	
	/* changePasswordWindow entry text */
	changePassEText = gtk_entry_new(); 
	gtk_entry_set_max_length(GTK_ENTRY(changePassEText),50); 
	g_signal_connect(changePassEText,"activate",G_CALLBACK(password_change_enter),changePassEText); 
	gtk_box_pack_start(GTK_BOX(changePassvbox),changePassEText,TRUE,TRUE,0); 
	gtk_widget_show(changePassEText);
	
	/* changePasswordWindow add btn */
	GtkWidget *passchange;
	passchange = gtk_button_new_with_label("Change Password");
	g_signal_connect(passchange,"clicked",G_CALLBACK(password_change_enter),changePasswordWindow); 
	gtk_box_pack_start(GTK_BOX(changePassvbox),passchange,TRUE,TRUE,0); 
	gtk_widget_set_can_default(passchange,TRUE);
	gtk_widget_grab_default(passchange); 
	gtk_widget_show(passchange);
	
	/* connecting delete signal for changePasswordWindow */
	g_signal_connect(changePasswordWindow,"delete_event",G_CALLBACK(returnFromChangePassword),changePasswordWindow); 

	changePasshbox = gtk_hbox_new(FALSE,0); 
	gtk_container_add(GTK_CONTAINER(changePassvbox),changePasshbox);
	gtk_widget_show(changePasshbox);
	
	/* fixed placement of widgets */
	homelay = gtk_layout_new(NULL,NULL);
	gtk_layout_put(GTK_LAYOUT(homelay), backgroundHome, 0, 0);
	gtk_layout_put(GTK_LAYOUT(homelay), changePasswordBtn, 275, 255);
	gtk_layout_put(GTK_LAYOUT(homelay), homeLabel, 5, 20);
	gtk_layout_put(GTK_LAYOUT(homelay), friendList, 5, 75);
	//gtk_layout_put(GTK_LAYOUT(homelay), AddFriendButton, 250, 20);
	gtk_layout_put(GTK_LAYOUT(homelay), AddFriendButton, 175, 300);
	gtk_layout_put(GTK_LAYOUT(homelay), chessbtn, 200, 110);
	//gtk_layout_put(GTK_LAYOUT(homelay), FriendRequestButton, 175,300); 
	gtk_layout_put(GTK_LAYOUT(homelay), LogOutButton, 275,300); 
	gtk_container_add(GTK_CONTAINER(homeWindow), homelay);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/* places icon on top left of window */  
	gtk_window_set_icon(GTK_WINDOW(loginInputWindow), icon);
	gtk_window_set_icon(GTK_WINDOW(loginRegisWindow), icon);
	gtk_window_set_icon(GTK_WINDOW(registerWindow), icon);
	gtk_window_set_icon(GTK_WINDOW(homeWindow), icon);
	
	
	gtk_window_set_icon(GTK_WINDOW(AddFriendWindow), icon);
	gtk_window_set_icon(GTK_WINDOW(FriendRequestWindow), icon);
	gtk_window_set_icon(GTK_WINDOW(changePasswordWindow), icon);
	gtk_window_set_icon(GTK_WINDOW(FriendChatWindow), icon);
	
	/* displays the loginRegisWindow */
	gtk_widget_show_all(loginRegisWindow);
	
	FD_ZERO(&active_fds);
	FD_SET(client_fd, &active_fds);
	//FD_SET(STDIN_FILENO, &active_fds);
	
	//gint fd = client_fd;
	/*
	GIOChannel *channel = g_io_channel_unix_new(fd);
	g_io_add_watch
	*/
	//stupidReset();
	// printf("new service\n");
	// GSocketService *service = g_socket_service_new();
	// printf("new InetAddr\n");
	//GInetAddress *address = server_addr.sin_addr.s_addr;
	// GInetAddress *address = g_inet_address_new_from_string("127.0.0.1");
	// printf("new socket addr\n");
	//GSocketAddress *socket_address = client_fd;
	// GSocketAddress *socket_address = g_inet_socket_address_new(address, 5004);
	// printf("listner proc\n");
// g_socket_listener_add_address(G_SOCKET_LISTENER(service), socket_address, G_SOCKET_TYPE_STREAM,
          // G_SOCKET_PROTOCOL_TCP, NULL, NULL, NULL);

  // printf("unref sock addr\n");
  // g_object_unref(socket_address);
  // printf("unref addr\n");
  // g_object_unref(address);
  // printf("start service\n");
  // g_socket_service_start(service);

  // g_signal_connect(service, "incoming", G_CALLBACK(new_connection), NULL);
    //g_idle_add((GSourceFunc) (void (*)(void)) (test), NULL);
	//g_timeout_add (5000, (GSourceFunc) (void (*)(void)) (test), NULL);
	g_print("end of main\n");
	
	g_main_loop_run(loop);
	/*
	read_fds = active_fds;
		
	select_res = select( FD_SETSIZE , &read_fds , NULL , NULL , NULL);
	
	while(1)
	{
		read_fds = active_fds;
		
		select_res = select( FD_SETSIZE , &read_fds , NULL , NULL , NULL);
	
	
		close(client_fd);
	}
	*/
	gdk_threads_enter();
	//gtk_main();
	gdk_threads_leave();
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
    close(client_fd);
	printf("delete event occurred\n");
	kill(pid, SIGKILL);
	//gtk_main_quit();
	g_main_loop_quit (data);
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
void OpenFriendChat(GtkButton *button, void *user_data)
{
	lastFriendClicked = button;
	printf("%s\n", gtk_button_get_label (lastFriendClicked));
	lastUserClicked = gtk_button_get_label (button);
	printf("Friend Chat Button Pressed\n");
	
	/* changes the formatting of the string using Pango Attributes */
	char *stringTemp = g_strdup_printf ("<span font=\"22\" color=\"black\">" "<b>%s</b>" "</span>", lastUserClicked);
	gtk_label_set_markup (GTK_LABEL (friendName), stringTemp);
	gtk_label_set_justify(GTK_LABEL(friendName), GTK_JUSTIFY_CENTER);
    gtk_label_set_line_wrap(GTK_LABEL(friendName), TRUE);
	g_free (stringTemp);
	
	gtk_widget_hide(homeWindow);
	gtk_widget_show_all(FriendChatWindow);
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
void LogOutBtnFunct(GtkButton *button, void *user_data) 
{
	LogOut();
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
	/* Clear send chat view*/
	gtk_text_buffer_get_iter_at_offset (SendBuffer, &start, 0);
	gtk_text_buffer_get_iter_at_offset (SendBuffer, &end, 1000);
	gtk_text_buffer_delete(SendBuffer,&start,&end);
	GtkTextIter rstart;
	GtkTextIter rend;
	gtk_text_buffer_get_iter_at_offset(ReceiveBuffer,&rstart,0); 
	gtk_text_buffer_get_iter_at_offset(ReceiveBuffer,&rend,1000); 
	gtk_text_buffer_delete(ReceiveBuffer,&rstart,&rend); 
	
	gtk_widget_hide(FriendChatWindow);
	gtk_widget_hide(FriendRequestWindow);
	gtk_widget_hide(AddFriendWindow); 
	gtk_widget_show_all(LogOutButton);
	gtk_widget_show_all(homeWindow); 
	
	
}

static void PressEnter_Login( GtkWidget *widget, GtkWidget *entry )
/* whenever you press enter on login text entry box */
{
  printf("PressEnter_Login Pressed\n");
  const gchar *entry_text1, *entry_text2;
  memset(username, '\0', STR_LEN);
  memset(p, '\0', STR_LEN);
  entry_text1 = gtk_entry_get_text (GTK_ENTRY (usernameTextEntry));
  entry_text2 = gtk_entry_get_text (GTK_ENTRY (password));
  
  char *user = (char*)entry_text1;
  char *pass = (char*)entry_text2;
  
  size_t userLength = strlen(entry_text1);
  size_t passLength = strlen(entry_text2);
  
  //strcpy(user, username);
  //strcpy(pass, p);
  strncpy(username, user, sizeof username - 1);
      username[userLength] = '\0';
	strncpy(p, pass, sizeof username - 1);
      p[passLength] = '\0';
  
  //printf("%s\n",username);
  //printf("%s\n",p);
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
	  printf("sending to server \n");
	  write(client_fd, "$login", strlen("$login")); /*login */ memset(buffer,'\0',MSG_LEN);
	  read(client_fd, buffer, MSG_LEN); 
	  
	  #ifdef DEBUG				
		printf("should say Login: %s \n",buffer);
	  #endif
	  
	  write(client_fd, username, strlen(username)); /* username*/ memset(buffer,'\0',MSG_LEN);
	  printf("sent msg\n");
	  read(client_fd, buffer, MSG_LEN);
	  write(client_fd, p, strlen(p)); /*password*/ 
					
	  memset(buffer,'\0',MSG_LEN);
	  #ifdef DEBUG
					printf("time to read the server's response \n");
	#endif
					read(client_fd, buffer, MSG_LEN); /*read $login */
	#ifdef DEBUG				
					printf("should say Login: %s \n",buffer);
	#endif
					printf("writing back to login\n");
					write(client_fd, buffer, strlen(buffer)); /*write back to it $login */
					printf("read success or fail\n");
					read(client_fd, success_fail, MSG_LEN);   /*read if it was successful or failed */
					printf("writes back something useless\n");
					write(client_fd, buffer, strlen(buffer)); /*write back something useless */
					printf("read reason\n");
					read(client_fd, reason, MSG_LEN);         /*read the reason */
	#ifdef DEBUG			
					printf("%s \n", success_fail);
					printf("%s \n", reason);
	#endif			
					
					if(strcmp(success_fail, "success") ==0) {
						Login = 1;
						setUserHomeText(user);
						/****/
						if(Login == 1)
						{
							HandlePulling();
							//while(!Exit) {
								if((pid = fork() == 0)){
									if(Login == 1){
										//gtk_widget_show_all(homeWindow);
										read_fds = active_fds;
										while(!Exit)
										{
											if(addFriendFlagSucc == 1)
											{
												GtkWidget *buttonFriendNew = gtk_button_new_with_label(usernameH);							//create a new btn
												gtk_widget_set_size_request(buttonFriendNew, 130, 20);									//resizes
												gtk_table_attach_defaults (GTK_TABLE (table), buttonFriendNew, 0, 1, addX, addX+1); 	//adds the btn on the table
												addX++;																				//increments test counter
												g_signal_connect(buttonFriendNew, "clicked", G_CALLBACK(OpenFriendChat), table);		//connect delete friend signal
												gtk_widget_show(buttonFriendNew);
												addFriendFlagSucc = 0;
											}
											if(chatFlagSucc == 1)
											{
												receive_chat(message);
												chatFlagSucc = 0;
											}
											select_res = select( FD_SETSIZE , &read_fds , NULL , NULL , NULL);
										
											if ((select_res < 0)){ perror("select error"); exit(10);
											} else if(select_res ==0 ) { 
												kill(pid, SIGKILL);
												perror("we timed out bois"); 
												close(client_fd);
												exit(10);
											}
										
											if(FD_ISSET(client_fd,&read_fds)) { /***** Got a message from the server **********/

												memset(buffer,'\0',MSG_LEN);
												w_or_r = recv(client_fd, buffer, sizeof(buffer), 0); 
												if(w_or_r ==0 ) {
													//break;
													
												}
												HandleServer(client_fd, buffer, sizeof(buffer));
											}
											sleep(1);
										}
									}
								}
								else
								{
									printf("test\n");
								}
							//}
						}
						/****/
						//stupidReset();
	#ifdef DEBUG
					printf("login passed!\n");
	#endif
					} else {
						printf("%s \nPlease try again\n", reason);
					}
		memset(buffer,'\0',MSG_LEN);
		memset(buffer2,'\0',MSG_LEN);
		memset(buffer3,'\0',MSG_LEN);
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

/*resetting msg entry box */
void resetMsgEntry()
{
	/* reset username text entry box */
    gtk_entry_set_text(GTK_ENTRY(msg), "");
	
}

/* chatting */
static void msg_enter(GtkWidget *widget, GtkWidget *entry)
{
	chat_enter();
	const gchar *entry1; 
	entry1 = gtk_entry_get_text(GTK_ENTRY(msg));
	memset(usernameMSG, '\0', STR_LEN);
	memset(messageSent, '\0', STR_LEN);
	if(strcmp(entry1,emptyString) == 0)
	{
		g_print("You did not enter a message\n"); 
	}
	else
	{
		memset(buffer,'\0',MSG_LEN);
		memset(buffer2,'\0',MSG_LEN);
		memset(buffer3,'\0',MSG_LEN);
		
		char *temp = "$chat";
		strncpy(buffer, temp, sizeof buffer -1);
		buffer[5] = '\0';
		
		char *msgSent = (char*)entry1;
		char *user = (char*)lastUserClicked;
		
		size_t msgLength = strlen(entry1);
		size_t userLength = strlen(lastUserClicked);
		
		strncpy(buffer2, user, sizeof buffer2 - 1);
		buffer2[userLength] = '\0';
		strncpy(buffer3, msgSent, sizeof buffer3 - 1);
		buffer3[msgLength] = '\0';
		//strncpy(usernameMSG, user, sizeof usernameMSG - 1);
		//usernameMSG[userLength] = '\0';
		//strncpy(messageSent, msgSent, sizeof messageSent - 1);
		//messageSent[msgLength] = '\0';
	  
		//g_print("You've said: %s\n",entry1);
		flag = 1;
		Chat();
		//stupidReset();
	}
	//g_timeout_add (5000, (GSourceFunc) (void (*)(void)) (test), NULL);
	resetMsgEntry();
}

/* resetting text entry box */
void resetTextEntry()
{
	/* reset username text entry box */
    gtk_entry_set_text (GTK_ENTRY (usernameTextEntry), "");
	
	/* reset password text entry box */
	gtk_entry_set_text (GTK_ENTRY (password), "");
	
	/* reset ALL registration text entry box */
	gtk_entry_set_text (GTK_ENTRY (CreateUsername) , "");
	gtk_entry_set_text (GTK_ENTRY (CreatePassword) , "");
	gtk_entry_set_text (GTK_ENTRY (CreatePasswordConfirmation) , "");
	
	/* reset addFriend text entry box */
	gtk_entry_set_text (GTK_ENTRY (AddUsername), "");
	
	/* reset changePassEText text entry box */
	gtk_entry_set_text (GTK_ENTRY (changePassEText), "");
}

/* whenever you press enter on register text entry box */
static void PressEnter_Register( GtkWidget *widget, gpointer user_data)
{
  printf("PressEnter_Register Pressed\n");
  const gchar *entry_text1, *entry_text2, *entry_text3;
  memset(regisUser,'\0',STR_LEN);
  memset(regisPass,'\0',STR_LEN);
  entry_text1 = gtk_entry_get_text (GTK_ENTRY (CreateUsername));
  entry_text2 = gtk_entry_get_text (GTK_ENTRY (CreatePassword));
  entry_text3 = gtk_entry_get_text (GTK_ENTRY (CreatePasswordConfirmation));
  
  char *user = (char*)entry_text1;
  char *pass = (char*)entry_text2;
		
		size_t userLength = strlen(entry_text1);
		size_t passLength = strlen(entry_text2);
		
		
		strncpy(regisUser, user, sizeof regisUser - 1);
		regisUser[userLength] = '\0';
		strncpy(regisPass, pass, sizeof regisPass - 1);
		regisPass[passLength] = '\0';
  
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
	  			write(client_fd, "$register", strlen("$register")); /*login */ memset(buffer,'\0',MSG_LEN);
				read(client_fd, buffer, MSG_LEN); 
	#ifdef DEBUG				
				printf("should say Register: %s \n",buffer);
	#endif
				write(client_fd, regisUser, strlen(regisUser)); /* username*/ memset(buffer,'\0',MSG_LEN);
				read(client_fd, buffer, MSG_LEN);
				write(client_fd, regisPass, strlen(regisPass)); /*password*/ 
				
				memset(buffer,'\0',MSG_LEN);
				
				
				/**** TIME TO IMPLEMENT IT  ****************/
#ifdef DEBUG
				printf("time to read the server's response \n");
#endif

				read(client_fd, buffer, MSG_LEN); /*read $login */
				
#ifdef DEBUG				
				printf("should say Register: %s \n",buffer);
#endif
				write(client_fd, buffer, strlen(buffer)); /*write back to it $register */
				read(client_fd, success_fail, MSG_LEN);   /*read if it was successful or failed */
				write(client_fd, buffer, strlen(buffer)); /*write back something useless */
				read(client_fd, reason, MSG_LEN);         /*read the reason */
#ifdef DEBUG			
				printf("%s \n", success_fail);
				printf("%s \n", reason);
#endif			
				
				if(strcmp(success_fail, "success") ==0) {
#ifdef DEBUG
					printf("register successful!\n");
					printf("please log in now\n");
					resetTextEntry();
					gtk_widget_hide(registerWindow);
					gtk_widget_show_all(loginRegisWindow);
#endif
				} else {
					printf("%s \nPlease try again\n", reason);
				}
  }
  memset(buffer,'\0',MSG_LEN);
  memset(buffer2,'\0',MSG_LEN);
  memset(buffer3,'\0',MSG_LEN);
  memset(regisUser,'\0',STR_LEN);
  memset(regisPass,'\0',STR_LEN);
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
// void addFriend(const gchar *text)
// {
	// /*
	// char *tmp;																			//testing purposes to print out numbers	
	// tmp = (char*)calloc(255, sizeof(char));												//allocs mem for pointer
	// sprintf(tmp, "%d", testInt);														//writes int as a *char
	// */
	// printf("adding %s\n", tmp);
	// const gchar *tmp = (char*)calloc(255, sizeof(char));
	// tmp = "asdf";
	// g_print("adding %s\n", tmp);
	// GtkWidget *buttonFriendNew = gtk_button_new_with_label("11");						//create a new btn
	// gtk_widget_set_size_request(buttonFriendNew, 130, 20);								//resizes
	// gtk_table_attach_defaults (GTK_TABLE (table), buttonFriendNew, 0, 1, addX, addX+1); //adds the btn on the table
	// addX++;																				//increments the counter
	// testInt++;																			//increments test counter
	// free(tmp);																		//free pointer
	// g_signal_connect(buttonFriendNew, "clicked", G_CALLBACK(removeFriend), table);		//connect delete friend signal
	// gtk_widget_show(buttonFriendNew);
	
// }

/* removes friend from contact list */
void removeFriend(GtkWidget *button, GtkButton *user_data)
{
	//printf("%s\n", gtk_button_get_label (lastFriendClicked));
	char *strtemp = "$deletefriend";
	strncpy(buffer, strtemp, sizeof buffer -1);
	buffer[13] = '\0';
	tmpFriend  = gtk_button_get_label (lastFriendClicked);
	char *fr = (char*)tmpFriend;
	size_t friendLength = strlen(tmpFriend);
	strncpy(buffer2, fr, sizeof buffer2 - 1);
	buffer2[friendLength] = '\0';
	
	int succ_fail = DeleteFriend();
	if(succ_fail == 0)
	{
		gtk_widget_destroy(GTK_WIDGET(lastFriendClicked));
		
		gtk_widget_show_all(homeWindow);
		gtk_widget_hide(FriendChatWindow);
	}
	else{
		printf("FAILED TO DELETE FRIEND\n");
	}
}

/* test print */
void printTest(GtkWidget *widget, gpointer data)
{
	printf("test %d\n", testInt);
	testInt++;
}

// /* log out of homeWindow to loginInputWindow */
// void logOutFunction(GtkWidget *button, GtkWidget *widget)
// {
	// resetTextEntry();
	// gtk_widget_hide(homeWindow);
	// gtk_widget_hide(AddFriendWindow);
	// gtk_widget_hide(FriendRequestWindow);
	// gtk_widget_hide(FriendRequestDialog);
	// gtk_widget_show_all(loginInputWindow);
// }

void ExitFriendReqDialog(GtkWidget *widget, gpointer *user_data)
{
	gtk_widget_hide(FriendRequestDialog);
}

void FriendReq(GtkButton *button, void *user_data)
{
	int alreadyHave = 0;
	// char *msgSent = (char*)entry1;
		// char *user = (char*)lastUserClicked;
		
		// size_t msgLength = strlen(entry1);
		// size_t userLength = strlen(lastUserClicked);
		
		// strncpy(buffer2, user, sizeof buffer2 - 1);
		// buffer2[userLength] = '\0';
		// strncpy(buffer3, msgSent, sizeof buffer3 - 1);
		// buffer3[msgLength] = '\0';
	tmpFriend  = gtk_button_get_label (button);
	char *fr = (char*)tmpFriend;
	size_t friendLength = strlen(tmpFriend);
	strncpy(friend, fr, sizeof friend - 1);
	friend[friendLength] = '\0';
  
  int valid = VerifyUserName(friend);
	if(valid == 0) {
		printf("person doesn't exist!\n");
	}
	else{
  
	  GtkWidget *dialog1, *dialog2;
	  dialog1 = gtk_message_dialog_new(GTK_WINDOW(FriendRequestWindow),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_QUESTION,
				GTK_BUTTONS_YES_NO,
				"Would you like to accept this friend request?");
	  gtk_window_set_title(GTK_WINDOW(dialog1), "Friend Request");
	  gtk_widget_show_all(dialog1);
	  gint response = gtk_dialog_run(GTK_DIALOG(dialog1)); //stores the enum value for the response
	  
	  //if yes, another dialog box will be open
	  //if no, returns back to the screen
	  
	  if(response == -8)
	  {
			dialog2 = gtk_message_dialog_new(GTK_WINDOW(FriendRequestWindow),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_INFO,
				GTK_BUTTONS_OK,
				"Adding to your friend list...\n");
			gtk_window_set_title(GTK_WINDOW(dialog2), "Added Friend");
			gtk_dialog_run(GTK_DIALOG(dialog2));
			gtk_widget_destroy(dialog2);
			
		#ifdef DEBUG
			printf("This is the username passed in AppendFriend : %s \n", username);
			printf("This is the friend passed in AppendFriend : %s \n", friend);
		#endif

			FILE *fp, *fp2;

			/*changes file path*/
			char currentpath[STR_LEN] = "./Users/";
			char filetype[STR_LEN] = ".txt";
			char line[STR_LEN] = {0};
			strcat(currentpath,username);
			strcat(currentpath,filetype);
			
			/*./Users/username.txt is opened */
			fp2 = fopen(currentpath,"a+");
			while (fgets(line, sizeof(line), fp2)) {
				/* note that fgets don't strip the terminating \n, checking its
				   presence would allow to handle lines longer that sizeof(line) */
		#ifdef DEBUG
				printf("%s", line);
		#endif	
				if( line[0] !='\n') {
					line[strcspn(line, "\r\n")] = 0; /* if last char  == '\0' then replaces again with null; this is not met if first char is new line because then it would be an empty string */
				} 
				if( strcmp(line,".friendlist") == 0 ){
					memset(line,'\0',STR_LEN);
		#ifdef DEBUG
					printf("I found friendlist! \n");
		#endif
					break;
				}
				memset(line,'\0',STR_LEN);
			}
			
			/*now checking after friendslist */
		#ifdef DEBUG
				printf("Checking Username List for that friend \n");
		#endif	


			while (fgets(line, sizeof(line), fp2)) {
		#ifdef DEBUG
				printf("%s", line);
		#endif	
				if( line[0] !='\n') {
					line[strcspn(line, "\r\n")] = 0; /* if last char  == '\0' then replaces again with null; this is not met if first char is new line because then it would be an empty string */
				} 
				if(strcmp(line,friend) == 0 ) {
					printf("friend already added \n");
					fclose(fp2);
					alreadyHave = 1;
				}
				memset(line,'\0',STR_LEN);
			}
			if(alreadyHave == 0)
			{
				/***** Does not have that friend ****/
				
				/*username addds the friend ***/
				fprintf(fp2,"%s\n",friend);
				
				char currentpath2[STR_LEN] = "./Users/";
				char filetype2[STR_LEN] = ".txt";
				strcat(currentpath2,friend);
				strcat(currentpath2,filetype2);
				fp = fopen(currentpath2,"a+");
				
				/*friend adds the username */
				
				fprintf(fp,"%s\n",username);
				fclose(fp2);
				fclose(fp);
				gtk_widget_destroy(GTK_WIDGET(button));
				
				GtkWidget *buttonFriendNew = gtk_button_new_with_label(tmpFriend);						//create a new btn
				gtk_widget_set_size_request(buttonFriendNew, 130, 20);									//resizes
				gtk_table_attach_defaults (GTK_TABLE (table), buttonFriendNew, 0, 1, addX, addX+1); 	//adds the btn on the table
				addX++;																					//increments the counter
				testInt++;																				//increments test counter
				g_signal_connect(buttonFriendNew, "clicked", G_CALLBACK(OpenFriendChat), table);		//connect delete friend signal
				gtk_widget_show(buttonFriendNew);
			}
		  }
	  
	  
	  gtk_widget_destroy(dialog1);
  }
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


void stall(GtkWidget *widget, gpointer data)
{
	fd_set active_fds, read_fds;
	FD_ZERO(&active_fds);
	FD_SET(client_fd, &active_fds);
	FD_SET(STDIN_FILENO, &active_fds);
	int select_res;
	int w_or_r;
	
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	memset(buffer3,'\0',MSG_LEN);
	
	read_fds = active_fds;
		
		select_res = select( FD_SETSIZE , &read_fds , NULL , NULL , NULL);
	
		if ((select_res < 0)){ perror("select error"); exit(10);
        } else if(select_res ==0 ) {   
			perror("we timed out bois"); 
			exit(10);
		}
	
		if(FD_ISSET(client_fd,&read_fds)) { /***** Got a message from the server **********/

			memset(buffer,'\0',MSG_LEN);
			w_or_r = recv(client_fd, buffer, sizeof(buffer), 0);
			HandleServer(client_fd, buffer, sizeof(buffer));
		}
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	memset(buffer3,'\0',MSG_LEN);
	
	char random[STR_LEN] = {0};
	/*echo */
	write(client_fd, "hello", strlen("hello"));

		
		
		
	/*reading message */
		
	read(client_fd, random, MSG_LEN);
		
	if(random[0] != '\n') {
		random[strcspn(random, "\r\n")] = 0;
	}
		
	printf("The string recieved was %s \n",random);
}

void HandleServer(int client_fd, char* buffer, int buffer_size) {
	char welcomeH[MSG_LEN] = {0};
	memset(message, '\0', MSG_LEN);
	memset(usernameH, '\0', STR_LEN);
	
	if(strcmp(buffer,"$addfriend") == 0) {
#ifdef DEBUG
		printf(" I GOT THIS! \n");
#endif
		write(client_fd, buffer, strlen(buffer)); /*write back to it $addfriend */
		read(client_fd, usernameH, MSG_LEN);   /*read the usernameH */
		write(client_fd, buffer, strlen(buffer)); /*write back something useless */
		read(client_fd, welcomeH, MSG_LEN);         /*read the welcomeH */
		
#ifdef DEBUG
		printf("%s added you to their friend list :D\n", usernameH);
		printf("%s \n", welcomeH);
#endif	

		addFriendFlagSucc = 1;
		
	} else if (strcmp(buffer,"$chat") == 0){
		memset(message, '\0', MSG_LEN);
		write(client_fd, buffer, strlen(buffer)); /*write back to it $chat */
		read(client_fd, usernameH, MSG_LEN);   /*read the usernameH */
		write(client_fd, buffer, strlen(buffer)); /*write back something useless */
		read(client_fd, message, MSG_LEN);         /*read the message */

		printf("%s%s: %s %s\n", "\x1B[34m", usernameH, message, "\x1B[0m");
		chatFlagSucc = 1;
	} else if (strcmp(buffer,"$online") == 0){
		write(client_fd, buffer, strlen(buffer)); /*write back to it $online */
		read(client_fd, usernameH, MSG_LEN);   /*read the usernameH */
#ifdef DEBUG
		printf("%s is now online \n", usernameH);
#endif			
	} else if (strcmp(buffer,"$offline") == 0){
		write(client_fd, buffer, strlen(buffer)); /*write back to it $offline */
		read(client_fd, usernameH, MSG_LEN);   /*read the usernameH */
#ifdef DEBUG
		printf("%s is now offline \n", usernameH);
#endif			
	} else if (strcmp(buffer,"$refresh") == 0){
		printf("how refreshing\n");
	}
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	memset(buffer3,'\0',MSG_LEN);
}

void receive_chat(char msgrec[MSG_LEN])
{
	//char *tmpchat = (char *)malloc(strlen(msgrec)+1);
	const gchar *receive;
	//tmpchat = strcpy(tmpchat, msgrec);
	//receive = (const char*)tmpchat;
	receive = "testing";
	
	mark = gtk_text_buffer_get_insert(ReceiveBuffer); 
	gtk_text_buffer_get_iter_at_mark(ReceiveBuffer,&iter,mark); 
	if(gtk_text_buffer_get_char_count(ReceiveBuffer))
	{
		gtk_text_buffer_insert(ReceiveBuffer,&iter,"\n",1);
	}
	gtk_text_buffer_get_end_iter(SendBuffer,&end); 
	gtk_text_buffer_insert(ReceiveBuffer,&iter,msgrec,-1); 
	gtk_text_buffer_insert(SendBuffer,&end,"Friend:\n",-1);
	//free(tmpchat);
}


void Chat() {
		/*grabs all three buffers before can write */
	char success_fail[64] = {0};
	char reason[64] = {0};
	
	/*starts writing to server */
	write(client_fd, buffer, strlen(buffer)); /*write $chat */
	read(client_fd, buffer, MSG_LEN);		/*read chat */
	write(client_fd, buffer2, strlen(buffer2)); /*write username*/
	read(client_fd, buffer, MSG_LEN);      /*read username */
	write(client_fd, buffer3, strlen(buffer3)); /*write message */
#ifdef DEBUG
	printf("buffer3 is: %s\n", buffer3);
#endif
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	memset(buffer3,'\0',MSG_LEN);
	
	/*** ACCEPTING SERVER RESPONSE ****/
	
	read(client_fd, buffer, MSG_LEN); 		   /*read $chat */
	write(client_fd, buffer, strlen(buffer)); /*write back to it $chat */
	read(client_fd, success_fail, MSG_LEN);   /*read if it was successful or failed */
	write(client_fd, buffer, strlen(buffer)); /*write back something useless */
	read(client_fd, reason, MSG_LEN);         /*read the reason */	
	
#ifdef DEBUG
	printf("The chat was a %s \n", success_fail);
	printf("The reason is: %s\n", reason);
#endif 
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	memset(buffer3,'\0',MSG_LEN);
	//stupidReset();
	
}

int AddFriend() {
	char success_fail[64] = {0};
	char reason[64] = {0};
	
	/* getting who the person wants to friend */
	
#ifdef DEBUG
	printf("trying to add user: %s \n", buffer2);
#endif			
	write(client_fd, buffer, strlen(buffer)); /*write $addfriend */
	read(client_fd, buffer, MSG_LEN);		/*read addfriend */
	write(client_fd, buffer2, strlen(buffer2)); /*write username that you want to add*/
	
	/**** server response ***/
	memset(buffer,'\0',MSG_LEN);
	read(client_fd, buffer, MSG_LEN); 		   /*read $addfriend */
	write(client_fd, buffer, strlen(buffer)); /*write back to it $addfriend */
	read(client_fd, success_fail, MSG_LEN);   /*read if it was successful or failed */
	write(client_fd, buffer, strlen(buffer)); /*write back something useless */
	read(client_fd, reason, MSG_LEN);         /*read the reason */	
	
#ifdef DEBUG
	printf("The reason is: %s\n", reason);
#endif 
	if(strcmp(success_fail, "failed") == 0)
	{
		return -1;
	}
	
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	//stupidReset();
	return 1;
}

void LogOut() {
	char message[MSG_LEN] = {0};
	write(client_fd, "$logout", strlen("$logout")); /*write to it $logout */
	read(client_fd, message, MSG_LEN);   /*read the response */
#ifdef DEBUG
	printf("you are logged out \n");
	printf("message is : %s",message);
#endif
	Login = 0;
}

int DeleteFriend(){
	char success_fail[64] = {0};
	char reason[64] = {0};

	/*starts writing to server */
	write(client_fd, buffer, strlen(buffer)); /*write $deletefriend */
	read(client_fd, buffer, MSG_LEN);		/*read deletefriend */
	write(client_fd, buffer2, strlen(buffer2)); /*write friend*/

#ifdef DEBUG
	printf("buffer3 is: %s", buffer3);
#endif
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	memset(buffer3,'\0',MSG_LEN);
	
	/*** ACCEPTING SERVER RESPONSE ****/
	
	 read(client_fd, buffer, MSG_LEN); 		   /*read $deletefriend */
	 write(client_fd, buffer, strlen(buffer)); /*write back to it $chat */
	 read(client_fd, success_fail, MSG_LEN);   /*read if it was successful or failed */
	 write(client_fd, buffer, strlen(buffer)); /*write back something useless */
	 read(client_fd, reason, MSG_LEN);         /*read the reason */	
	
#ifdef DEBUG
	printf("The deletefriend: %s \n", success_fail);
	printf("The reason is: %s", reason);
#endif 
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	memset(buffer3,'\0',MSG_LEN);
	
	if(strcmp(success_fail, "failed") == 0)
	{
		return -1;
	}
	return 0;

}

void ChangePassword() {
	char success_fail[64] = {0};
	char reason[64] = {0};
	
	fgets(buffer2, MSG_LEN-1, stdin);
	if(buffer2[0] != '\n') {
		buffer2[strcspn(buffer2, "\r\n")] = 0;
	}
	
	fgets(buffer3, MSG_LEN-1, stdin);
	if(buffer3[0] != '\n') {
		buffer3[strcspn(buffer3, "\r\n")] = 0;
	}
	/*starts writing to server */
	write(client_fd, buffer, strlen(buffer)); /*write $changepassword */
	read(client_fd, buffer, MSG_LEN);		/*read changepassword */
	write(client_fd, buffer2, strlen(buffer2)); /*write password*/
	read(client_fd, buffer, MSG_LEN);      /*read password */
	write(client_fd, buffer3, strlen(buffer3)); /*write new password */
#ifdef DEBUG
	printf("buffer3 is: %s\n", buffer3);
#endif
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	memset(buffer3,'\0',MSG_LEN);
	
	/*** ACCEPTING SERVER RESPONSE ****/
	
	 read(client_fd, buffer, MSG_LEN); 		   /*read $changepassword */
	 write(client_fd, buffer, strlen(buffer)); /*write back to it $changepassword */
	 read(client_fd, success_fail, MSG_LEN);   /*read if it was successful or failed */
	 write(client_fd, buffer, strlen(buffer)); /*write back something useless */
	 read(client_fd, reason, MSG_LEN);         /*read the reason */	
	
#ifdef DEBUG
	printf("The changepassword: %s \n", success_fail);
	printf("The reason is: %s\n", reason);
#endif 
	memset(buffer,'\0',MSG_LEN);
	memset(buffer2,'\0',MSG_LEN);
	memset(buffer3,'\0',MSG_LEN);	

}

void stupidReset()
{
	// struct timeval timeout;
	// timeout.tv_sec = 1;
	// read_fds = active_fds;
	// while(select_res == 0){
		// select_res = select( FD_SETSIZE , &read_fds , NULL , NULL , &timeout);
	// }
	g_print("before select_res");
	while(select_res != 0)
	{
		select_res = select( FD_SETSIZE , &read_fds , NULL , NULL , NULL);
	}
	g_print("after select_res");
	if ((select_res < 0)){ perror("select error"); exit(10);
        } else if(select_res ==0 ) {   
			perror("we timed out bois"); 
			exit(10);
		}
	
		if(FD_ISSET(client_fd,&read_fds)) { /***** Got a message from the server **********/

			memset(buffer,'\0',MSG_LEN);
			w_or_r = recv(client_fd, buffer, sizeof(buffer), 0); 
			if(w_or_r ==0 ) {
				//break;
				
			}
			HandleServer(client_fd, buffer, sizeof(buffer));
			
			
			
		} else if(flag == 1) { /******* Sending a Message to Server ************/
			/*message */

			/*reset the buffers to correctly grab user input */
			//memset(buffer,'\0',MSG_LEN);
			//memset(buffer2,'\0',MSG_LEN);
			//memset(buffer3,'\0',MSG_LEN);
			//fgets(buffer, MSG_LEN-1, stdin); /*message transfered in buffer of size MSG_LEN */
			/*note this buffer truncates one character if full length and last char  = null */ 
			/*this is done for good practice note you have undefined functionality if you print a string with no terminating null */
		
			if(buffer[0] != '\n') { 
				buffer[strcspn(buffer, "\r\n")] = 0; /*last character is always null anyway */
			}
		
			if(strcmp(buffer, "FORCE_EXIT") == 0) { 
				printf("YOU SHOULD HAVE QUIT \n");
				Exit = 1; 
			}else if(strcmp(buffer,"$chat")==0) { 
				Chat();
				flag = 0;
				
				
				
			} else if(strcmp(buffer,"$addfriend")==0) { /**** takes in argument $addfriend and then next line is the person you trying to add ***/
				AddFriend();
				flag = 0;

			
			}else {
		
		
				/*echo */
				w_or_r = write(client_fd, buffer, strlen(buffer));
				if((w_or_r) <0 ) { perror("send failed"); exit(3);}

		
		
		
				/*reading message */
		
				w_or_r = read(client_fd, buffer, MSG_LEN);
				if((w_or_r)<0) { perror("recieve "); exit(10);}
		
				if(buffer[0] != '\n') {
					buffer[strcspn(buffer, "\r\n")] = 0;
				}
		
				printf("The string recieved was %s \n",buffer);


		
			}
		
		
	
		}
}

gboolean
new_connection(GSocketService *service,
              GSocketConnection *connection,
              GObject *source_object,
              gpointer user_data)
{
	g_print("new connection\n");
  GSocketAddress *sockaddr = g_socket_connection_get_remote_address(connection, NULL);
  GInetAddress *addr = g_inet_socket_address_get_address(G_INET_SOCKET_ADDRESS(sockaddr));
  guint16 port = g_inet_socket_address_get_port(G_INET_SOCKET_ADDRESS(sockaddr));

  g_print("New Connection from %s:%d\n", g_inet_address_to_string(addr), port);

  g_object_ref (connection);
  GSocket *socket_tmp = g_socket_connection_get_socket(connection);

  gint fd = g_socket_get_fd(socket_tmp);
  GIOChannel *channel = g_io_channel_unix_new(fd);
  // Pass connection as user_data to the watch callback
  g_io_add_watch(channel, G_IO_IN, (GIOFunc) network_read, connection);
  g_print("end new connection\n");
  return TRUE;
}

gboolean
network_read(GIOChannel *source,
             GIOCondition cond,
             gpointer data)
{
	g_print("newnetworkread\n");
  GString *s = g_string_new(NULL);
  GError *error = NULL;
  GIOStatus ret = g_io_channel_read_line_string(source, s, NULL, &error);

  if (ret == G_IO_STATUS_ERROR) {
    //g_error ("Error reading: %s\n", error->message);
    g_warning ("Error reading: %s\n", error->message);
    // Drop last reference on connection
    g_object_unref (data);
    // Remove the event source
    return FALSE;
  }
  else
    g_print("Got: %s\n", s->str);

  if (ret == G_IO_STATUS_EOF) {
    return FALSE;
  }
  //stupidReset();
  g_print("end newnetworkread\n");
  return TRUE;
}

void HandlePulling()
{
	char discard[MSG_LEN] = {0};
	char online[MSG_LEN] = {0};
	char offline[MSG_LEN] = {0};
	/*send to server */
	write(client_fd, "$pull", strlen("$pull")); /*write $pull */
	
	/*reading response */
	 read(client_fd, discard, MSG_LEN);		    /*read pull */
	 write(client_fd, discard, strlen(discard)); /*write useless*/
	 read(client_fd, online, MSG_LEN);           /*read online */
	 write(client_fd, discard, strlen(discard)); /*write useless */
	 read(client_fd, offline, MSG_LEN);            /*read offline */

	if(strcmp(online,"failed") == 0 ) {
		printf("nothing should happen here because they failed to log in \n");
	} else {
		/* they are logged in and they have a friends list */
#ifdef DEBUG
		printf("%s \n", online);
		printf("%s \n", offline);
#endif
		int index_online = strcspn(online,":");
		int index_offline = strcspn(offline,":");

		char online_temp[STR_LEN] = {0};
		char offline_temp[STR_LEN] = {0};
		int i = 0; 
		for(i = 0; i < index_online+1; i++) {
			if(online[i] != ':') {
				online_temp[i] = online[i];   
			}
		}
		int numberonline = atoi(online_temp);
		for(i = 0; i < index_offline+1; i++) {
			if(offline[i] != ':') {
				offline_temp[i] = offline[i];   
			}
		}
		int numberoffline = atoi(offline_temp);
		char online_final[MSG_LEN] = {0};
		char offline_final[MSG_LEN] = {0};
		printf("numer online is %d \n", numberonline);
		printf("number offline is %d \n", numberoffline);
		strcpy(online_final, &online[0]+index_online+1);
		strcpy(offline_final, &offline[0]+index_offline+1);
		printf("online string is %s \n", online_final);
		printf("offline string is %s \n",offline_final);
		//const char s[2] = "\n";
		
		//char* token = NULL;
		//char* token2= NULL;
	// use string sep based on \n
	}
}

void changePassword(GtkWidget *widget, gpointer data)
{
	gtk_widget_hide(homeWindow);
	gtk_widget_show_all(changePasswordWindow);
}

void returnFromChangePassword(GtkWidget *widget, gpointer data)
{
	gtk_widget_hide(changePasswordWindow);
	gtk_widget_show_all(homeWindow);
}

static void password_change_enter(GtkWidget *widget, gpointer data)
{
	const gchar *entry1; 
	entry1 = gtk_entry_get_text(GTK_ENTRY(changePassEText));
	if(strcmp(entry1,emptyString) == 0)
	{
		g_print("You did not enter a username\n"); 
	}
	else
	{
		memset(changePassEntry, '\0', STR_LEN);
		memset(buffer,'\0',MSG_LEN);
		memset(buffer2,'\0',MSG_LEN);
		memset(buffer3,'\0',MSG_LEN);
		
		char *pass = (char*)entry1;
		
		size_t passLength = strlen(entry1);
		
		strncpy(buffer2, pass, sizeof buffer2 - 1);
		buffer2[passLength] = '\0';
		
		char *temp = "$changepassword";
		strncpy(buffer, temp, sizeof buffer -1);
		buffer[15] = '\0';
		//strncpy(changePassEntry, pass, sizeof changePassEntry - 1);
		//changePassEntry[passLength] = '\0';
		/*starts writing to server */
		write(client_fd, buffer, strlen(buffer)); /*write $changepassword */
		read(client_fd, buffer, MSG_LEN);		/*read changepassword */
		write(client_fd, buffer2, strlen(buffer2)); /*write password*/
		read(client_fd, buffer, MSG_LEN);      /*read password */
		write(client_fd, buffer3, strlen(buffer3)); /*write new password */
	#ifdef DEBUG
		printf("buffer3 is: %s\n", buffer3);
	#endif
		memset(buffer,'\0',MSG_LEN);
		memset(buffer2,'\0',MSG_LEN);
		memset(buffer3,'\0',MSG_LEN);
		
		/*** ACCEPTING SERVER RESPONSE ****/
		
		 read(client_fd, buffer, MSG_LEN); 		   /*read $changepassword */
		 write(client_fd, buffer, strlen(buffer)); /*write back to it $changepassword */
		 read(client_fd, success_fail, MSG_LEN);   /*read if it was successful or failed */
		 write(client_fd, buffer, strlen(buffer)); /*write back something useless */
		 read(client_fd, reason, MSG_LEN);         /*read the reason */	
		
	#ifdef DEBUG
		printf("The changepassword: %s \n", success_fail);
		printf("The reason is: %s\n", reason);
	#endif
		flag = 1;
		
		
		g_print("Password Changed\n");
		gtk_widget_hide(changePasswordWindow);
		gtk_widget_show_all(homeWindow);
		//stupidReset();
	}
	resetTextEntry();
}

void test(GtkWidget *widget, gpointer data)
{
	struct timeval timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
	read_fds = active_fds;
	g_print("before select_res\n");
	//while(select_res == 0)
	//{
		select_res = select( FD_SETSIZE , &read_fds , NULL , NULL , &timeout);
		printf("hi\n");
	//}
	g_print("after select_res\n");
	// if ((select_res < 0)){ perror("select error"); exit(10);
        // } else if(select_res ==0 ) {   
			// perror("we timed out bois"); 
			// exit(10);
		// }
	
		if(FD_ISSET(client_fd,&read_fds)) { /***** Got a message from the server **********/

			memset(buffer,'\0',MSG_LEN);
			w_or_r = recv(client_fd, buffer, sizeof(buffer), 0); 
			if(w_or_r ==0 ) {
				//break;
				
			}
			HandleServer(client_fd, buffer, sizeof(buffer));
			
			
			
		} else if(flag == 1) { /******* Sending a Message to Server ************/
			/*message */

			/*reset the buffers to correctly grab user input */
			//memset(buffer,'\0',MSG_LEN);
			//memset(buffer2,'\0',MSG_LEN);
			//memset(buffer3,'\0',MSG_LEN);
			//fgets(buffer, MSG_LEN-1, stdin); /*message transfered in buffer of size MSG_LEN */
			/*note this buffer truncates one character if full length and last char  = null */ 
			/*this is done for good practice note you have undefined functionality if you print a string with no terminating null */
		
			if(buffer[0] != '\n') { 
				buffer[strcspn(buffer, "\r\n")] = 0; /*last character is always null anyway */
			}
		
			if(strcmp(buffer, "FORCE_EXIT") == 0) { 
				printf("YOU SHOULD HAVE QUIT \n");
				Exit = 1; 
			}else if(strcmp(buffer,"$chat")==0) { 
				Chat();
				flag = 0;
				
				
				
			} else if(strcmp(buffer,"$addfriend")==0) { /**** takes in argument $addfriend and then next line is the person you trying to add ***/
				int a = AddFriend();
				a--;
				flag = 0;

			
			}else {
		
		
				/*echo */
				w_or_r = write(client_fd, buffer, strlen(buffer));
				if((w_or_r) <0 ) { perror("send failed"); exit(3);}

		
		
		
				/*reading message */
		
				w_or_r = read(client_fd, buffer, MSG_LEN);
				if((w_or_r)<0) { perror("recieve "); exit(10);}
		
				if(buffer[0] != '\n') {
					buffer[strcspn(buffer, "\r\n")] = 0;
				}
		
				printf("The string recieved was %s \n",buffer);


		
			}
		
		
	
		}
}
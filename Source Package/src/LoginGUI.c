/***********************************************************
*
*	Log-in screen
*	LoginGUI.c
*	author: Bryan Trinh
*	initial version: 02/20/19 EECS22L Winter 2019
*
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

/* Global Variables */
GtkWidget *username;
GtkWidget *password;
const char *emptyString = "";
const char *secretUsername = "bryan";
const char *secretPassword = "password123";

static void enter_callback( GtkWidget *widget,
                            GtkWidget *entry )
{
  const gchar *entry_text1, *entry_text2;
  entry_text1 = gtk_entry_get_text (GTK_ENTRY (username));
  entry_text2 = gtk_entry_get_text (GTK_ENTRY (password));
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
	  if(strcmp(entry_text1, secretUsername) == 0 && strcmp(entry_text2, secretPassword) == 0)
	  {
		  printf("You have logged in!\n");
		  gtk_main_quit();
	  }
	  else
	  {
		  printf("Incorrect password. Please try again.\n");
	  }
  }
}
/*
static void entry_toggle_editable( GtkWidget *checkbutton,
                                   GtkWidget *entry )
{
  gtk_editable_set_editable (GTK_EDITABLE (entry),
                             GTK_TOGGLE_BUTTON (checkbutton)->active);
}
*/
static void entry_toggle_visibility( GtkWidget *checkbutton,
                                     GtkWidget *entry )
{
  gtk_entry_set_visibility (GTK_ENTRY (entry),
			    GTK_TOGGLE_BUTTON (checkbutton)->active);
}

int main( int   argc,
          char *argv[] )
{

    GtkWidget *window;
    GtkWidget *vbox, *hbox;
    //GtkWidget *entry;
    GtkWidget *button;
    GtkWidget *check;

    gtk_init (&argc, &argv);

    /* create a new window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (GTK_WIDGET (window), 200, 100);
    gtk_window_set_title (GTK_WINDOW (window), "ZotChat");
    g_signal_connect (window, "destroy",
                      G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect_swapped (window, "delete-event",
                              G_CALLBACK (gtk_widget_destroy), 
                              window);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);

	/* username entry box */
    username = gtk_entry_new ();
    gtk_entry_set_max_length (GTK_ENTRY (username), 50);
    g_signal_connect (username, "activate",
		      G_CALLBACK (enter_callback),
		      username);
	//gint tmp_pos1;
	/*
    gtk_entry_set_text (GTK_ENTRY (username), " < Enter");
    tmp_pos1 = GTK_ENTRY (username)->text_length;
    gtk_editable_insert_text (GTK_EDITABLE (username), " Username >", -1, &tmp_pos1);
    gtk_editable_select_region (GTK_EDITABLE (username),
			        0, GTK_ENTRY (username)->text_length);
	*/
    gtk_box_pack_start (GTK_BOX (vbox), username, TRUE, TRUE, 0);
    gtk_widget_show (username);
	
	
	/* password entry box */
	password = gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY (password), 50);
    g_signal_connect (password, "activate",
		      G_CALLBACK (enter_callback),
		      password);
	//gint tmp_pos2;
	/*
	gtk_entry_set_text (GTK_ENTRY (password), " < Enter");
    tmp_pos2 = GTK_ENTRY (password)->text_length;
    gtk_editable_insert_text (GTK_EDITABLE (password), " Password >", -1, &tmp_pos2);
    gtk_editable_select_region (GTK_EDITABLE (password),
			        0, GTK_ENTRY (password)->text_length);
	*/
	gtk_box_pack_start (GTK_BOX (vbox), password, TRUE, TRUE, 0);
    gtk_widget_show (password);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (vbox), hbox);
    gtk_widget_show (hbox);
	
	/* editable check box */
    /*                              
    check = gtk_check_button_new_with_label ("Editable");
    gtk_box_pack_start (GTK_BOX (hbox), check, TRUE, TRUE, 0);
    g_signal_connect (check, "toggled",
	              G_CALLBACK (entry_toggle_editable), entry);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gtk_widget_show (check);
    */
	
	/* visible check box */
    check = gtk_check_button_new_with_label ("Visible");
    //gtk_box_pack_start (GTK_BOX (hbox), check, TRUE, TRUE, 0);
    g_signal_connect (check, "toggled",
	              G_CALLBACK (entry_toggle_visibility), password);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);			//makes the password entry box visible
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), FALSE);		//makes the password entry box invisible
    //gtk_widget_show (check);
	
	/* close click box */
    button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    g_signal_connect_swapped (button, "clicked",
			      G_CALLBACK (gtk_widget_destroy),
			      window);
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    gtk_widget_set_can_default (button, TRUE);
    gtk_widget_grab_default (button);
    gtk_widget_show (button);
	
    gtk_widget_show (window);

    gtk_main();

    return 0;
}



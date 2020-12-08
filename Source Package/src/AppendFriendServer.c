#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define DEBUG
#define STR_LEN 64
#define MSG_LEN 1024
#define max_users 10

int MakeServerSocket(int portno);
int VerifyUserName(char* username);
int VerifyPassword(char* username, char* password);
void PrintScanOptions(int* option, char* buffer);
void AppendUser(char* username, char* password);
int LoginUser(int client_fd, int message_size, char client_username[10][64], int max_client);
int ChatRecieve(int client_fd, int message_size, char client_username[10][64], char* message, int client_fds[10]);
int AddFriend (int client_fd, int message_size, char client_username[10][64],int client_fds[10]);
int FindClient(int client_fd, int client_fds[10]);
int CheckOnline(char sent_user[64], char client_username[10][64]);
int AlreadyFriend(char* username, char* friend);
void RegisterUser(int client_fd, int message_size, char client_username[10][64], int client_fds[10]);
int AppendFriend(char* username, char* friend);
int Exit = 0;

int main(int argc, char *argv[])
{
    int serversocket_fd, client_fd;
    int portno;
    struct sockaddr_in client;
	socklen_t client_size;
	
	
    char* program_name = argv[0];
#ifdef DEBUG
    printf("Number of Argc : %d \n", argc);
    printf("%s: Starting...\n", program_name);
    int index = 0;
    for(index= 1; index <argc; index++) {
        printf("The %d argument is : %s \n", index,argv[index]);
    }
#endif
    
    /*Autobinds port to 5050 if not defined */
    /*If defined then define to port number set */
    if(argc < 2 ) {
        printf("Autobinding port to #5050 \n");
        portno = 5050;
    } else {
        portno = atoi(argv[1]);
        if(portno <= 2000) { fprintf(stderr,"Port Number should be greater than 2000. Closing\n"); return 0;}
    }
    
	client_size = sizeof(client);
	
	/*make, listen, and bind server */
	
    serversocket_fd = MakeServerSocket(portno);
	
	
	fd_set active_fds, read_fds;
//	int max_users=10;
	int client_fds[max_users];	memset(client_fds, 0, sizeof(client_fds));
	
	char client_username[max_users][STR_LEN];
	memset(client_username, 0, sizeof(client_username)); 
	int select_res, i;

	
		/*trying to get some messages from the client */
	int w_or_r; /*integer to see if there is an invalid output or input from write or read */
	char buffer[MSG_LEN];
	memset(buffer,'\0',MSG_LEN);
	
    FD_ZERO(&active_fds);   /*empty all file descriptors resets every use*/
	FD_SET(serversocket_fd, &active_fds);   /*add server */ 
	struct timeval timeout;
	
    while(!Exit){   
	
		/*lets start reading all the connected fds*/
		read_fds = active_fds;
     
        //wait for an activity on one of the sockets , timeout is 15 seconds
		timeout.tv_sec = 120;
		timeout.tv_usec = 0;
		
		
        select_res = select( FD_SETSIZE , &read_fds , NULL , NULL , &timeout);   
       
        if ((select_res < 0)){ perror("select error"); exit(10);
        } else if(select_res ==0 ) {   
			perror("we timed out bois"); 
			close(serversocket_fd);
			exit(10);
		}
		
		
		/* new connection to server */
        if (FD_ISSET(serversocket_fd, &read_fds))   
        {   
			/*accept signals */
			client_fd = accept(serversocket_fd, (struct sockaddr*)&client, &client_size);
	
			if(client_fd < 0) { perror("did not accept client"); exit(1);} else 
				printf("server accepted new client \n");
				
			/*print the deets */
			printf("New client ip: %s:%hu.\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
			
             
            if( send(client_fd, "Hello", 5, 0) != strlen(buffer) )   
            {   
                perror("send");   
            }   
#ifdef DEBUG                
            puts("Welcome message sent successfully");   
#endif                
			/*add to the set of file descriptors checked next cycle */
			FD_SET(client_fd, &active_fds);
			
        }   
             
        /* some existing socket */
        for (i = 0; i < FD_SETSIZE; ++i) {
                        
            if (FD_ISSET( i , &read_fds))   
            {   
				if( i != serversocket_fd) {
					/*reads and puts in buffer */
					
					w_or_r = recv(i, buffer, sizeof(buffer), 0); 
					
					/*if 0 then the user disconnected*/
					if(w_or_r == 0) {
						getpeername(i , (struct sockaddr*)&client , &client_size);   
						printf("A user disconnected. The ip: %s. The port :%d \n" ,  inet_ntoa(client.sin_addr) , ntohs(client.sin_port));   
                         
						/************************SHOULD ALSO CLEAR CLIENT FDS  IN ARRAY AND NAME***********************/
						int temp;
						for(temp = 0 ; temp < max_users; temp++) {
							if(client_fds[temp] == i) {
								break;
							}
						}
						if(temp != max_users) {
							client_fds[temp] = 0;
							memset(client_username[temp], 0, strlen(client_username[temp]));
#ifdef DEBUG
							printf("\n client cleared at %d\n", temp);
							printf("client_fds[temp] = %d\n", client_fds[temp]);
							if( strcmp(client_username[temp],"") == 0 ) {printf("client_username in that position is now null \n");}
#endif
						}
						close( i );
						FD_CLR(i, &active_fds);					   
					} else {
						/*this is something the client sent to the server */
					
						

						
						printf("The string recieved was %s \n",buffer);  
						
						
						/*Parses string depending on what is recieved, if it not a command then it is just an echo to the server */
						
						/*TIME TO PARSE STRING */
						if( strcmp(buffer,"$login") ==0) {
							int fd_place_holder;
#ifdef DEBUG
							printf("Recieved $Login String! \n"); 
#endif
							send(i , buffer , strlen(buffer) , 0 );           /*send login back */
		
							fd_place_holder = LoginUser(i, sizeof(buffer) ,client_username, max_users); 
							if(fd_place_holder != -1) {
								client_fds[fd_place_holder] = i;
#ifdef DEBUG
								printf("Client File Descriptor has now also been updated in location : %d \n", fd_place_holder);
#endif	
							}

/*************************************************** LOT OF STUFF TO SUPPORT ***********************************************************/
						} else if(strcmp(buffer,"$register") == 0) { 
							printf("Recieved $register String! \n");
						
							send(i , buffer , strlen(buffer) , 0 );           /*send register back */
						
							RegisterUser(i, sizeof(buffer), client_username, client_fds);
						
						}else if(strcmp(buffer,"$chat") == 0) {
#ifdef DEBUG
							printf("Recieved $Chat String! \n"); 
#endif							
							int fd_location;
							char message[MSG_LEN];
							memset(message,'\0',MSG_LEN);
							send(i , buffer , strlen(buffer) , 0 );           /*send $chat back */
							
							fd_location = ChatRecieve(i, sizeof(buffer), client_username, message, client_fds);
							
							
							
						} else if ( strcmp(buffer,"$addfriend") == 0) {
#ifdef DEBUG
							printf("Recieved $addfriend String! \n"); 
#endif							
							send(i , buffer , strlen(buffer) , 0 );           /*send $addfriend back */
							
							AddFriend(i, sizeof(buffer), client_username, client_fds);
							
						}else {
							/*ECHOES*/
							
							/*makes last char null */
							if(buffer[0] != '\n') {
								buffer[strcspn(buffer, "\r\n")] = 0;
							}
							send(i , buffer , strlen(buffer) , 0 );   
							
						}
						/*Resets buffer to recieve a new message after every message */
						memset(buffer,'\0',MSG_LEN);
						
						
					}
				}
			
            }   
        
		}



		/*end of while loop*/
    }   	
	
	
	
	
	
	
	

	close(serversocket_fd);
	
	
	

    return 0;
}
void RegisterUser(int client_fd, int message_size, char client_username[10][64], int client_fds[10]) {
	char username[STR_LEN] = {0};
	char password[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
	int username_found = 0;
	

		recv(client_fd, username, message_size, 0);       /*get username */             //recv(client_fd, username, sizeof(buffer), 0); 
		send(client_fd , username , strlen(username) , 0 ); /*send username back */
		recv(client_fd, password, message_size, 0);       /*get password */             //recv(client_fd, password, sizeof(buffer), 0); 
		//never need this				                                                 //send(i , password , strlen(password) , 0 ); /*send password back */
#ifdef DEBUG
		printf("username recieved : %s \n", username);      /*THESE PRINT STATEMENTS STALL */
		printf("password recieved : %s \n", password);      /*THESE PRINT STATEMENTS STALL */
#endif
	
	username_found = VerifyUserName(username);
	if(username_found) {
#ifdef DEBUG
		printf("Username already exists! Cannot Register \n \n \n ");
#endif
		write(client_fd, "$register", strlen("$register"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "failed", strlen("failed"));
		read(client_fd, discard, MSG_LEN);
		write(client_fd, "Username already taken\n", strlen("Username already taken\n"));
		return;
	}
	AppendUser(username, password);
	write(client_fd, "$register", strlen("$register"));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, "success", strlen("success"));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, "Successful. Please Login\n", strlen("Successful. Please Login\n"));
	return;
}
int AddFriend (int client_fd, int message_size, char client_username[10][64],int client_fds[10]) {
	char friend[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
	int client_index;
	int recv_index;
		recv(client_fd, friend, message_size, 0);         /*get friend username */ 
		
	
#ifdef DEBUG
	printf("the friend username is: %s \n", friend);
#endif

	client_index = FindClient(client_fd, client_fds);
	
	char user[STR_LEN] = {0};
	strcpy(user,client_username[client_index]);
	
	recv_index = CheckOnline(friend,client_username);
	if(recv_index == -1){
#ifdef DEBUG
		printf(" the user who you are trying to send to is offline! \n");
#endif	
		write(client_fd, "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
		read(client_fd, discard, MSG_LEN);                                                /*recv */
		write(client_fd, "failed", strlen("failed"));                                     /*send fail */
		read(client_fd, discard, MSG_LEN);                                                /*recv*/
		write(client_fd, "That person is offline\n", strlen("That person is offline\n")); /*send reason */
		
		
		return -1;
	}
	/*********** ALSO MAKE SURE THE PERSON IS NOT ALREADY ON FRIEND LIST, else return error; If person is not already on friend list then add  ******/
	int valid_add = AppendFriend(user,friend);
	if(valid_add == -1){
#ifdef DEBUG
		printf(" the user who you are trying to send to is offline! \n");
#endif	
		write(client_fd, "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
		read(client_fd, discard, MSG_LEN);                                                /*recv */
		write(client_fd, "failed", strlen("failed"));                                     /*send fail */
		read(client_fd, discard, MSG_LEN);                                                /*recv*/
		write(client_fd, "tried adding yourself as friend or already on friend list\n", strlen("tried adding yourself as friend or already on friend list\n")); /*send reason */
		
		
		return -1;
	}
	write(client_fd, "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
	read(client_fd, discard, MSG_LEN);                                                /*recv */
	write(client_fd, "success", strlen("success"));                                     /*send  */
	read(client_fd, discard, MSG_LEN);                                                /*recv*/
	write(client_fd, "Say hi to your new friend\n", strlen("Say hi to your new friend\n")); /*send reason */
	
	/***** NOW MUST SEND MESSAGE TO OTHER PERSON LETTING THEM KNOW  *******/
	write(client_fds[recv_index], "$addfriend", strlen("$addfriend"));                             /*send $addfriend */
	read(client_fds[recv_index], discard, MSG_LEN);                                                /*recv */
	write(client_fds[recv_index], user, strlen(user));                                     /*send  */
	read(client_fds[recv_index], discard, MSG_LEN);                                                /*recv*/
	write(client_fds[recv_index], "Added you as a friend\n", strlen("Added you as a friend\n")); /*send reason */
	return 0;
}
int CheckOnline(char sent_user[64], char client_username[10][64]){
	int i = 0;
	for(i = 0; i <max_users; i++) {
		if(strcmp(sent_user,client_username[i]) == 0 ) {
			return i;
		}
	}
	return -1;
}
int FindClient(int client_fd, int client_fds[10]) {
	int i =0;
	for(i =0; i < max_users; i ++ ) { 
		if(client_fd == client_fds[i]) {
			return i;
		}
	}
	return -1;
}
int ChatRecieve(int client_fd, int message_size, char client_username[10][64], char* message, int client_fds[10]) {
	char reciever[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
	
		recv(client_fd, reciever, message_size, 0);         /*get reciever */   
		send(client_fd , reciever , strlen(reciever) , 0 ); /*send reciever back */		
		recv(client_fd, message, message_size, 0);         /*get message */

#ifdef DEBUG
		for(int x = 0; x<STR_LEN; x++ ) {
			if(reciever[x] =='\n') {
				printf(" somehow i copied a newline !");
			}
		}
		printf("The reciever sent to is: %s \n", reciever);
		printf("The message is: %s \n", message);
#endif
	/****************************************if user if offline, not on friend list, else successful  *************************/
	int online_index = CheckOnline(reciever, client_username);
	int sender_index = FindClient(client_fd, client_fds);
	char sender_name[STR_LEN] = {0};
	strcpy(sender_name, client_username[sender_index]);
	int already_friend = AlreadyFriend(sender_name, reciever);
	
#ifdef DEBUG
	printf("The sender is: %s \n", sender_name);
	printf("The index he is at is: %d \n", sender_index);
	printf("Reciever is: %d (shows the index which they online, -1 for offline) \n", online_index);
	printf("AlreadyFriend is: %d (1 for already a friend, -1 for not a friend) \n", already_friend);
#endif
	/**************** PERSON NEEDS TO BE ABLE TO CHAT WITH THEM SELF ******************************/
	if(strcmp(sender_name,reciever) == 0) {
#ifdef DEBUG
	printf("I should be able to talk to myself \n");
#endif
	} else {
		/*person is not a friend */
		if(already_friend == -1) {
			write(client_fd, "$chat", strlen("$chat"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "failed", strlen("failed"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "Not your friend. Try $addfriend\n", strlen("Not your friend. Try $addfriend\n"));
			return -1;
		}
	/*person receiving is offline */
		if(online_index == -1) {
			write(client_fd, "$chat", strlen("$chat"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "failed", strlen("failed"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "The person you are trying to access if offline\n", strlen("The person you are trying to access if offline\n"));
			return -1;
		}
	}
	/** person is online and a friend, tell the user it was a success **/
	write(client_fd, "$chat", strlen("$chat"));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, "success", strlen("success"));
	read(client_fd, discard, MSG_LEN);
	write(client_fd, "Chat sent successfully\n", strlen("Chat sent successfully\n"));
	
	/** time to write to the other person that a message was sent  **********/
	write(client_fds[online_index], "$chat", strlen("$chat"));                             /*send $chat */
	read(client_fds[online_index], discard, MSG_LEN);                                                /*recv */
	write(client_fds[online_index], sender_name, strlen(sender_name));                                     /*send  */
	read(client_fds[online_index], discard, MSG_LEN);                                                /*recv*/
	write(client_fds[online_index], message, strlen(message));                  /*send message */
	return 1;
}
/*** returns -1 if not successful returns 1 if successfully added ****/
int AppendFriend(char* username, char* friend){ 
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

	if(strcmp(friend,username)==0 ) {
#ifdef DEBUG
		printf("you cannot add yoruself");
#endif
		return -1;
	}
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
			return -1;
		}
		memset(line,'\0',STR_LEN);
	}
	
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




	return 1;
}

/* returns 1 if you are already friends */
int AlreadyFriend(char* username, char* friend) {
#ifdef DEBUG
    printf("This is the username passed in AppendFriend : %s \n", username);
	printf("This is the friend passed in AppendFriend : %s \n", friend);
#endif

	FILE *fp2;

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
			return 1;
		}
		memset(line,'\0',STR_LEN);
	}	
	
	fclose(fp2);
	return -1;
}
/*login will return the location where the person will slot in the array, if there are max users, already logged in, wrong username or password, this will return -1 */
int LoginUser(int client_fd, int message_size, char client_username[10][64], int max_client) {
	char username[STR_LEN] = {0};
	char password[STR_LEN] = {0};
	char discard[MSG_LEN] = {0};
	int username_found = 0, password_found = 0;
	

		recv(client_fd, username, message_size, 0);       /*get username */             //recv(client_fd, username, sizeof(buffer), 0); 
		send(client_fd , username , strlen(username) , 0 ); /*send username back */
		recv(client_fd, password, message_size, 0);       /*get password */             //recv(client_fd, password, sizeof(buffer), 0); 
		//never need this				                                                 //send(i , password , strlen(password) , 0 ); /*send password back */
#ifdef DEBUG
		printf("username recieved : %s \n", username);      /*THESE PRINT STATEMENTS STALL */
		printf("password recieved : %s \n", password);      /*THESE PRINT STATEMENTS STALL */
#endif
							
	/*now has the username and password */	
	
	username_found = VerifyUserName(username);
	if(username_found) {
		password_found = VerifyPassword(username, password);
	}
		if(username_found && password_found) {
			/*both username and password are found; client should be ready to login unless he is already logged in*/
#ifdef DEBUG
			printf("both username and password found \n");
#endif
			/*CHECK IF HE IS ALREADY LOGGED IN */
			int j = 0; 
			int already_logged_in = 0;
			int full = 0;
			for(j=0;j< max_client; j++) {
				if(strcmp(username,client_username[j]) == 0 ) {
					already_logged_in = 1;
#ifdef DEBUG
					printf("user already logged in at position : %d \n", j); 
					printf(" already logged in value is now :%d \n", already_logged_in);
#endif
				break;
				}
			}
			
			/*if not already logged in */
			
			if(!already_logged_in) {
				/*add the user, unless full */
				for(j=0;j< max_client; j++) {
					if(strcmp("",client_username[j]) == 0 ) {
						strcpy(client_username[j], username);
#ifdef DEBUG
					printf("user appended at position in array : %d \n", j); 
					printf("user: '%s' is now logged in \n", username);
#endif
						break; /*only executes once */
					}
					if(j == (max_client-1)) {
#ifdef DEBUG
						printf("server is full");
#endif
						full = 1;
					}
				}
				
				/*return not logged in, max users */
				if(full) {
					write(client_fd, "$login", strlen("$login"));
					read(client_fd, discard, MSG_LEN);
					write(client_fd, "failed", strlen("failed"));
					read(client_fd, discard, MSG_LEN);
					write(client_fd, "max users. please wait and log in later\n", strlen("max users. please wait and log in later\n"));
					return -1;
				}
				
				/*Client actually logs in */
				
				write(client_fd, "$login", strlen("$login"));
				read(client_fd, discard, MSG_LEN);
				write(client_fd, "success", strlen("success"));
				read(client_fd, discard, MSG_LEN);
				write(client_fd, "welcome\n", strlen("welcome\n"));
				return j;
			} else {
				/*already logged in fail */
				write(client_fd, "$login", strlen("$login"));
				read(client_fd, discard, MSG_LEN);
				write(client_fd, "failed", strlen("failed"));
				read(client_fd, discard, MSG_LEN);
				write(client_fd, "already logged in\n", strlen("already logged in\n"));
				return -1;
			}

		} else if(username_found) {
			/*has entered the wrong password */
#ifdef DEBUG
			printf("Only username found \n");
#endif
			write(client_fd, "$login", strlen("$login"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "failed", strlen("failed"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "password not found\n", strlen("password not found\n"));
			return -1;
		} else {
			/*user name not found */
#ifdef DEBUG
			printf("Username not found \n");
#endif
			write(client_fd, "$login", strlen("$login"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "failed", strlen("failed"));
			read(client_fd, discard, MSG_LEN);
			write(client_fd, "username not found\n", strlen("username not found\n"));
			return -1;
		}
	return -1;
}

int MakeServerSocket(int portno) {
    int socket_fd;
    struct sockaddr_in serveraddress;
    
    /*create socket */
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd <0) {
        perror("failed to create socket");
        exit(1);
    }
    
    /*bind socket to server */
    
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(portno); /*The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.*/
    /*The htonl() function converts the unsigned integer hostlong from host byte order to network byte order. */
    /*binds to all avaliable sources refer https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming */
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(socket_fd, (struct sockaddr*)&serveraddress, sizeof(serveraddress)) < 0 ) {
        perror("binding socket to addr failed");
        exit(2);
    }
    if( listen(socket_fd,3 ) < 0) { /*also makes this passive socket */
        perror("failed to listen; too many in backlog");
        exit(3);
    }
    return socket_fd;
}
int VerifyUserName(char* username) {
    
#ifdef DEBUG
    printf("This is the username passed in VerifyUserName : %s \n", username);
#endif

    FILE *fp;
    char ch;
    char currentname[STR_LEN] = {0};
    char currentchar[STR_LEN] = {0};
    
    fp = fopen("./Users/UserList.txt","a+");
    
    if (fp == NULL) {
        printf ("Error opening file; errno = %d\n", errno);
        return 1;
    }


    while((ch=fgetc(fp))!=EOF){
        if ((ch != '\n') && (ch!= '\r')) {
            printf("%c ",ch);
            memset(currentchar, ch, 1);   /*makes the char a string of length 1 with that char */
            strcat(currentname,currentchar); /* concats the two strings together */
        } else {
            /* ch = '\n' or '\r' */
            /*string is done */
            
#ifdef DEBUG
            printf("%s \n", currentname);
#endif

            if (strcmp(username, currentname) != 0) { /*they are not the same */
                memset(currentname, '\0', sizeof(currentname));
            } else { /*they are the same, found username */
                return 1;
            }
            
        }
    }
    
    fclose(fp);    
    return 0;
}
int VerifyPassword(char* username, char* password) {
	FILE *fp;
    char ch;
    char currentpass[STR_LEN] = {0};
    char currentchar[STR_LEN] = {0};
	
	/*changes file path*/
	char currentpath[STR_LEN] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	strcat(currentpath,username);
	strcat(currentpath,filetype);
	
	fp = fopen(currentpath,"a+");
	
#ifdef DEBUG 
	printf("the path is: %s \n", currentpath);
	printf("the username passed in password() is : %s \n", username);
	printf("the password passed in password() is : %s \n", password);
#endif
	if (fp == NULL) {
        printf ("Error opening file; errno = %d\n", errno);
        return 1;
    }
	
	while((ch=fgetc(fp))!=EOF){
        if ((ch != '\n') && (ch!= '\r')) {
            printf("%c ",ch);
            memset(currentchar, ch, 1);   /*makes the char a string of length 1 with that char */
            strcat(currentpass,currentchar); /* concats the two strings together */
        } else {
            /* ch = '\n' or '\r' */
            /*string is done */
            
#ifdef DEBUG
            printf("%s \n", currentpass);
#endif

            if (strcmp(password, currentpass) != 0) { /*they are not the same */
                memset(currentpass, '\0', sizeof(currentpass));
				break;
            } else { /*they are the same, found username */
                return 1;
            }
            
        }
    }
    

	fclose(fp);
    
    
    return 0;
}

void AppendUser(char* username, char* password) {
	FILE *fp, *fp2;

	
	fp = fopen("./Users/UserList.txt","a");
	fprintf(fp,"%s\n",username);
	
	
	/*changes file path*/
	char currentpath[STR_LEN] = "./Users/";
	char filetype[STR_LEN] = ".txt";
	strcat(currentpath,username);
	strcat(currentpath,filetype);
	
	fp2 = fopen(currentpath,"a");	
	fprintf(fp2,"%s\n",password);	
	fprintf(fp2,".friendlist\n");
	
	fclose(fp2);
	fclose(fp);
	
}
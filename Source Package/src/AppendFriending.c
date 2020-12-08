#include <stdio.h>
#include <errno.h>
#include <string.h>

#define STR_LEN 64
#define DEBUG
int AppendFriend(char* username,char* friend);
int VerifyUserName(char* username);
int VerifyPassword(char* username, char* password);
void PrintScanOptions(int* option, char* buffer);
void AppendUser(char* username, char* password);
int main (void) {
	int option;
	char buffer;
	char username[STR_LEN];
	char friend[STR_LEN];
	char password[STR_LEN];
	int found;
	
	while(1) {
		PrintScanOptions(&option, &buffer);
		switch(option) {
			case 4: 
				memset(username,'\0',STR_LEN);
				memset(friend,'\0',STR_LEN);				
				printf("What is your username?");
				fgets(username, STR_LEN-1, stdin);
				printf("What is your friend name?");
				fgets(friend, STR_LEN-1, stdin);
				if( username[0] !='\n') {
					username[strcspn(username, "\r\n")] = 0; /* if last char  == '\0' then replaces again with null; this is not met if first char is new line because then it would be an empty string */
				} 
				
				if( friend[0] !='\n') {
					friend[strcspn(friend, "\r\n")] = 0; /* if last char  == '\0' then replaces again with null; this is not met if first char is new line because then it would be an empty string */
				} 
				int valid = VerifyUserName(friend);
				if(valid == 0) {
					printf("person doesn't exist!");
					break;
				}
				/**** time for server to return stuff */
				
				valid = VerifyUserName(username);
				if(valid == 0) {
					printf("you don't exist!");
					break;
				}
				
				
				valid = AppendFriend(username,friend);
#ifdef DEBUG 
				printf("valid is %d",valid);
#endif
				
				break;
			case 3:
				/*exit case */
				return 0;
			case 2:
/***********************************************************************Handled By CLIENT************************************************************/
				/*Already Have An Account */
				
				/*takes in client username and password */
				memset(username,'\0',STR_LEN);
				memset(password,'\0',STR_LEN);
				printf("what is your user name? \n");
				scanf(" %s%c", username, &buffer);
					
#ifdef DEBUG
				printf("printing username : %s \n", username);
#endif	

				printf("what is your password? \n");
				scanf(" %s%c", password, &buffer);
#ifdef DEBUG
				printf("printing password : %s \n", password);
#endif	
			
			/*attempts to find username, breaks if not found */
					printf("Attempting to find username \n");
					
/***********************************************************************CLIENT Should Now send the SERVER INFORMATION***************************************/
			/* send string $login */
			/* send username */
			/* send password */

/***********************************************************************Handled By SERVER************************************************************/
			/*parse client information */
			/*recv() signals */
					found = VerifyUserName(username);

#ifdef DEBUG	
					if(found == 1) {
						printf("I found the user name! \n");
					}
#endif	
					
					if(found != 1) {
						printf("error no username found! \n");
						break;
					}
			
			/*attempts to find password, breaks if not found */			
					printf("Attempting to confirm password \n");
					
					found = VerifyPassword(username, password);

#ifdef DEBUG	
					if(found == 1) {
						printf("I found the password! \n");
					}
#endif	
					if(found != 1) {
						printf("Error incorrect password try again! \n");
						break;
					}
					
/***********************************************************************SERVER Should Now send the CLIENT INFORMATION***************************************/	
			/*send string $login */
			/*send string "success" or "fail" */
			/*send reason why */
			
					break;
			case 1:
			
/***********************************************************************Handled By Client ********************************************************/
				/*create new account */
				printf("What would you like your username to be? \n");
				scanf(" %s%c", username, &buffer);
				printf("what is your password? \n");
				scanf(" %s%c", password, &buffer);
				
				
/***********************************************************************CLIENT SENDS SERVER INFORMATION SERVER INFORMATION***************************************/					
				/* sends string "$register" */
				/*sends username */
				/*sends password */
				
				
/***********************************************************************Handled By SERVER ********************************************************/	
				/*rec() signals */
				
				/*breaks if username already in use */
				
				found = VerifyUserName(username);
				if(found) {printf("Username already exists! \n"); break;}
			
				/* else this is a new user, so create append him on userlist and add stuff */
				AppendUser(username, password);
				
/***********************************************************************SERVER SENDS CLIENT INFORMATION SERVER INFORMATION***************************************/					
				/*send $register */
				/*send "failed" or "success"
				send reason */
				
				
				
				break;
			default :
				printf("Error reached default case! Exiting...\n");
				return 0;
		}
		
	}
	

	return 0;
}
void PrintScanOptions(int* option, char* buffer) {
	printf("1-  I want to create an account! \n");
	printf("2-  I already have a username and password and am ready to login \n");
	printf("3-  Exit! \n");
	printf("4 - Append User \n");
	scanf(" %d%c", option, buffer);	
	
}
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
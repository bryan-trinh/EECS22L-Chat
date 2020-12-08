#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "CreateLoginAndVerify.h"

#define STR_LEN 64
//#define DEBUG
int VerifyUserName(char* username);
int VerifyPassword(char* username, char* password);
void PrintScanOptions(int* option, char* buffer);
void AppendUser(char* username, char* password);
/*
int main (void) {
	int option;
	char buffer;
	char username[STR_LEN];
	char password[STR_LEN];
	int found;
	
	while(1) {
		PrintScanOptions(&option, &buffer);
		switch(option) {
			case 3:
				//exit case
				return 0;
			case 2:
				//already have an account
				
				//takes in username and password and prints in debug mode
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
			
			//attempts to find username, breaks if not found
					printf("Attempting to find username \n");
					
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
			
			//attempts to find password, breaks if not found
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
					break;
			case 1:
				//create new account
				printf("What would you like your username to be? \n");
				scanf(" %s%c", username, &buffer);
				printf("what is your password? \n");
				scanf(" %s%c", password, &buffer);
				
				//breaks if username already in use
				found = VerifyUserName(username);
				if(found) {printf("Username already exists! \n"); break;}
			
				//else this is a new user, so create append him on userlist and add stuff
				AppendUser(username, password);
				break;
			default :
				printf("Error reached default case! Exiting...\n");
				return 0;
		}
		
	}
	

	return 0;
}
*/
void PrintScanOptions(int* option, char* buffer) {
	printf("1-  I want to create an account! \n");
	printf("2-  I already have a username and password and am ready to login \n");
	printf("3-  Exit! \n");
	scanf(" %d%c", option, buffer);	
	
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
            //printf("%c ",ch);
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
            //printf("%c ",ch);
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
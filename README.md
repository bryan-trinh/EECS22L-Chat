# ZotChat by Team05 - 4OUR

version 1.0
3/18/2019

# INSTALLATION
Learn how to install this by reading the INSTALL.txt

Special thanks to QV for assigning this project.

Authors:  
HE, JINGTIAN  
RAMOS, ROY  
TANG, JOHNATHAN  
TRINH, BRYAN  
ZHU, KEVIN  
ZOU, ZIHAO  


# NOTES FOR BUGS:
- Chat will not display on the GUI for receiving end, and will instead show up in terminal
- Additionally, adding a friend will append users to both accounts, but will not show up in the receiving end's GUI friend list

# NOTES:

- After you register an account, you will have to execute the file again because GTK automatically quits
- Both users have to be online at the same time to message
- Server times out after 120 seconds of inactivity
- When server times out, the client is still in the GUI

Format for server startup:
./Server <port_no>

Format for client startup:
./Client <host> <port_no>

Set Up:
1. Start the server.	eg: ./Server 5005
2. Start the client 1.	eg: ./Client bondi 5005
3. Start the client 2. 	eg: ./Client bondi 5005
4. If an account has not been registered yet, register one.
5. Login in with the login credentials.
6. Click on add friend button and add a new friend. Both users must be online.
6. To message with another user, you must click on their profile from your friend's list and type your message in the text entry box.
7. The other user's chat will show up on the terminal.
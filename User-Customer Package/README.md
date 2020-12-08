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


# Known Errors
This version has a buffer issue where it can not flush the memory of the buffer.
Not all buttons are working as of now, in particular: unfriend, friendRequests, and log out.
When server times out, the client is still in the GUI.
You must use the STALL button to receive messages
Messaging only works when both users are online and only 1 message at a time, while the other user has to click STALL.

# NOTES:

- After you register an account, you will have to execute the file again because GTK automatically quits
- Both users have to be online at the same time to message
- Server times out after 120 seconds of inactivity

Set Up:
1. Start the server.	eg: ./Server 5005
2. Start the client 1.	eg: ./Client bondi 5005
3. Start the client 2. 	eg: ./Client bondi 5005
4. If an account has not been registered yet, register one.
5. Login in with the login credentials.
6. To message with another user, you must click on their profile from your friend's list and type your message in the text entry box.
7. After you message on one client, the other client must click on the "STALL" button to receive the message.

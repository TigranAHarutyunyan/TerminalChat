# TerminalChat

## Objective
The ability to transfer data between two devices on the same local network.
## Team Roles and Responsibilities
- **Team Member 1 Gevorg Manukyan:** Role and responsibilities
- **Team Member 2 Tigran Harutyunyan:** Role and responsibilities
## Functionality
# Server 
1) Accept Client connections and send data.
2) Send update if connection list changed.
3) 
# Client
1) Client has config file where should be ip address and hostname of server.
- IP and hostname are given via command line.
- Config file is created in the /etc directory.
2) The client connects to the server and receives data from there about active users.
- If Clinet already  
3) After this, the user selects with whom to create a direct connection. And after selecting the user, the current user makes a request to the server and receives the IP of this user, after which a direct connection is created using a socket
4)When a request to open a socket is sent to the second user, he chooses whether to connect or not.
5)When the second user accepts the connection, a chat opens where the second user is on the left side and the current user is on the right side. In the chat, the first letter of the user’s name and the time when the message is sent are written
6)To send a file, use the following format: /file:path.
7)To disconnect a user these two commands are used
1)/disconnect - closes the chat with the user and sends a request to the server that he is already free and an activator for other connections
2)/exit - This disconnects the user permanently and sends a request to the server where the user's status becomes inactive.
### Technology Stack
- Linux (distribution Ubuntu OS )
- Computer Network
- C++ (Boost.Asio)
## Communication Plan
### Meetings
We meet 1 time in 2 days
### Tools
Slack,Skype,Gmail.



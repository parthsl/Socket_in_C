This program will multiplex among more than one client and will wait for sometime for input from client.
If there is no input from client for 7 seconds then it will timeout and close connection with client.

run this program by:
./server <portno>

run client by:
./client <server-addr> <portno>

you can run more than one client

Input:
Input any string from client within 7 seconds to get UPPERCASE of that string

OUTPUT:
Gives timeout or UPPERCASE string to respective client.

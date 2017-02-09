Run this program by first running server:
./server <portno>
and in other terminal run client:
./client <server-addr> <portno>

INPUT:
Any string

Output:
Reverse of the string

Here server will close when client closes, infact server will wait for other client to connect. So you can connect as many client as you want but all will get connection one after the other only, means server will server one client after other, that's what iterative is.

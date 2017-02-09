This program will show Date and Time of server on connect with "daytime" server.

To run this program one needs to setup daytime service in server. To do so in ubuntu install "xinet.d" by
1.)sudo apt-get install xinet.d
2.)sudo gedit /etc/xinet.d/daytime
3.)make no wherever "disable = yes" 
4.)save and exit
5.)sudo invoke-rc.d xinetd reload
DONE

To run this program, run command
./Daytime

Input:
nothing

Output:
Date and Time from Server

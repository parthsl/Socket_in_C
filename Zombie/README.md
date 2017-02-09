run this program by following command
./Zombie [flag]

where argument 1 is optional which is zombie handler flag
e.g. ./Zombie
will not handle zombie processes
whereas ./Zombie 1
will handle zombie processes

INPUT:
Nothing
or 
<CTRL>+c to signal process to stop abnormally. This will be handled by signal handler written in program.

Output:
Parent process creates child process and goes to sleep for 30 seconds
It will create 10 zombie processes which will die after 30 seconds as parent wakes up.

Analysis:
run following command in other terminal:
ps -aux | grep -i zombie

it will show all the process running with their states
Z+ state indicates zombie process
while S+ indicates sleeping process

check status by running above command after every second and see how after 30 second this process dies


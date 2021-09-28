#include <stdio.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include "Process.cpp"

int main(int argc, char **argv){
    
	int fd_2[2];
	fd_2[0] = atoi(argv[0]);

	char data[20];
	read(fd_2[0],data,20);

	//cout<<"Read from Pipe\n";
	//cout<<data<<endl;
	
    //Pipe to send text read from file to Ready.cpp
	int fd_1[2];
	char Buffer_1[200];
    pipe(fd_1);
    snprintf(Buffer_1,sizeof(Buffer_1),"%d",fd_1[0]);

    //Open file and read text in buffer
    char Buffer_Read[200] = {'\0'};
	int fd1  = open("processes.txt", O_RDONLY, 0); 
	read(fd1, &Buffer_Read, 200);
	cout<<"Read from file to buffer"<<endl<<endl;

    //Displays contents read from files
	for(int i = 0; Buffer_Read[i] != '\0'; i++){
		cout<<Buffer_Read[i];
	}
	
    //fork to go to Ready.cpp and send data
	pid_t child = fork();
    
	if(child>0){
        
        //Writing into Ready.cpp
		close(fd_1[0]);
		write(fd_1[1],Buffer_Read,sizeof(Buffer_Read));
		wait(NULL);
	
	}
	else if (child==0) {
        
        //exec into Ready.cpp
		close(fd_1[1]);
		execlp("./ready",Buffer_1,NULL);
	}
	
	return 0;
}


	


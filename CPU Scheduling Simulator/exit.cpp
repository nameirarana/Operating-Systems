#include <stdio.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include "Process.cpp"
#include <queue> 
#include <vector> 
#include <cstdlib>
//#include <bits/stdc++.h>
using namespace std;
int main(int argc, char **argv){
	
	//Pipe to read Processes from Ready.cpp
	int fd_from_running[2];
	fd_from_running[0] = atoi(argv[0]);

	int Process_Number = 0;
	read(fd_from_running[0],&Process_Number,sizeof(Process_Number));
	
	Process temp1;
	Process temp2;

	queue <Process> exit_q;
	
	while(exit_q.size() != Process_Number){
		read(fd_from_running[0],&temp1,sizeof(Process_Number));
		exit_q.push(temp1);
		cout<<"Recieved Process "<<temp1.Process_Name<<" in exit"<<endl;
		sleep(1);
	}
	return 0;
}

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
	
	int fd_write_to_ready[2];
	fd_write_to_ready[1] = atoi(argv[0]);

	int fd_read_from_running[2];
	fd_read_from_running[0] = atoi(argv[1]);
	
	int fd_write_to_running[2];
	fd_write_to_running[1] = atoi(argv[2]);
	
	Process temp1;
	Process temp2;
	
	int signal = 0;
	int burst_block = 0;
	
	int ongoing_process = 1;
	int k = 0;
	int signal_ready = 0;
	bool send_to_ready = false;
	while(signal != 2){
		ongoing_process = 1;
		write(fd_write_to_running[1],&ongoing_process,sizeof(ongoing_process));
		sleep(1);
			if(ongoing_process == 1){
				read(fd_read_from_running[0],&signal,sizeof(signal));
				sleep(1);
				if(signal == 1){
					cout<<"IN BLOCKED\n";
					ongoing_process = 0;
					k++;
					read(fd_read_from_running[0],&temp1,sizeof(temp1));
					temp1.PrintProcess();
					burst_block = rand() % 10 + 15;
					cout<<"BURST BLOCK= "<<burst_block<<endl;
					signal_ready = 0;
					//write(fd_write_to_running[1],&signal_ready,sizeof(signal_ready));
					for(int i = 0; i < burst_block - 1; i++){
						send_to_ready = true;
						/*ongoing_process = 0;
						write(fd_write_to_running[1],&ongoing_process,sizeof(ongoing_process));
						write(fd_write_to_running[1],&signal_ready,sizeof(signal_ready));*/
						sleep(1);
					}
					cout<<"Executed"<<endl;
					//ongoing_process = 0;
					//write(fd_write_to_running[1],&ongoing_process,sizeof(ongoing_process));
					//sleep(1);
				}
			}
		if(send_to_ready == true){
			signal_ready = 1;
			cout<<"BLOCK CYCLE EXECUTED"<<endl;
			write(fd_write_to_ready[1],&signal_ready,sizeof(signal_ready));
			sleep(1);
			temp2 = temp1;
			write(fd_write_to_ready[1],&temp2,sizeof(Process));
			send_to_ready = false;
			sleep(1);
			
		}
		else{
			signal_ready = 0;
			write(fd_write_to_ready[1],&signal_ready,sizeof(signal_ready));
			sleep(1);	
		}
		
	}
	cout<<"ITERATION: in blocked "<<k<<endl;
	//signal_ready = 1;
	//write(fd_write_to_ready[1],&signal_ready,sizeof(signal_ready));
	//sleep(1);
	return 0;
}

#include <stdio.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include "Process.cpp"
#include <queue> 
#include <vector> 
//#include <bits/stdc++.h>
using namespace std;
int main(int argc, char **argv){
    
    //Here data is read from New
    int fd_from_new[2];
    fd_from_new[0] = atoi(argv[0]);
    char Buffer_1[200] = {'\0'};
    read(fd_from_new[0],Buffer_1,200);
    
    //Displays data read in Buffer from New
    cout<<"Read from Pipe\n";
    cout<<Buffer_1<<endl;
       

    //Pipe to write from Ready to Running
	int fd_to_running[2];
    char Buffer_2[200];
    pipe(fd_to_running);
    snprintf(Buffer_2,200,"%d",fd_to_running[0]);
    
    //Pipe to Read from Running in Ready
    int fd_from_running[2];
    char Buffer_3[200];
    pipe(fd_from_running);
    snprintf(Buffer_3,200,"%d",fd_from_running[1]);
    
    //Pipe to Read from Block in Ready
    int fd_from_block[2];
    pipe(fd_from_block);
    char Buffer_4[200];
    snprintf(Buffer_4,200,"%d",fd_from_block[1]);
	
	
    //Array of type Processes to store the different procs
	Process X[12];

    //Loop Iterators
    int i = 0;
    int s = 0;
    int j = 0;
    
    //Some Variables
    int convert = 0;
    int Process_Quantum = 0;
    int Algo = -1;
    string temp = "";
    string Process_Algo;
    
    //Objects to store incoming Processes from Running in.
    Process temp_1;
    Process temp_2;

    //Used as Signals for breaking conditions of loops
    int keep = 0;
    int empty = 0;
	int Process_Number = 0;
	int Process_Executed = 0;
    
    //Vector is used to store all the Processes in and then sort them according to the Algorithm requirement
    vector <Process> ready;
    
    //Queue is used to send Processes to Running and recieve them from Running
    queue <Process> ready_q;
    
	cout<<endl<<endl<<"Writing into object"<<endl<<endl;
	
    //The first line in every file is the name of the Algorithm and is read here
	while(Buffer_1[i]!='\n'){
		temp+=Buffer_1[i];
		i++;
	}

    //The name of the Algorithm is stored in a string
	Process_Algo = temp;
	temp = "";
	i++;
    
    //According to the type of Algorithm a variable, Algo, is set and Process_Quantum is also set
	if(Process_Algo=="RR"){
		Algo = 0;
		while(Buffer_1[i]!='\n'){
		temp+=Buffer_1[i];
		i++;
		}
		Process_Quantum = stoi(temp);
		temp = "";
		i++;
	}
	if(Process_Algo=="FCFS"){
		Algo = 1;
	}
	if(Process_Algo=="SRTF"){
		Algo = 2;
	}
	if(Process_Algo=="SJF"){
		Algo = 3;
	}

	cout<<Process_Algo<<endl;
	cout<<"ALGO TYPE "<<Algo<<endl;
	cout<<"Process_Quantum= "<<Process_Quantum<<endl<<endl<<endl;

    //Data is Written into Process Array
    
	for(; Buffer_1[i] != '\0'; i++){
		if(Buffer_1[i]=='P'){
			X[j].setProcess_Name(Buffer_1[i+4]);
			while(Buffer_1[i]!='\n'){
				i++;
			}
			i++;
			while(Buffer_1[i]!='\n'){
				temp+=Buffer_1[i];
				i++;
			}
			convert = stoi(temp);
			X[j].setProcess_Arrival(convert);
			temp="";
			i++;
			while(Buffer_1[i]!='\n'){
				temp+=Buffer_1[i];
				i++;
			}
			convert = stoi(temp);
			X[j].setProcess_Burst(convert);
			temp="";
		j++;
		}
	}
	
	for(int l = 0; l < j; l++){
		//cout<<"Process Name = "<<X[l].getProcess_Name()<<endl;
		//cout<<"Arrival time = "<<X[l].getProcess_Arrival()<<endl;
		//cout<<"Burst time = "<<X[l].getProcess_Burst()<<endl<<endl;
	}
	
    //Processes from Process Array are pushed into a vector
	for(int l = 0; l < j; l++){
		ready.push_back(X[l]);
	}
	//cout<<compareProcess_Arrival(X[0],X[1]);
    
    //Here The vector is sorted according to the requirement of the Algorithm
	sort(ready.begin(), ready.end(), compareProcess_Arrival);
    
	//cout<<"From Vector after sorting"<<endl<<endl;
	for (int it = 0 ; it <j; ++it) {
        //ready[it].PrintProcess();
    } 
	
	//Data from the Vector is Pushed into the Queue
	
	for(int l = 0; l < j; l++){
		ready_q.push(ready[l]);
	}

	//FORK
	pid_t child = fork();
	if(child>0){
		
        //Ends of Pipes not being used are closed
		close(fd_from_running[1]);
		close(fd_to_running[0]);
		close(fd_from_block[1]);
        
        //Status of Queue is set
        empty = ready_q.empty();
		Process_Number = ready_q.size();
    
        //The Algorithm type and status of Queue are sent to Running
		write(fd_to_running[1],&Algo,sizeof(Algo));
		write(fd_to_running[1],&empty,sizeof(empty));
		write(fd_to_running[1],&Process_Number,sizeof(Process_Number));
		do{
            
            //The front of the Queue is sent to Running and the Queue is Popped
            //cout<<ready_q.size();
			write(fd_to_running[1],&ready_q.front(),sizeof(ready_q.front()));
			ready_q.pop();
			sleep(1);

            //Signal from Block to see if a Process has to be pushed into the Queue
			read(fd_from_block[0],&keep,sizeof(keep));
			cout<<"SIGNAL FROM RUNNING IN READY "<<keep<<endl;
			sleep(1);
            
			//read(fd_read[0],&temp_1,sizeof(Process));
			//cout<<"Read from Pipe\n";
			//temp_1.PrintProcess();
			//s++;
			//temp_2 = temp_1;
			//ready_q.push(temp_2);
			//sleep(1);

			/*read(fd_read_from_block[0],&keep,sizeof(keep));
			cout<<"SIGNAL FROM BLOCK "<<keep<<endl;
			sleep(1);*/
			if(keep == 1){
				sleep(1);
				read(fd_from_block[0],&temp_1,sizeof(Process));
			//read(fd_read[0],&keep,sizeof(keep));
			//sleep(1);
			//if(keep == 1){
				//break;
			//}
				cout<<endl<<"After reading from Block"<<endl;
			//sread(fd_read[0],&temp_1,sizeof(Process));
			//cout<<"Read from Pipe\n";
				temp_1.PrintProcess();
			//s++;
				if(temp_1.Process_Burst > 0){
					cout<<"PUSHING IN QUEUE "<<endl;
					temp_2 = temp_1;
					ready_q.push(temp_2);
					sleep(1);
				}
				sleep(1);
			}
            
            //The new status of the Queue is sent to Running
			empty = ready_q.empty();
			write(fd_to_running[1],&empty,sizeof(empty));
			sleep(1);

			read(fd_from_running[0],&Process_Executed,sizeof(Process_Executed));
			//cout<<"Number of Processes Executed "<<Process_Executed<<endl;
			sleep(1);
            
		}
		while(Process_Executed != Process_Number);
		//sleep(1);
		
		/*while(read(fd_read[0],&temppp,sizeof(Process))){
			//read(fd_read[0],&keep,sizeof(keep));
			//sleep(1);
			//if(keep == 1){
				//break;
			//}
			cout<<endl<<"After reading from Buffer"<<endl;
			//read(fd_read[0],&temp_1,sizeof(Process));
			//cout<<"Read from Pipe\n";
			//temp_1.PrintProcess();
			s++;
			if(temp_1.Process_Burst > 0){
				temp_2 = temp_1;
				ready_q.push(temp_2);
			}
			sleep(1);
		}*/
		//free(Buffer_3);
		//sleep(1);
		
		wait(NULL);
	
	}
	else if (child==0) {
		close(fd_from_block[0]);
		close(fd_to_running[1]);
		close(fd_from_running[0]);
		execlp("./running",Buffer_2,Buffer_3,Buffer_4,NULL);
	}
	return 0;
}

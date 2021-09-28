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
	int fd_from_ready[2];
	fd_from_ready[0] = atoi(argv[0]);

	//Pipe to write back Processes to Ready.cpp 
	int fd_to_ready[2];
	fd_to_ready[1] = atoi(argv[1]);

	//Pipe to send Blocked Processes to Blocked.cpp
	char Buffer_1[200];
	int fd_to_block[2];
	pipe(fd_to_block);
	snprintf(Buffer_1,200,"%d",fd_to_block[0]);
	
	//Pipe to recieve signal from Block
	char Buffer_2[200];
	int fd_from_block[2];
	pipe(fd_from_block);
	snprintf(Buffer_2,200,"%d",fd_from_block[1]);
	
	//Pipe to send signal to Exit
	char Buffer_3[200];
	int fd_to_exit[2];
	pipe(fd_to_exit);
	snprintf(Buffer_3,200,"%d",fd_to_exit[0]);
		
	//A random variable generator 
	int random = 0;
	srand(time(NULL));
	random = rand() % 2;
	//cout<<"Random = "<<random<<endl;
	
	//Two objects of type Process created to read a Process from Running and then write to Running
	Process temp1;
	Process temp2;

	int i = 0;
	int Algo = -1;
	int burst = 0;
	int total_burst = 0;
	int empty_ready_q = -1;
	int keep = 0;
	bool broken = false;
	bool to_ready_block = false;
	int empty_block = 0;
	int l = 0;
	int processes_executed = 0;
	int j = 1;
	int Process_Number = 0;
    queue <Process> block_q;
	int to_ready = 0;

	pid_t Child_Blocked = fork();
	if(Child_Blocked > 0){
			
		pid_t Child_Exit = fork();
		if(Child_Exit > 0){
			//Read the type of Algorithm to be applied and the status of the Queue in Running
			read(fd_from_ready[0],&Algo,sizeof(Algo));
			read(fd_from_ready[0],&empty_ready_q,sizeof(empty_ready_q));
			read(fd_from_ready[0],&Process_Number,sizeof(Process_Number));
			
			write(fd_to_exit[1],&Process_Number,sizeof(Process_Number));
			//sleep(1);
		
			cout<<"QUEUE EMPTY "<<empty_ready_q<<endl;
			cout<<"ALGO TYPE "<<Algo<<endl;
			cout<<"TOTAL PROCESSES "<<Process_Number<<endl;
		    
			close(fd_to_block[0]);
			if(Algo == 1){
				do{
		            
		            //Reads incomming Process from Ready and sets its burst to a variable
					read(fd_from_ready[0],&temp1,sizeof(Process));
					//cout<<"Read from Pipe\n";
					temp1.PrintProcess();
					burst = temp1.getProcess_Burst();
					cout<<endl<<"Burst "<<burst<<endl<<endl;
		            
		            //The loop executes the Burst of the Process
					for(j = 1; j <= burst; j++){
						sleep(1);
						total_burst++;
						temp1.Process_Burst --;
						
		                //If 5 sleep cycles have passed then a random variable between 1 and 0 is generated to decide if the process will be blocked or will keep executing
						if(j % 5 == 0 ){
							random = rand() % 2;
							if(random == 0){
								//cout<<"RETURN ";
								//temp1.Process_Burst -= j;
								cout<<"NEW BURST "<<temp1.Process_Burst<<endl;
		                        
		                        //If the Process has remaining Burst more than 0 then it is pushed in block Queue
								if(temp1.Process_Burst > 0){
									cout<<"BREAAK"<<endl;
									temp2 = temp1;
									block_q.push(temp2);
									l++;
									broken = true;
									sleep(1);
									//keep = 1;
									//write(fd_to_block[1],&keep,sizeof(keep));
									//sleep(1);
									//temp2 = temp1;
									//write(fd_to_block[1],&temp2,sizeof(Process));
									//sleep(1);
									//keep = 0;
									//write(fd_to_ready[1],&keep,sizeof(keep));
									//sleep(1);*/
									break;
								}
							}
							else{
								//cout<<"NOT RETURN ";
							}	
						}
		                
		                //Signal is read from Block to check if it is executing a Block Cycle or not
						/*read(fd_from_block[0],&empty_block,sizeof(empty_block));
						sleep(1);
						cout<<"EMPTY BLOCK "<<empty_block<<endl;
		                //If Block is not executing a Block Cycle
						if(empty_block == 1){
		                    
		                    //If Queue is empty a Process is sent to Block
							if(!block_q.empty()){
								keep = 1;
								write(fd_to_block[1],&keep,sizeof(keep));
								sleep(1);
								cout<<"WRITING TO BLOCK in loop"<<endl;
								//temp2 = temp1;
								//l++;
								//broken = true;
								write(fd_to_block[1],&block_q.front(),sizeof(Process));
								block_q.pop();
								sleep(1);
							}
		                    
		                    //If Queue is Empty no Process is Sent
							else{
								keep = 0;
								write(fd_to_block[1],&keep,sizeof(keep));
								sleep(1);
							}
						}

							read(fd_from_block[0],&to_ready,sizeof(to_ready));
							sleep(1);
							cout<<"SIGNAL FROM BLOCK: "<<to_ready<<endl;
							if(to_ready == 1){
								write(fd_to_ready[1],&to_ready,sizeof(to_ready));
								to_ready_block = true;
								sleep(1);
								
							}*/
		
					}
					
		            
		            //To check how many Processes have been completely executed and will send them to Exit
					//cout<<"Temp 1 .BURST"<<temp1.Process_Burst<<endl;
					if(temp1.Process_Burst == 0 ){
						processes_executed ++;
						temp2 = temp1;
						write(fd_to_exit[1],&temp2,sizeof(Process));
						sleep(1);
					}
		            
		            //here the signal is again recieved from Block incase the loop was did not finish and a process was pushed into Block Queue
					/*if(broken == true){
						read(fd_from_block[0],&empty_block,sizeof(empty_block));
						sleep(1);
						
						if(empty_block == 1){
							if(!block_q.empty()){
								keep = 1;
								write(fd_to_block[1],&keep,sizeof(keep));
								sleep(1);
								cout<<"WRITING TO BLOCK"<<endl;
								//broken = true;
								//l++;
								write(fd_to_block[1],&block_q.front(),sizeof(Process));
								block_q.pop();
								sleep(1);
							}
							else{
								keep = 0;
								write(fd_to_block[1],&keep,sizeof(keep));
								sleep(1);
							}
						}
							read(fd_from_block[0],&to_ready,sizeof(to_ready));
							sleep(1);
							cout<<"SIGNAL FROM BLOCK: "<<to_ready<<endl;
							if(to_ready == 1){
								write(fd_to_ready[1],&to_ready,sizeof(to_ready));
								to_ready_block = true;
								sleep(1);
							}
						
					}*/
						read(fd_from_block[0],&empty_block,sizeof(empty_block));
						sleep(1);

						if(empty_block == 1 && !block_q.empty()){
							keep = 1;
							write(fd_to_block[1],&keep,sizeof(keep));
							sleep(1);
							cout<<"WRITING TO BLOCK End"<<endl;
							//broken = true;
							//l++;
							write(fd_to_block[1],&block_q.front(),sizeof(Process));
							block_q.pop();
							sleep(1);
						}
						else{
							keep = 0;
							write(fd_to_block[1],&keep,sizeof(keep));
							sleep(1);
						}
							//read(fd_from_block[0],&to_ready,sizeof(to_ready));
							//sleep(1);
							//cout<<"SIGNAL FROM BLOCK: "<<to_ready<<endl;
							/*if(to_ready == 1){
								write(fd_to_ready[1],&to_ready,sizeof(to_ready));
								to_ready_block = true;
								sleep(1);
							}*/

					//keep = 0;
					//write(fd_write_to_ready[1],&keep,sizeof(keep));
					//sleep(1);
					cout<<"EMPTY BLOCK "<<empty_block<<endl;
					//cout<<"SIGNAL FROM BLOCK: "<<to_ready<<endl;
					broken = false;
					i++;
					/*if(to_ready_block == false){
						write(fd_to_ready[1],&to_ready,sizeof(to_ready));
						sleep(1);
					}*/
					to_ready_block = false;
						
		            //cout<<"SIGNAL TO READY: "<<to_ready<<endl;
		            //Checks if the Queue in Ready is Empty
					read(fd_from_ready[0],&empty_ready_q,sizeof(empty_ready_q));
					//cout<<"QUEUE EMPTY "<<empty_ready_q<<endl;
					sleep(1);

					//keep = 0;
					write(fd_to_ready[1],&processes_executed,sizeof(processes_executed));
					sleep(1);
				}
				while(processes_executed != Process_Number);
				keep = 2;
				read(fd_from_block[0],&empty_block,sizeof(empty_block));
				sleep(1);
				write(fd_to_block[1],&keep,sizeof(keep));
				sleep(1);
				write(fd_to_block[1],&total_burst,sizeof(total_burst));
				cout<<"TOTAL ITERATIONS "<<i<<endl;
				cout<<"TOTAL BURST "<<total_burst<<endl;
				cout<<"SEND TO BLOCK: "<<l<<endl;
				cout<<"QUEUE BLOCKED: "<<block_q.size()<<endl;
				cout<<"TOTAL PROCESSES EXECUTED: "<<processes_executed<<endl;
			}
				wait(NULL);
		}	
		if(Child_Exit == 0){
			close(fd_to_exit[1]);
			execlp("./exit",Buffer_3,NULL);
			cout<<"Exec failed";
    		exit(-1);
		}
	}
	if(Child_Blocked == 0){
		close(fd_to_block[1]);
		close(fd_from_block[0]);
		execlp("./block",argv[2],Buffer_1,Buffer_2,NULL);
		cout<<"Exec failed";
    	exit(-1);
	}

	return 0;
}

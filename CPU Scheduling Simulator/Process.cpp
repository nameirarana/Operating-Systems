#include <iostream>
#include <string.h>
using namespace std;
class Process {
private:

public:	
	char Process_Name;
	int Process_Arrival;
	int Process_Burst;
	int Process_Remaining;
	Process(){
		Process_Name = '\0';
		Process_Arrival = -1;
		Process_Burst = -1;
		Process_Remaining = -1;
	}
	void setProcess_Name(char n){
		Process_Name = n;
	}
	char getProcess_Name(){
		return Process_Name;
	}
	void setProcess_Arrival(int a){
		Process_Arrival = a;
	}
	int getProcess_Arrival(){
		return Process_Arrival;
	}
	void setProcess_Burst(int b){
		Process_Burst = b;
	}
	int getProcess_Burst(){
		return Process_Burst;
	}
	void PrintProcess(){ 
   	cout<<"Process Name = "<<Process_Name<<endl;
	cout<<"Arrival time = "<<Process_Arrival<<endl;
	cout<<"Burst time = "<<Process_Burst<<endl<<endl;
} 
	~Process(){ 
    	Process_Name = '\0';
		Process_Arrival = -1;
		Process_Burst = -1;
		Process_Remaining = -1;
	} 
	
};
bool compareProcess_Arrival(Process P1, Process P2){ 
   	return (P1.Process_Arrival < P2.Process_Arrival); 
} 


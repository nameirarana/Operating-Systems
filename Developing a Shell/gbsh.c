#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <csignal>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



char **envp;
void read_command( char cmd[], char *par[]){
	char line[1024];
	int count = 0;
	int i = 0;
	int j = 0;
	char* array[100];
	char* pch;
	for(;;){
		int c = fgetc(stdin);
		line[count++] = (char) c;
		if(c == '\n'){
			break;
		}
	}
	if(count == 1){
		return;
	}
	pch = strtok(line,"\n");
	
	while(pch!= NULL){
		array[i++] = strdup (pch);
		pch = strtok ( NULL, " \n");
	}
	strcpy(cmd , array[0]);
	
	/*for(int j = 0; j < i; j++){
		par[j] = array[j];
	}
	par[i] = NULL;*/
}

void token_command( char cmd[], char *par[]){
	int i = 0;
	int j = 0;
	char* array[100];
	char* pch;

	pch = strtok(cmd," \n");
	
	while(pch!= NULL){
		array[i++] = strdup (pch);
		pch = strtok ( NULL, " \n");
	}
	strcpy(cmd , array[0]);
	
	int k = 0;
	for(j = 0; j < i; j++){
		if(array[j][0] != '&' && array[j][0] != '>' && array[j][0] != '<'){// && array[j][0] != '|'){
			par[k] = array[j];
			k++;
		}
	}	
	//printf("%d",k);
	par[k+1] = NULL;
	for(int i = 0; i < k; i++){
		for(int j = 0; par[i][j] != '\0'; j++){
			if(par[i][j] == '&' && par[i][j+1] == '\0'){
				par[i][j] = '\0';
				break;
			}
		}
	}
	for(int j = 0; j < k; j++){
		//printf("%s\n", par[j]);
	}
}

const char *getUserName()
{
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw){
    return pw->pw_name;
  }

  return "";
}
void type_prompt(){
	int first_time = 1;
	if(first_time){
		const char* CLEAR_SCREEN_ANSI = " \e[1;1H\e[2J";
		write(STDOUT_FILENO,CLEAR_SCREEN_ANSI,12);
		first_time = 0;
	}
}
char* GetCurrentWorkingDir() {
  char buff[FILENAME_MAX];
  getcwd( buff, FILENAME_MAX );
  char* current_working_dir(buff);
  return current_working_dir;
}
void Print_Current_Directory() {
    int i = 0;
    int count = 0;
    char current_working_dir_copy[1024] = {'\0'};
    char buff[FILENAME_MAX];
    getcwd( buff, FILENAME_MAX );
    char* current_working_dir(buff);
    for(; current_working_dir[i] != '\0'; i++){
        if(count == 3){
            break;
        }
        if(current_working_dir[i] == '/'){
            count++;
        }
    }
    current_working_dir_copy[0] = '/';
    for(int k = 1; current_working_dir[i] != '\0'; i++, k++){
        current_working_dir_copy[k] = current_working_dir[i];
    }
    printf(current_working_dir_copy);
}

void Change_Directory_To(char cmd[]){
	char directory_name[100] = {'\0'};

	for(int k = 0; cmd[k] != '\0'; k++){
		directory_name[k] = cmd[k];
	}
	DIR *dir;
	struct dirent *u;
	char *directory_name_2 = directory_name;
	dir = opendir(directory_name);
	if(dir == NULL){
		printf("Directory not found\n");
	}
	else{
		chdir(directory_name);
	}
	closedir(dir);
}
void Change_Directory_Home(){
	char* directory_home;
    char directory_home_copy[1024] = {'\0'};
    directory_home = GetCurrentWorkingDir();
    int i = 0;
    int count = 0;
    for(; directory_home[i] != '\0'; i++){
        if(count == 3){
            break;
        }
        if(directory_home[i] == '/'){
            count++;
        }
    }
	
   for(int k = 0; k < i - 1; k++){
        directory_home_copy[k] = directory_home[k];
    }
	while((strcmp (directory_home, directory_home_copy) != 0)){
		chdir("..");
		directory_home = GetCurrentWorkingDir();
	}
}
 
void ls_Directory(char cmd[]){
    char directory_name[100] = {'\0'};

    for(int k = 0; cmd[k] != '\0'; k++){
        directory_name[k] = cmd[k];
		//printf("%c", cmd[k]);
    }
    DIR *dir;
    struct dirent *u;
    dir = opendir(directory_name);
    if(dir == NULL){
        printf("Directory not found\n");
    }
    else{
        while( ( u = readdir(dir)) != NULL){
            if(u->d_name[0]!='.'){
                printf("%s\n", u->d_name);
            }
        }
    }
    closedir(dir);
}
void ls_Current_Directory(){
    DIR *dir;
    struct dirent *u;
    dir = opendir(".");
    if(dir == NULL){
        printf("Directory not found\n");
    }
    else{
        while( ( u = readdir(dir)) != NULL){
            if(u->d_name[0]!='.'){
                printf("%s\n", u->d_name);
            }
        }
    }
	closedir(dir);
}
void No_Command(char *cmd[]){
	for(int j = 0; cmd[j] != NULL; j++){
		printf("%s\n",cmd[j]);
	}
}

void print_Env(){
	char *Shell_Path = NULL;
	Shell_Path = GetCurrentWorkingDir();
	setenv("PATH", Shell_Path, 1);
	printf("Shell= %s\n", getenv("PATH"));
	int i = 1;
 	char *s = *environ;

 	 for (; s; i++) {
   		 printf("%s\n", s);
    	s = *(environ+i);
  	}
	
}
void set_Env(char cmd[]){

	char variable_name[100] = {'\0'};

	int found = 0;
    int equal = 0;
    char cmd_copy[100] = {'\0'};
    for(int i = 0, l = 0, k = 0; cmd[i] != '\0'; i++, l++ ){
        if(cmd[i] == '='){
            equal = 1;
			i++;
        }
        if(equal == 0){
            variable_name[l] = cmd[i];
        }
        if(equal == 1){
			cmd_copy[k] = cmd[i];
			k++;
		}
    }
	char *Exists = NULL;
	Exists = getenv(variable_name);
	if(Exists == NULL){
		setenv(variable_name, cmd_copy, 0);
	}
	else{
		printf("Variable already set\n3");
	}
	
}

void unset_Env(char variable_name[]){
	char *Exists = NULL;
	Exists = getenv(variable_name);
	if(Exists == NULL){
		printf("Variable doesnt exist\n");
	}
	else{
		unsetenv(variable_name);
	}
	
}

bool strcmp_index(char str1[], char str2[], int index){
	bool match = true;
	for(int i = 0; i < index; i++){
		if(str1[i] != str2[i]){
			match = false;
			return match;
		}
	}
	return match;
}
void sigintHandler(int sig_num){ 
    //signal(SIGINT, sigintHandler); 
    fflush(stdout); 
} 


char* Find_File(char *par[]){
	char file_name[1024] = {'\0'};
	char search[5] = ".txt";
	char *ptr;
	for(int i = 0; par[i] != NULL; i++){
		ptr = strstr(par[i], search);
		if(ptr != NULL){
			for(int j = 0; par[i][j] != '\0'; j++)
			{
				file_name[j] = par[i][j];
			}
			break;
		}
	}
	return file_name;
}

int pipe_proc (int input, int output, char *cmd[]){
  	pid_t child;
	child = fork();
  	if(child == 0){
		if (input != 0){
          dup2 (input, 0);
          close (input);
        }
      if(output != 1){
          dup2 (output, 1);
          close (output);
        }
      return execvp (cmd[0], cmd);
    }
		else if(child > 0){
		wait(NULL);
	}

  return child;
}	
		
int main(int argc, char *argv[]) {
	 printf("\033[1;31m");
 	 printf("Hello world\n");
	 printf("\033[0m");

	char command_1 [1024];
	char command_2 [1024];
	//char *parameter[20];
	const char *user_name = getUserName();
	char* directory;
	int j = 0;
    char hostname[1024];
    gethostname(hostname, 1024);
	char* directory_2;
	int size = strlen(user_name);
	type_prompt();
    bool command_check = false;
  	bool insertion = false;
	bool exertion = false;
	bool and_sign = false;
	bool Pipe_check = false;
   	//int f1, f2;
	

	//pipe(fd);
	while(true){
		int fd[2];
		command_check = false;
		insertion = false;
		exertion = false;
		and_sign = false;
		Pipe_check = false;
		char *parameter[20] = {"\0"};
		char file_read[1024] = {'\0'};
		char file_write[1024] = {'\0'};
		signal(SIGINT, sigintHandler); 
		command_2[0] = '\0';
		printf("\033[1;32m");
		printf(user_name);
		printf("@");
		printf(hostname);
		printf("\033[0m");
		printf(":");
		printf("\033[1;34m");
		printf("~");
        Print_Current_Directory();
		printf("\033[0m");
		printf("$ ");
		
		read_command (command_2, parameter);
		strcpy(command_1, command_2);
		//printf(command_1);
		char search_1[2] = "<";
		char *ptr = strstr(command_2, search_1);
		if (ptr != NULL) {
			//printf("WW");
			insertion = true;
		}
		char search_2[2] = ">";
		char *ptr_2 = strstr(command_2, search_2);
		if(ptr_2 != NULL){
			exertion = true;
		}
		char search_3[2] = "&";
		char *ptr_3 = strstr(command_2, search_3);
		if(ptr_3 != NULL){
			and_sign = true;
		}
		char search_4[2] = "|";
		char *ptr_4 = strstr(command_2, search_4);
		if(ptr_4 != NULL){
			Pipe_check = true;
			//printf("EIEIE");
		}
	//printf(command_2);
		token_command(command_2, parameter);
	if(Pipe_check){
			int i;
  			pid_t child_3;
			child_3 = fork();
			if (child_3 == 0){
  				int input = 0;
				int output = 1;
				int j = 0;
				int k =0;
				//printf("EIEIE");
				char *parameter_copy[20] = {"\0"};
 				for (i = 0; parameter[i] != NULL ;i++){
					if(parameter[i][0] == '|'){
						//printf("oewoiuroew");
						pipe(fd);
						pipe_proc(input, fd[1], parameter_copy);
	 					close(fd[1]);
	 					input = fd[0];
	 					char *parameter_copy[20] = {"\0"};
	 					k = 0;
					}
				else{
					parameter_copy[k] = parameter[i];
					k++;
				}
			}
			for(int l = 0; parameter_copy[l] != NULL; l++){
		}
 			 if (input != 0){
   		 		dup2 (input, 0);
			}
				pipe_proc(input, output, parameter_copy);
				exit(0);
			}
			else if(child_3 > 0){
				wait(NULL);
			}
		}
		//token_command(command_2, parameter);
	else{
		int index_insertion = 0;
		if(insertion){
			bool file_inserted = false;
			char search[5] = ".txt";
			char *ptr;
			for(int i = 0; parameter[i] != NULL; i++){
				ptr = strstr(parameter[i], search);
				if(ptr != NULL){
					index_insertion = i;
					for(int j = 0; parameter[i][j] != '\0'; j++)
					{
						file_read[j] = parameter[i][j];
					}
					file_inserted = true;
					break;
				}
			}
			if( file_inserted == false){
				char search_7[5] = ".cnt";
				char *ptr_7;
				//printf("EOIJEOI");
				for(int i = 0; parameter[i] != NULL; i++){
					ptr_7 = strstr(parameter[i], search_7);
					if(ptr_7 != NULL){
						for(int j = 0; parameter[i][j] != '\0'; j++){
							file_read[j] = parameter[i][j];
						}
						break;
					}	
				}
				//printf(file_write);
			}
				
		}
		if(exertion){
			bool file_exerted = false;
			char search[5] = ".txt";
			char *ptr;
			//printf("%d", index_insertion);
			for(int i = index_insertion + 1; parameter[i] != NULL; i++){
				//printf("%d", i);
				ptr = strstr(parameter[i], search);
				if(ptr != NULL){
					//printf("EJIE");
					for(int j = 0; parameter[i][j] != '\0'; j++){
						file_write[j] = parameter[i][j];
						//printf("EJIE");
					}
					file_exerted = true;
					break;
				}
			}
			if(file_exerted == false){
				char search_6[5] = ".cnt";
				char *ptr_6;
				//printf("EOIJEOI");
				for(int i = 0; parameter[i] != NULL; i++){
					ptr_6 = strstr(parameter[i], search_6);
					if(ptr_6 != NULL){
						for(int j = 0; parameter[i][j] != '\0'; j++){
							file_write[j] = parameter[i][j];
						}
						break;
					}	
				}
				//printf(file_write);
			}
		}
		if(insertion || exertion){
			pid_t child_2 = fork();
			if(child_2 == 0){
				if(insertion){
					int file_desc_read = open(file_read, O_RDONLY);
					if(file_desc_read < 0){
						printf("Error opening input file\n");
						exit(0);
					} 
					dup2(file_desc_read, 0);
				}
				if(exertion){
				int file_desc_write = open(file_write, O_WRONLY | O_CREAT | O_TRUNC, 0666); 
					if(file_desc_write < 0){
						printf("Error opening output file\n");
						exit(0);
					}
					dup2(file_desc_write, 1);
				}
			}	
			if(child_2 > 0){
				wait(NULL);
				continue;
			}
	}	
		
	if(strcmp (command_2, "clear") == 0){
		type_prompt();
        command_check = true;
	}
	int index = 0;
	char search_5[5] = ".txt";
	char *ptr_5;
	for(int i = 0; parameter[i] != NULL; i++){
		ptr_5 = strstr(parameter[i], search_5);
			if(ptr_5 != NULL){
				index = i;
				break;
			}
		}
	//printf("%d",index);
	if(strcmp (command_2, "ls") == 0 && (parameter[1] == NULL || index == 1) && Pipe_check == false){
        ls_Current_Directory();
        command_check = true;
	}
	if(strcmp(command_2,"ls") == 0 && parameter[1] != NULL && parameter[1][0] != '-' && index == 0 && Pipe_check == false){
        ls_Directory(parameter[1]);
        command_check = true;
	}
	if(strcmp(command_2,"cd") == 0 && parameter[1] != NULL && parameter[1][0] != '-'){
		Change_Directory_To(parameter[1]);
       	command_check = true;
	}
	if(strcmp (command_2, "cd") == 0 && parameter[1] == NULL){
		Change_Directory_Home();
        command_check = true;
	}
	
	if(strcmp (command_2, "pwd") == 0){
		directory_2 = GetCurrentWorkingDir();
		printf(directory_2);
		printf("\n");
        command_check = true;
	}
	if(strcmp (command_2, "exit") == 0){
		break;
	}
	if(strcmp(command_2,"setenv")==0){
		command_check = true;
		set_Env(parameter[1]);
	}
	if(strcmp(command_2,"environ")==0){
		command_check = true;
		print_Env();
	}
	if(strcmp(command_2,"unset")==0){
		command_check = true;
		unset_Env(parameter[1]);
	}
	if(command_check == false && Pipe_check == false){
        pid_t child_1 = fork();
		if(child_1 > 0 ){
			command_check = true;
			if(!and_sign){
				waitpid(child_1, NULL, 0);
			}
		}
		else{
			if(and_sign){
		    	sleep(15);
		    }
				//printf("EXEC");
				execvp(parameter[0], parameter);
		}
	}

    if(command_check == false){
       No_Command(parameter);
    }
		if(insertion || exertion){
			exit(0);
		}
	}
    command_check = false;
	insertion = false;
	exertion = false;
	and_sign = false;
	Pipe_check = false;
}
exit(0); 
}

//
//  main.cpp
//  Os Assignment 2
//
//  Created by Nameira Rana on 4/17/20.
//  Copyright © 2020 Nameira Rana. All rights reserved.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <iostream>
using namespace std;
int main(){
    
    int fd[2];
    char Buffer[20];
    char message[12]="HELLO WORLD";
    pipe(fd);
    snprintf(Buffer,sizeof(message),"%d",fd[0]);
    
    pid_t child = fork();
    if(child>0){
        close(fd[0]);
        write(fd[1],message,sizeof(message));
        wait(NULL);
    }
    else if (child==0) {
        close(fd[1]);
        execlp("./new",Buffer,NULL);
    }

    return 0;
}


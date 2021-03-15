#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>

#define MAX_LINE_LEN 1024
#define MAX_FILE_LINE 2048

void strip_extra_spaces(char* str) {
    int i, x;
    for (i = x = 0; str[i]; ++i) {
        if (!isspace(str[i]) || (i > 0 && !isspace(str[i-1]))) {}
            str[x++] = str[i];
    }
    str[x] = '\0';
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("An error has occured\n");
        exit(1);
    }

    FILE* fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("An error has occured\n");
        exit(1);
    }

    char* lineArray[MAX_FILE_LINE];

    int lineNo = 0;
    int i = 0;

    for (i = 0; i < MAX_FILE_LINE; i++) {
        lineArray[i] = (char*) malloc(MAX_LINE_LEN * sizeof(char));
        memset(lineArray[i], 0x00, MAX_LINE_LEN * sizeof(char));
    }

    ssize_t read;
    size_t len = 0;    

    while ((read = getline(&lineArray[lineNo], &len, fp)) != -1) {
        strip_extra_spaces(lineArray[lineNo]);
        lineNo++;
    }

    char** args;
    args = (char**) malloc(sizeof(char*) * 4);
    for (i = 0; i < 4; i++) {
        args[i] = malloc(sizeof(char) * MAX_LINE_LEN);
    }

    for (i = 0; i < lineNo; i++) {
     
        sscanf(lineArray[i], "%s %s %s", args[0], args[1], args[2]);
 
        args[3] = NULL;
        
        pid_t pid = fork();

        if (pid == 0) {

            execvp(args[0], args);
            exit(EXIT_SUCCESS);  
        }
        else if (pid > 0) {
    
            int statusPtr = 0;
            waitpid(pid, &statusPtr, 0);
        }
        else {
            printf("An error has occured\n");
        }     
    }

    fclose(fp);
    for (i = 0; i < MAX_FILE_LINE; i++) {
        free(lineArray[i]);
    }

    for (i = 0; i < 4; i++) {
        free(args[i]);
    }

    free(args);

    return 0;
}

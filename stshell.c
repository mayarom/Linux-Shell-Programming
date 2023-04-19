#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_ARGS 10
#define MAX_COMMANDS 10
#define MAX_PIPE 2
#define BUFFER_SIZE 1024

void handle_sigint(int sig) {
    printf("\n"); // print a newline after the Ctrl+C is pressed
}

void execute_command(char *args[]) {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        printf("Error: Fork failed.\n");
        exit(1);
    } else if (pid == 0) {
        if (execvp(args[0], args) < 0) {
            printf("Error: Execution failed.\n");
            exit(1);
        }
    } else {
        wait(NULL);
    }
}

void execute_pipe(char *commands[MAX_COMMANDS][MAX_ARGS], int pipe_count) {
    int i, in = 0, fd[2];
    pid_t pid;

    for (i = 0; i < pipe_count; i++) {
        pipe(fd);
        pid = fork();

        if (pid < 0) {
            printf("Error: Fork failed.\n");
            exit(1);
        } else if (pid == 0) {
            dup2(in, 0);

            if (i < pipe_count - 1) {
                dup2(fd[1], 1);
            }

            close(fd[0]);

            if (execvp(commands[i][0], commands[i]) < 0) {
                printf("Error: Execution failed.\n");
                exit(1);
            }
        } else {
            wait(NULL);
            close(fd[1]);
            in = fd[0];
        }
    }
}

void parse_command(char *command, char *args[]) {
    int i = 0;
    char *token = strtok(command, " \t\n");
    while (token != NULL) {
        args[i] = token;
        i++;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

int main() {
    char buffer[BUFFER_SIZE], *commands[MAX_COMMANDS][MAX_ARGS];
    int pipe_count = 0;
    signal(SIGINT, handle_sigint);

    while (1) {
        printf("stshell> ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if (strcmp(buffer, "exit\n") == 0) {
            exit(0);
        }

        // parse commands separated by pipes
        char *token = strtok(buffer, "|");
        while (token != NULL) {
            char *command = token;
            char *args[MAX_ARGS];
            parse_command(command, args);
            commands[pipe_count][0] = args[0];
            int i;
            for (i = 0; args[i] != NULL; i++) {
                commands[pipe_count][i] = args[i];
            }
            commands[pipe_count][i] = NULL;
            pipe_count++;
            token = strtok(NULL, "|");
        }

        // execute the commands using pipes
        if (pipe_count == 1) {
            execute_command(commands[0]);
        } else {
            execute_pipe(commands, pipe_count);
        }

        // reset pipe count
        pipe_count = 0;
    }
    return 0;
}



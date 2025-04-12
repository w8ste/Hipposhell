#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define RED "\033[0;31m"
#define RESET "\e[0m"

#define TK_BUFF_SIZE 64
#define TOKEN_DELIMITER " \t\r\n"
#define history_path "./hipposhell_history"

void check_buffer(char *buffer);
void check_tokens(char **buffer);
char *read_command();
char **tokenize(char *buffer);

// main programm loop
void loop() {
    int status = 1, stat_loc;

    char *buffer;
    char **tokens;

    pid_t pid_child;
    FILE *file_ptr;

    while (status) {
        printf("<(^^)> ");
        buffer = read_command();
        tokens = tokenize(buffer);

        char **tokens_to_file = tokens;

        pid_child = fork();

        if(0 == pid_child) {
            // Execute command received as input
            if (execvp(tokens[0], tokens) < 0) {
                fprintf(stderr, "%s%s: command not found%s\n", RED, tokens[0], RESET);
                exit(0);
            }
        }
        else if (-1 == pid_child) {
            fprintf(stderr, "%sAn error has occured whilst trying to fork!%s\n", RED, RESET);
        }
        else {
            // Parent waiting for child process to die
            waitpid(pid_child, &stat_loc, WUNTRACED);
        }

        file_ptr = fopen(history_path, "a");

        if(NULL == file_ptr) {
            fprintf(stderr, "%sAn error has occured whilst trying to write the history file!%s\n", RED, RESET);
        }
        else {
            while (*tokens_to_file != NULL) {
                fprintf(file_ptr, "%s ", *tokens_to_file);
                tokens_to_file++;
            }
            fprintf(file_ptr, "\n");
            fclose(file_ptr);
        }

        free(buffer);
        free(tokens);
    }
}

// tokenize input
char **tokenize(char *buffer) {
    const int buffsteps = 64;
    int buffsize = buffsteps, buf_pos = 0;

    char **tokens = malloc(TK_BUFF_SIZE * sizeof(char *));

    check_tokens(tokens);

    char *token = strtok(buffer, TOKEN_DELIMITER);

    while(NULL != token) {
        tokens[buf_pos] = token;
        buf_pos++;

        if(buf_pos >= buffsize) {
            buffsize += buffsteps;
            tokens = realloc(tokens, buffsize);

            check_tokens(tokens);
        }


        token = strtok(NULL, TOKEN_DELIMITER);
    }

    return tokens;
}

// read in lines from the cli
char *read_command() {
    const int buffer_steps = 512;
    int buffsize = buffer_steps;
    char *buffer = malloc(sizeof(char) * buffsize);

    check_buffer(buffer);

    int buf_pos = 0;
    int c = getchar();

    while(c != EOF && c != '\n') {
        buffer[buf_pos] = c;
        buf_pos++;

        if(buf_pos >= buffsize) {
            buffsize += buffer_steps;
            buffer = realloc(buffer, buffer_steps);
            check_buffer(buffer);
        }

        c = getchar();
    }

    buffer[buf_pos] = '\0';
    return buffer;
}

// checks if the buffer is correctly initialized
void check_buffer(char *buffer) {
    if(!buffer) {
        fprintf(stderr, "%s<(^^)>: Allocation did not succeed in buffer creation%s\n", RED, RESET);
        exit(EXIT_FAILURE);
    }
}

// checks if the token is correctly initialized
void check_tokens(char **buffer) {
    if(!buffer) {
        fprintf(stderr, "%s<(^^)>: Allocation did not succeed token creation%s\n", RED, RESET);
        exit(EXIT_FAILURE);
    }

}

// exit the shell
int exit_shell() {
    return 0;
}

int main() {
    loop();
    return 0;
}

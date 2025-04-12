#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\033[0;31m"
#define RESET "\e[0m"

#define TK_BUFF_SIZE 64
#define TOKEN_DELIMITER " \t\r\n"
void check_buffer(char *buffer);
void check_tokens(char **buffer);
char *read_command();
char **tokenize(char *buffer);
void loop() {
    int status = 1;
    while (status) {
        printf("<(^^)> ");
        char *buffer = read_command();
        tokenize(buffer);
    }
}

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

void check_buffer(char *buffer) {
    if(!buffer) {
        fprintf(stderr, "%s<(^^)>: Allocation did not succeed in buffer creation%s\n", RED, RESET);
        exit(EXIT_FAILURE);
    }
}

void check_tokens(char **buffer) {
    if(!buffer) {
        fprintf(stderr, "%s<(^^)>: Allocation did not succeed token creation%s\n", RED, RESET);
        exit(EXIT_FAILURE);
    }

}

int main() {
    loop();
    return 0;
}

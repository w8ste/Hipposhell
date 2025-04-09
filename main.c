#include <stdio.h>
#include <stdlib.h>

#define RED "\033[0;31m"
#define RESET "\e[0m"

void check_buffer(char * buffer);
char *read_command();

void loop() {
    int status = 1;
    while (status) {
        printf("<(^^)> ");
        read_command();
    }
}

char *read_command() {
    const int buffer_steps = 512;
    int buffsize = buffer_steps;
    char *buffer = malloc(sizeof(char) * buffsize);

    //check if buffer has been properly allocated
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
        fprintf(stderr, "%s<(^^)>: Allocation did not succeed%s", RED, RESET);
        exit(EXIT_FAILURE);
    }
}

int main() {
    loop();
    return 0;
}

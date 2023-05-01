#include "kernel/types.h"
#include "user/user.h"

void newfork(int _pipe[2]) {
    int n;
    int prime;
    close(_pipe[1]); 
    if (read(_pipe[0], &prime, 4) != 4) {
        fprintf(2, "Error: read from pipe wrong!\n");
        close(_pipe[0]);
        exit(1);
    }
    fprintf(1, "prime %d\n", prime);
    if (read(_pipe[0], &n, 4) == 4) {
        int _pipe2[2];
        pipe(_pipe2);
        if (!fork()) {
            close(_pipe2[0]);
            if (n % prime) write(_pipe2[1], &n, 4);
            while (read(_pipe[0], &n, 4) == 4) {
                if (n % prime) write(_pipe2[1], &n, 4);
            }
            close(_pipe2[1]);
            close(_pipe[0]);
            wait(0);
        } else {
            close(_pipe[0]);
            newfork(_pipe2);
        }
    }
}

int main(int argc, char *argv[]) {
    int _pipe[2];
    pipe(_pipe);
    if (fork() == 0) {
        newfork(_pipe);
    } else {
        close(_pipe[0]);
        for (int i = 2; i <= 35; i++) {
            if (write(_pipe[1], &i, 4) != 4) {
                fprintf(2, "Error: write to pipe wrong!\n");
                close(_pipe[1]);
                exit(1);
            }
        }
        close(_pipe[1]);
        wait(0);
    }
    exit(0);
}
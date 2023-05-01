#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p[2];
    pipe(p);    
    if (fork() == 0) {
        char bf[1];
        read(p[0], bf, 1);
        close(p[0]);
        fprintf(1, "%d: received ping\n", getpid());
        write(p[1], bf, 1);
        close(p[1]);
    } else {
        char bf[1];
        bf[0] = '1';
        write(p[1], bf, 1);
        close(p[1]);
        read(p[0], bf, 1);
        fprintf(1, "%d: received pong\n", getpid());
        close(p[0]);
    }
    exit(0);
}

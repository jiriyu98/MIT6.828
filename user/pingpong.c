#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc > 1) exit(-1);
    
    int p[2];
    pipe(p);

    char ch[1] = {'a'};

    if (fork() == 0) {
        read(p[0], ch, 1);
        fprintf(2, "%d: received ping\n", getpid());
        write(p[1], ch, 1);
        exit(0);
    } else {
        write(p[1], ch, 1);
        wait(0);
        read(p[0], ch, 1);
        fprintf(2, "%d: received pong\n", getpid());
    }
    exit(0);
}
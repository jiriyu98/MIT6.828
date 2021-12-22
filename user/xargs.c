#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int help_gets(char *buf, int max);

int main(int argc, char *argv[]) {
    if (argc < 2) exit(-1);

    char *_argv[argc+1];
    for (int i=0; i<argc-1; ++i) {
        _argv[i] = argv[i+1];
    }
    _argv[argc] = 0;

    char parameter[512];

    while (help_gets(parameter, 512) > 0) {
        if (fork() == 0) {
            _argv[argc-1] = parameter;
            exec(_argv[0], _argv);
        } else {
            wait(0);
        }
    }
    exit(0);
}

int help_gets(char *buf, int max) {
    int i, cc;
    char c;

    for (i=0; i+1 < max; ) {
        cc = read(0, &c, 1);
        if (cc < 1)
            break;
        if (c == '\n')
            break;
        buf[i++] = c;
    }
    buf[i] = '\0';
    return i;
}
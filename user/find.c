#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char *path, char *filename);
char* fmtname(char *path);

int main(int argc, char *argv[]) {
    if (argc < 3) exit(-1);
    find(argv[1], argv[2]);
    exit(0);
}

char* fmtname(char *path) {
    char *p;
    for(p = path+strlen(path); p >= path && *p != '/'; --p)
    ;
    p++;
    return p;
}

void find(char *path, char *filename) {
    int fd;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(-1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(-1);
    }

    char* fmtpath = fmtname(path);
    int len = strlen(path);
    struct dirent de;
    
    switch (st.type) {
        case T_FILE: 
            if (!strcmp(fmtpath, filename))
                fprintf(1, "%s\n", path);
            break;
        case T_DIR:
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0)
                    continue;
                if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
                    continue;
                char p[512];
                strcpy(p, path);
                strcpy(p+len, "/");
                strcpy(p+len+1, de.name);
                find(p, filename);
            }
            break;
    }
    close(fd);
}
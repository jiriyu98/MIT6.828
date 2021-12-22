#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes();

int main(int argc, char *argv[]) {
    if (argc > 1) exit(-1);
    
    int nums[34];
    for (int i=0; i<=33; ++i) nums[i] = 2+i;
    primes(nums, 34);
    exit(0);
}

void primes(int *nums, int size) {
    if (size == 0) return;

    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[1]);
        int i = 0;
        while (read(p[0], nums+i, sizeof(int))) {
            i++;
        }
        close(p[0]);
        primes(nums, i);
    } else {
        close(p[0]);
        int output = nums[0];
        printf("prime %d\n", output);
        for (int i=0; i<size; ++i) {
            if (nums[i] % output == 0) continue;
            write(p[1], nums+i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}
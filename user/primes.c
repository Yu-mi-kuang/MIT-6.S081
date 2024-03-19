#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void solve(int* pl) {
    int prime;
    if ((read(pl[0], &prime, sizeof prime)) != 0) {
        printf("prime %d\n", prime);
    } else {
        return;
    }

    int pr[2];
    pipe(pr);

    int pid = fork();
    if (pid > 0) {
        close(pr[0]);

        int num;
        while (read(pl[0], &num, sizeof num)) {
            if(num % prime != 0) {
                write(pr[1], &num, sizeof num);
            }
        }

        close(pl[0]);
        close(pr[1]);
        wait(0);
    } else if (pid == 0) {
        close(pr[1]);
        solve(pr);
        close(pr[0]);
    } else {
        printf("fork error!\n");
        exit(1);
    }
}

int main() {
    int pl[2];
    pipe(pl);

    int pid = fork();
    if (pid > 0) {
        close(pl[0]);
        for (int i = 2; i <= 35; i++) {
            write(pl[1], &i, sizeof i);
        }
        close(pl[1]);
        wait(0);
    } else if (pid == 0) {
        close(pl[1]);
        solve(pl);
        close(pl[0]);
    } else {
        printf("fork error!\n");
        exit(1);
    }

    exit(0);
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int child_to_parent[2], parent_to_child[2];
    pipe(child_to_parent), pipe(parent_to_child);

    int size;
    char buf[5];

    int pid = fork();
    if (pid == 0) {
        // 子进程
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        // 写入到父进程
        write(child_to_parent[1], "pong\n", 5);

        // 从父进程读取
        size = read(parent_to_child[0], buf, sizeof buf);
        if (size > 0) {
            printf("%d: received ", getpid());
            write(1, buf, size);
        } else {
            printf("%d: receive failed\n", getpid());
        }
    } else if (pid > 0) {
        // 父进程
        close(child_to_parent[1]);
        close(parent_to_child[0]);

        // 写入到子进程
        write(parent_to_child[1], "ping\n", 5);

        wait(0);

        // 从子进程读取
        size = read(child_to_parent[0], buf, sizeof buf);
        if(size > 0) {
            printf("%d: received ", getpid());
            write(1, buf, sizeof buf);
        } else {
            printf("%d: receive failed\n", getpid());
        }
    } else {
        printf("fork error\n");
        exit(1);
    }

    exit(0);
}

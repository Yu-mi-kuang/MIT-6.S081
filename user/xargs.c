#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    char buf[512];
    char* command[MAXARG];

    for (int i = 1; i < argc; i++) {
        command[i - 1] = argv[i];
    }

    while (1) {
        int i = 0;
        while (read(0, &buf[i], 1) == 1) {
            if (buf[i] != '\n') {
                i ++;
            } else {
                break;
            }
        }

        if (i == 0) {
            break;
        }

        buf[i] = 0;
        command[argc - 1] = buf;

        int pid = fork();
        if (pid == 0) {
            exec(command[0], command);
            exit(0);
        } else {
            wait(0);
        }
    }

    exit(0);
}

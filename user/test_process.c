#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>


#define CLONE_FOR_TEST 0x400000000ULL

int child_fn(void *arg) {
    int i = 0;
    printf("Child process started with PID: %d\n", getpid());

    while (1) {
        // printf("%d\n", i);
        __asm__ volatile("nop");
    }

    printf("Child process finish\n");
}

int main() {
    struct clone_args cl_args;

    memset(&cl_args, 0, sizeof(cl_args));

    cl_args.flags = 0 | CLONE_FOR_TEST;
    cl_args.exit_signal = SIGCHLD;

    pid_t tid = syscall(SYS_clone3, &cl_args, sizeof(cl_args));
    if (tid == -1) {
        perror("clone failure");
        exit(1);
    }else if (tid == 0) {
        // Child
        child_fn(NULL);
        _exit(0);
    } else {
        // Parent
        printf("Created child with PID=%d\n", tid);
        exit(0);
    }

    return 0;
}
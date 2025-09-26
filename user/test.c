#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

// Helper to print process info
void print_proc_info(const char *label) {
  printf("[%s] pid=%d ppid=%d\n", label, getpid(), getppid());
}

// Test 1: Simple fork and wait
void test_fork_wait() {
  printf("Test 1: fork and wait\n");
  pid_t pid = fork();
  if (pid == 0) {
    // Child
    print_proc_info("child");
    _exit(0);
  } else if (pid > 0) {
    // Parent
    int status;
    waitpid(pid, &status, 0);
    printf("Parent: child exited with status %d\n", WEXITSTATUS(status));
  } else {
    perror("fork");
  }
}

// Test 2: Multiple children, check scheduling order
void test_multiple_children() {
  printf("Test 2: multiple children\n");
  int n = 3;
  pid_t pids[3];
  for (int i = 0; i < n; ++i) {
    pids[i] = fork();
    if (pids[i] == 0) {
      printf("Child %d running (pid=%d)\n", i, getpid());
      sleep(1 + i); // Staggered sleep
      printf("Child %d exiting\n", i);
      _exit(i);
    }
  }
  for (int i = 0; i < n; ++i) {
    int status;
    pid_t wpid = waitpid(pids[i], &status, 0);
    printf("Parent: child %d (pid=%d) exited with %d\n", i, wpid,
           WEXITSTATUS(status));
  }
}

// Test 3: Priority (nice) effect
void test_nice_effect() {
  printf("Test 3: nice/priority effect\n");
  pid_t pid = fork();
  if (pid == 0) {
    printf("Child: setting nice value to 10\n");
    if (nice(10) == -1 && errno != 0) {
      perror("nice");
    }
    for (volatile int i = 0; i < 100000000; ++i)
      ; // Busy loop
    printf("Child: done\n");
    _exit(0);
  } else if (pid > 0) {
    printf("Parent: running busy loop\n");
    for (volatile int i = 0; i < 100000000; ++i)
      ;
    printf("Parent: done\n");
    int status;
    waitpid(pid, &status, 0);
  } else {
    perror("fork");
  }
}

// Test 4: Yielding (sched_yield)
void test_sched_yield() {
  printf("Test 4: sched_yield\n");
  pid_t pid = fork();
  if (pid == 0) {
    printf("Child: yielding CPU\n");
    sched_yield();
    printf("Child: after yield\n");
    _exit(0);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    perror("fork");
  }
}

int main() {
  printf("=== Scheduling Behavior Tests ===\n");
  test_fork_wait();
  printf("\n");
  test_multiple_children();
  printf("\n");
  test_nice_effect();
  printf("\n");
  test_sched_yield();
  printf("=== Tests complete ===\n");
  return 0;
}

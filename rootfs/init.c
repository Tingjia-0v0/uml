#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

// Simple shell for UML root filesystem
void print_prompt() {
  printf("UML# ");
  fflush(stdout);
}

// Built-in commands
int builtin_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "cd: expected argument\n");
    return 1;
  }
  if (chdir(args[1]) != 0) {
    perror("cd");
  }
  return 0;
}

int builtin_exit() { return reboot(RB_HALT_SYSTEM); }

int builtin_help() {
  printf("Built-in commands:\n");
  printf("  help          - Show this help\n");
  printf("  cd <dir>      - Change directory\n");
  printf("  ls            - List directory contents\n");
  printf("  cat <file>    - Display file contents\n");
  printf("  echo <text>   - Print text\n");
  printf("  Ctrl+C / exit - Exit the shell\n");
  return 1;
}

int builtin_ls() {
  DIR *dir = opendir(".");
  if (dir == NULL) {
    perror("ls");
    return 1;
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    printf("%s  \n", entry->d_name);
  }
  closedir(dir);
  return 0;
}

int builtin_cat(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "cat: expected argument\n");
    return 1;
  }

  FILE *file = fopen(args[1], "r");
  if (file == NULL) {
    perror("cat");
    return 1;
  }

  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), file)) {
    printf("%s", buffer);
  }
  fclose(file);
  return 0;
}

int builtin_echo(char **args) {
  for (int i = 1; args[i] != NULL; i++) {
    printf("%s ", args[i]);
  }
  printf("\n");
  return 0;
}

// Array of built-in commands
struct builtin {
  char *name;
  int (*func)(char **);
};

struct builtin builtins[] = {
    {"cd", builtin_cd}, {"exit", builtin_exit}, {"help", builtin_help},
    {"ls", builtin_ls}, {"cat", builtin_cat},   {"echo", builtin_echo},
    {NULL, NULL},
};

// Check if command is built-in
int is_builtin(char *cmd) {
  for (int i = 0; builtins[i].name != NULL; i++) {
    if (strcmp(cmd, builtins[i].name) == 0) {
      return i;
    }
  }
  return -1;
}

// Parse input line into arguments
char **parse_line(char *line) {
  static char *args[MAX_ARGS];
  int argc = 0;
  char *token = strtok(line, " \t\n\r");

  while (token != NULL && argc < MAX_ARGS - 1) {
    args[argc++] = token;
    token = strtok(NULL, " \t\n\r");
  }
  args[argc] = NULL;
  return args;
}

// Execute external command
int execute_external(char **args) {
  pid_t pid = fork();

  if (pid == 0) {
    // Child process
    execvp(args[0], args);
    perror("execvp");
    exit(1);
  } else if (pid > 0) {
    // Parent process
    int status;
    waitpid(pid, &status, 0);
  } else {
    perror("fork");
    return 1;
  }
  return 1;
}

// Main shell loop
void shell_loop() {
  printf("Welcome to UML Simple Root Filesystem\n");
  builtin_help();

  while (1) {
    print_prompt();

    char line[MAX_LINE];
    if (fgets(line, sizeof(line), stdin) == NULL) {
      printf("\n");
      break;
    }

    char **args = parse_line(line);
    if (args[0] == NULL)
      continue;

    int index = is_builtin(args[0]);
    if (index >= 0) {
      builtins[index].func(args);
    } else {
      execute_external(args);
    }
  }
  builtin_exit();
}

int main() {
  // Initialize system
  printf("Initializing UML Root Filesystem...\n");

  // Start the shell
  shell_loop();
  return 0;
}

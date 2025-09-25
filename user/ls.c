#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main() {
  DIR *dir = opendir(".");
  if (dir == NULL) {
    perror("ls");
    return 1;
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }
    printf("%s  \n", entry->d_name);
  }
  closedir(dir);
}

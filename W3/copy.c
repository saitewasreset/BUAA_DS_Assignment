#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    char path[100] = {0};
    getcwd(path, (unsigned)100);
    printf("current working directory: %s\n", path);
    char name[100] = {0};
    gethostname(name, (unsigned)100);
    printf("hostname: %s\n", name);

    unsigned long uid = getuid();
    printf("UID: %ld\n", uid);

    unsigned long pid = getpid();
    printf("PID: %ld\n", pid);

    struct dirent *dp;
    DIR *dir = opendir(".");

    if (!dir) {
        return 0;
    }

    while ((dp = readdir(dir)) != NULL) {
        printf("%s\n", dp->d_name);
    }
    // printf("Running ls -al\n");
    // execlp("ls", "-a", "-l", "/", NULL);

    // printf("Running whoami\n");
    // execlp("whoami", NULL);
    // printf("Running ps aux\n");
    // execlp("ps", "aux", NULL);
    execlp("xz", "xz", "--version", NULL);
    return 0;
}
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>

#define PRELOAD_SIZE 255

struct option opts[] = {
       {"path", 0, NULL, 'p'},
       {"directory", 1, NULL, 'd'},
       {"--", 1, NULL, 'd'},
       {"help", 0, NULL, 'h'}
};

int get_exe_path(char* name, char* buf) {
    char* env_path = getenv("PATH");
    struct stat buffer;
    int exists;
    // make current dir's exe can be found
    char *token = strtok(strcat(env_path, ":."), ":");
    while( token != NULL )
    {
        sprintf(buf, "%s/%s", token, name);
        exists = stat(buf, &buffer);
        if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
            return 0;
        }
        token = strtok(NULL, ":");
    }
    fprintf(stderr, "%s ,command not found!\n", name);
    exit(1);
}

int main(int argc, char *argv[]) {
    int cmd_opt = 0;
    char PRELOAD_PATH[PRELOAD_SIZE - 50];
    char CWD[PRELOAD_SIZE - 50];
    strcpy(CWD, getcwd(NULL,0));
    char *exe = malloc(512);
    // default hijack library path 
    realpath("./sandbox.so", PRELOAD_PATH);
    while(1) {
        cmd_opt = getopt(argc, argv, "p:d:");
        /* End condition always first */
        if (cmd_opt == -1) {
            break;
        }
        switch (cmd_opt) {
            case 'p':
                realpath(optarg, PRELOAD_PATH);
                strcpy(PRELOAD_PATH, optarg);
                break;
            case 'd':
                if (realpath(optarg, CWD) == NULL){
                    fprintf(stderr, "restrict directory path error: %s\n", CWD);
                    exit(1);
                }
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n", argv[0]);
                fprintf(stderr, "\t-p: set the path to sandbox.so, default = ./sandbox.so\n");
                fprintf(stderr, "\t-d: restrict directory, default = .\n");
                fprintf(stderr, "\t--: seperate the arguments for sandbox and for the executed command\n");
                return 0;
                break;
        }
    }
    if (argc == 1){ /* no args */
        fprintf(stderr, "no command given.\n");
        exit(1);
    }
    char PRELOAD[PRELOAD_SIZE] = "LD_PRELOAD=";
    char CWD_ENV[PRELOAD_SIZE] = "CWD=";
    char *env[3] = {strcat(PRELOAD, PRELOAD_PATH), strcat(CWD_ENV, CWD), NULL};
    get_exe_path(argv[optind], exe);
    execvpe(exe, argv+optind, env);
    return 0;
}

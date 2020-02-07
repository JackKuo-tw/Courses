#ifndef WEBSERVER_H_
#define WEBSERVER_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "http.h"

void serv_client(int, struct sockaddr_in *);
FILE *open_target_file(char *);
int file_exist(char *);
void request_handler(char *, int);
void read_dir(int fd, char *path);
void read_target_file(FILE *fp, int fd);
extern char base_dir[30];
extern const char INDEX[30];
extern bool dir_no_slash;
extern char uri[MAXSIZE];
extern char ext[10];
extern char method[5];
extern char query_string[2000];
void execute_cgi(char *filename, char* buffer);
char addr_str[100];
char port_str[10];

#endif

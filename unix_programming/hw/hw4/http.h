#ifndef HTTP_H_
#define HTTP_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAXSIZE 1024
extern char **environ;

void encapsulate_http(int, int);
int parse_request(char *, char *);
void send_with_check(int fd, const void *buf, size_t n, int flags);
void find_question_mark(char *filename);
void http_403(int fd);
void http_404(int fd);
void http_301(int fd);
void parse_http_header(char *pch);
void search_http_header(char *target, char *result);
void hyper_reference(char *target, char *dest);
void get_mime_type(char *target, char *dest);
void get_ext(char *filename);
void set_header_env();
bool is_dynamic;

#endif

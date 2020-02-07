#ifndef SANDBOX_H_
#define SANDBOX_H_

#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>

/* some functions need it */
struct statx *statxbuf;
struct stat64 * stat_buf;

void path_guard_print(const char *fname, const char *path);
void print(const char *s, const char *fname, const char *path);
void print(const char *format, const char *fname, const char *path);
char check_path(const char *given_path);

#endif

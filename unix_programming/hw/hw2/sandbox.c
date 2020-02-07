#include "sandbox.h"

#define comma ,
#define GET_FROM_REAL_LIB(fname, ...)                                            \
            void *handle = dlopen("libc.so.6", RTLD_LAZY);                       \
            static uid_t (*old_getuid)() = NULL;                                 \
            if(handle != NULL) {                                                 \
                old_getuid = dlsym(handle, #fname);                              \
            } else {                                                             \
                printf("Error!");                                                \
                exit(1);                                                         \
            }                                                                    \
            if (old_getuid != NULL) {                                            \
                old_getuid(__VA_ARGS__);                                         \
            }                                                                    
#define	MAKEFUNC(fname, R, err_value, para, ...)                                      \
	R fname(__VA_ARGS__) {                                                            \
            if (check_path(path) == 'F'){                                             \
                path_guard_print(#fname , path);                                      \
                return err_value;                                                     \
            }                                                                         \
        GET_FROM_REAL_LIB(fname, para);                                               \
    }
#define MAKEFUNC2PATH(fname, R, err_value, para, ...)                                 \
    R fname(__VA_ARGS__) {                                                            \
            if (check_path(path1) == 'F'){                                            \
                path_guard_print(#fname , path1);                                     \
                return err_value;                                                     \
            }                                                                         \
            if (check_path(path2) == 'F'){                                            \
                path_guard_print(#fname , path2);                                     \
                return err_value;                                                     \
            }                                                                         \
        GET_FROM_REAL_LIB(fname, para);                                               \
    }
#define MAKEEXEC(fname, R, err_value, para, ...) \
    R fname(__VA_ARGS__) { \
        print("[sandbox] %s(%s): not allowed\n", #fname, path); \
        errno = EACCES; \
        return err_value; \
    }


void path_guard_print(const char *fname, const char *path) {
    print("[sandbox] %s: access to %s is not allowed\n", fname, path);
}

/* Force print to user's tty even if it redirects to /dev/null */
void print(const char *format, const char *fname, const char *path) {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    static FILE* (*old_open)() = NULL;
    old_open = dlsym(handle, "fopen");
    FILE* f = old_open("/dev/tty", "a+");
    char s[200];
    sprintf(s, format, fname, path);
    fprintf(f, s, strlen(s));
}

char check_path(const char *given_path) {
    char cwd[255];
    strcpy(cwd, getenv("CWD"));
    char rpath[1024];
    realpath(given_path, rpath);
    char *loc = strstr(rpath, cwd);
    if (loc == NULL || (loc - rpath) > 0){
        return 'F';
    }
    return 'T';
}

/* 
 * change family 
*/
MAKEFUNC(chdir, int, -1, *path  ,const char *path);
MAKEFUNC(chmod, int, -1, *path comma mode  ,const char *path, mode_t mode);
// chmod command use this
MAKEFUNC(fchmodat, int, -1, dirfd comma *path comma mode comma flags ,int dirfd, const char *path, mode_t mode, int flags);
MAKEFUNC(chown, int, -1, *path comma owner comma group ,const char *path, uid_t owner, gid_t group);
// chown command use this
MAKEFUNC(fchownat, int, -1, dirfd comma path comma owner comma group comma flags, int dirfd, const char *path, uid_t owner, gid_t group, int flags);

/* 
 * open family
*/
// MAKEFUNC(open, int, -1, path comma oflag  , const char *path, int oflag);
int open(const char *path, int oflag, ...) {
    if (check_path(path) == 'F'){
        path_guard_print("open", path);
        return -1;
    }
    GET_FROM_REAL_LIB(open, path, oflag);
}
int open64(const char *path, int oflag, ...) {
    if (check_path(path) == 'F'){
        path_guard_print("open64", path);
        return -1;
    }
    GET_FROM_REAL_LIB(open, path, oflag);
}
MAKEFUNC(fopen, FILE*, NULL, path comma mode, const char *path, const char *mode);
MAKEFUNC(fopen64, FILE*, NULL, path comma mode, const char *path, const char *mode);
int openat(int dirfd, const char *path, int flags, ...) {
    if (check_path(path) == 'F'){
        path_guard_print("openat", path);
        return -1;
    }
    GET_FROM_REAL_LIB(openat, path, flags);
}
int openat64(int dirfd, const char *path, int flags, ...) {
    if (check_path(path) == 'F'){
        path_guard_print("openat64", path);
        return -1;
    }
    GET_FROM_REAL_LIB(openat, path, flags);
}
// MAKEFUNC(openat, int, -1, dirfd comma path comma flags, int dirfd, const char *path, int flags);
MAKEFUNC(opendir, DIR*, NULL, path, const char *path);
MAKEFUNC(readlink, ssize_t, -1, path comma buf comma bufsiz, const char *path, char *buf, size_t bufsiz);
MAKEFUNC(remove, int, -1, path, const char *path);
MAKEFUNC2PATH(rename, int, -1, path1 comma path2, const char *path1, const char *path2);
MAKEFUNC(rmdir, int, -1, path, const char *path);

/* stat family: http://refspecs.linuxbase.org/LSB_3.0.0/LSB-PDA/LSB-PDA/baselib-xstat-1.html
 * __xstat  shall behave as `stat`  as specified by ISO POSIX (2003).
 * __lxstat shall behave as `lstat` as specified by ISO POSIX (2003).
 * __fxstat shall behave as `fstat` as specified by ISO POSIX (2003).
 * __xstat(), __lxstat(), and __fxstat() are not in the source standard; they are only in the binary standard.
*/
/* for stat */
MAKEFUNC(__xstat, int, -1, ver comma path comma stat_buf  ,int ver, const char * path, struct stat * stat_buf);
MAKEFUNC(__xstat64, int, -1, ver comma path comma stat_buf  ,int ver, const char * path, struct stat64 * stat_buf)
/* for lstat */
MAKEFUNC(__lxstat, int, -1, ver comma path comma stat_buf  ,int ver, const char * path, struct stat * stat_buf);
MAKEFUNC(__lxstat64, int, -1, ver comma path comma stat_buf  ,int ver, const char * path, struct stat64 * stat_buf);
/* __fxstat does not do anything about file */
MAKEFUNC(statx, int, -1, path comma flags comma mask comma statxbuf, const char *path, int flags, unsigned int mask, struct statx *statxbuf);

/* 
 * others 
*/
MAKEFUNC(creat, int, -1, path comma mode, const char *path, mode_t mode);
MAKEFUNC(creat64, int, -1, path comma mode, const char *path, mode_t mode);
MAKEFUNC2PATH(link, int, -1, path1 comma path2, const char *path1, const char *path2);
MAKEFUNC2PATH(linkat, int, -1, fd1 comma path1 comma fd2 comma path2 comma flag, int fd1, const char *path1, int fd2, const char *path2, int flag);
MAKEFUNC2PATH(symlink, int, -1, fd1 comma path1 comma fd2 comma path2 comma flag, int fd1, const char *path1, int fd2, const char *path2, int flag);
MAKEFUNC(unlink, int, -1, path, const char* path);
MAKEFUNC(unlink64, int, -1, fd comma path comma flag, int fd, const char* path, int flag);
MAKEFUNC(mkdir, int, -1, path comma mode, const char *path, mode_t mode);
MAKEFUNC(mkdirat, int, -1, fd comma path comma mode, int fd, const char *path, mode_t mode);

/* 
 * exec family
*/
MAKEEXEC(execl, int, -1, path comma arg, const char *path, const char *arg, ...);
MAKEEXEC(execlp, int, -1, path comma arg, const char *path, const char *arg, ...);
MAKEEXEC(execle, int, -1, path comma arg, const char *path, const char *arg, ...);
MAKEEXEC(execv, int, -1, path comma argv, const char *path, char *const argv[]);
MAKEEXEC(execvp, int, -1, path comma argv, const char *path, char *const argv[]);
MAKEEXEC(execve, int, -1, path comma argv comma envp, const char *path, char *const argv[], char *const envp[]);
MAKEEXEC(execvpe, int, -1, path comma argv comma envp, const char *path, char *const argv[], char *const envp[])
MAKEEXEC(system, int, -1, path, const char *path);

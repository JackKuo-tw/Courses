#include "webserver.h"
#include <dirent.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int fd, pfd, val;
    struct sockaddr_in sin, psin;
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s port path_to_your_webserver_docroot\n", argv[0]);
        return (-1);
    }
    signal(SIGCHLD, SIG_IGN);
    if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("socket");
        return (-1);
    }
    val = 1;
    if (setsockopt(fd,
                   SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0)
    {
        perror("setsockopt");
        return (-1);
    }
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(argv[1]));
    if (argc >= 3)
    {
        if (realpath(argv[2], base_dir) == NULL)
        {
            perror("docroot error");
            return (-1);
        };
    }
    if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("bind");
        return (-1);
    }
    if (listen(fd, SOMAXCONN) < 0)
    {
        perror("listen");
        return (-1);
    }

    while (1)
    {
        val = sizeof(psin);
        bzero(&psin, sizeof(psin));
        if ((pfd = accept(fd, (struct sockaddr *)&psin, &val)) < 0)
        {
            perror("accept");
            return (-1);
        }
        if ((pid = fork()) < 0)
        {
            perror("fork");
            return (-1);
        }
        else if (pid == 0)
        {
            /* child */
            close(fd); /* because it has been connected */
            serv_client(pfd, &psin);
            exit(0);
        }
        /* parent */
        close(pfd);
    }
}

void serv_client(int fd, struct sockaddr_in *sin)
{
    int len;
    char buf[2048];
    printf("connected from %s:%d\n",
           inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));
    sprintf(addr_str, "%s", inet_ntoa(sin->sin_addr));
    sprintf(port_str, "%d", ntohs(sin->sin_port));
    while ((len = recv(fd, buf, sizeof(buf), 0)) > 0)
    {
        *(buf + len) = '\0';
        request_handler(buf, fd);
        close(fd);
        return;
    }
    printf("disconnected from %s:%d\n",
           inet_ntoa(sin->sin_addr), ntohs(sin->sin_port));
    return;
}

FILE *open_target_file(char *filename)
{
    FILE *fp = fopen(filename, "r");
    return fp;
}

int file_exist(char *filename)
{
    if (access(filename, F_OK) != -1)
    {
        /* file exists */
        return 1;
    }
    else
    {
        return 0;
    }
}

void request_handler(char *buf, int fd)
{
    char filename[400];
    char header[200];
    is_dynamic = 0;
    FILE *fp;
    size_t read_size;
    struct stat path_stat;

    if (parse_request(buf, filename) != 0)
    {
        perror("Error occurs while parse request");
        return;
    }
    /* if file exists */
    if (stat(filename, &path_stat) == 0)
    {
        if (S_ISDIR(path_stat.st_mode))
        {
            /* if no slash in the end, return 301*/
            if (uri[strlen(uri) - 1] != '/' && strlen(uri) > 1)
            {
                http_301(fd);
                return;
            }
            char index_file_name[2][420];
            strcpy(index_file_name[0], filename);
            realpath(strcat(index_file_name[0], INDEX), index_file_name[1]);
            if (file_exist(index_file_name[1]))
            { /* if index.html exist*/
                strcpy(filename, index_file_name[1]);
                goto R_FILE;
            }
            else if (dir_no_slash)
            {
                http_403(fd);
            }
            else
            {
                strcpy(ext, ".html");
                encapsulate_http(fd, 200);
                read_dir(fd, filename);
            }
        }
        /* if it is regular file */
        else if (S_ISREG(path_stat.st_mode))
        {
        R_FILE:
            fp = open_target_file(filename);
            /* if permission denied */
            if (fp == NULL)
            {
                http_403(fd);
                return;
            }
            /* success */
            else
            {
                /* if it's cgi program */
                if (strstr(filename, "cgi-bin/") != 0 ||
                    strstr(filename + strlen(filename) - 3, ".cgi") != 0)
                {
                    is_dynamic = 1;
                    char buffer[MAXSIZE * MAXSIZE], newbuf[MAXSIZE * MAXSIZE + 20];
                    execute_cgi(filename, buffer);
                    // encapsulate_http(fd, 200);
                    strcpy(newbuf, "HTTP/1.1 200 OK\r\nServer: UnixProgramming\r\n");
                    if (*buffer == '\n')
                        strcat(newbuf, buffer + 1);
                    else
                        strcat(newbuf, buffer);
                    send_with_check(fd, newbuf, strlen(newbuf), 0);
                }
                else /* normal file */
                {
                    get_ext(filename);
                    encapsulate_http(fd, 200);
                    read_target_file(fp, fd);
                }
            }
        }
        else
        {
            http_403(fd);
            return;
        }
    }
    else
    { /* not exist */
        http_403(fd);
    }
}

void read_dir(int fd, char *path)
{
    char content[1000] = "<html><body>";
    DIR *dir_ptr;
    dir_ptr = opendir(path);
    struct dirent *direntp;
    if (dir_ptr == NULL)
    {
        closedir(dir_ptr);
        free(path);
        return;
    }
    while ((direntp = readdir(dir_ptr)) != NULL)
    {
        hyper_reference(direntp->d_name, content);
    }
    send_with_check(fd, content, strlen(content), 0);
}

void read_target_file(FILE *fp, int fd)
{
    char content[1400];
    int size = 0;
    while (!feof(fp))
    {
        size = fread(content, 1, sizeof(content), fp);
        send_with_check(fd, content, size, 0);
    }
}

void execute_cgi(char *filename, char *buffer)
{
    pid_t pid;
    int fd[2], n, status;
    char content_length[20];
    char content_type[100];
    if (pipe(fd) < 0)
        perror("pipe error");

    if ((pid = fork()) < 0)
    {
        perror("fork error");
    }
    else if (pid == 0) /* child */
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        setenv("REQUEST_METHOD", method, 1);
        setenv("QUERY_STRING", query_string, 1);
        setenv("REQUEST_URI", uri, 1);
        setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
        setenv("REMOTE_ADDR", addr_str, 1);
        setenv("REMOTE_PORT", port_str, 1);
        setenv("SCRIPT_NAME", filename, 1);
        setenv("PATH", "/bin:/usr/bin:/usr/local/bin", 1);
        set_header_env();
        if (strcmp(method, "POST") == 0)
        {
            search_http_header("Content-Length", content_length);
            setenv("CONTENT_LENGTH", content_length, 1);
            search_http_header("Content-Type", content_type);
            setenv("CONTENT_TYPE", content_type, 1);
        }
        if (execve(filename, environ, environ) < 0)
        {
            perror("execve");
        }
    }
    else
    {
        wait(&status);
        close(fd[1]);
        n = read(fd[0], buffer, MAXSIZE * MAXSIZE);
    }
}

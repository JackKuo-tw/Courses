#include "http.h"

char base_dir[100] = "./";
const char INDEX[30] = "/index.html";
bool dir_no_slash = 0;
char method[5], uri[MAXSIZE], version[MAXSIZE], ext[10];
char query_string[2000];
struct http_header
{
    char name[MAXSIZE];
    char value[MAXSIZE];
    struct http_header *next;
} * header;

void encapsulate_http(int fd, int http_status)
{
    char header[MAXSIZE] = "";
    char status[MAXSIZE] = "HTTP/1.1 ";
    switch (http_status)
    {
    case 200:
        strcat(status, "200 OK");
        if (is_dynamic == 0)
        {
            char content_type[MAXSIZE] = "Content-Type: ";
            get_mime_type(ext, content_type);
            sprintf(header, "%s%s\r\n", header, content_type);
        }
        break;
    case 404:
        strcat(status, "404 Not Found");
        break;
    case 403:
        strcat(status, "403 Forbidden");
        break;
    case 301:
        strcat(status, "301 MOVE PERMANENTLY");
        char redir[MAXSIZE];
        sprintf(redir, "Location: %s/", uri);
        sprintf(header, "%s%s\r\n", header, redir);
        break;
    }
    char server[30] = "Server: UnixProgramming";

    sprintf(header, "%s%s\r\n", header, server);

    strcat(header, "\r\n");
    sprintf(status, "%s\r\n%s", status, header);
    send_with_check(fd, status, strlen(status), 0);
}

int parse_request(char *buf, char *filename)
{
    char *pch = NULL;
    char real_path[600];
    strcpy(real_path, base_dir);
    pch = strtok(buf, "\r\n");
    sscanf(pch, "%s %s %s", method, uri, version);
    strcpy(filename, uri);
    find_question_mark(filename);
    strcat(real_path, filename);
    strcpy(filename, real_path);
    /* parse the other http header */
    parse_http_header(pch);
    return 0;
}

void send_with_check(int fd, const void *buf, size_t n, int flags)
{
    if (send(fd, buf, n, flags) < 0)
    {
        perror("send");
        exit(-1);
    }
}

void find_question_mark(char *filename)
{
    char *s;
    dir_no_slash = 0;
    s = strstr(filename, "?");
    if (s != NULL)
    {
        strcpy(query_string, s + 1);
        *s = '\0';
        if (*(s - 1) != '/')
        {
            dir_no_slash = 1;
        }
    }
}

void http_404(int fd)
{
    char *content = "<h1>404 Not Found!</h1>";
    encapsulate_http(fd, 404);
    send_with_check(fd, content, strlen(content), 0);
}

void http_403(int fd)
{
    char *content = "<h1>403 Forbidden!</h1>";
    encapsulate_http(fd, 403);
    send_with_check(fd, content, strlen(content), 0);
}

void http_301(int fd)
{
    char *content = "<h1>301 MOVE PERMANENTLY</h1>";
    encapsulate_http(fd, 301);
    send_with_check(fd, content, strlen(content), 0);
}

void parse_http_header(char *pch)
{
    pch = strtok(NULL, "\r\n");
    struct http_header *last = NULL;

    char name[MAXSIZE], value[MAXSIZE];
    int i = 0;
    bool ready_post_data = 0;
    while (1)
    {
        if (strstr(pch, ": ") == NULL)
        {
            ready_post_data = 1;
            if (strcmp(method, "POST") != 0)
                break;
            strcpy(query_string, pch);
            break;
        }
        sscanf(pch, "%[^:]: %s", name, value);
        struct http_header *tmp = malloc(sizeof(struct http_header));
        strcpy(tmp->name, name);
        strcpy(tmp->value, value);
        if (last == NULL)
        {
            header = tmp;
            last = tmp;
        }
        else
        {
            last->next = tmp;
            last = tmp;
        }
        pch = strtok(NULL, "\r\n");
        if (pch == NULL)
            break;
    }
}

void search_http_header(char *target, char *result)
{
    struct http_header *last = header;
    while (last != NULL)
    {
        if (strcasecmp(last->name, target) == 0)
        {
            strcpy(result, last->value);
            return;
        }
        last = last->next;
    }
}

void hyper_reference(char *target, char *dest)
{
    sprintf(dest, "%s<a href=\"%s%s\">%s</a><br>", dest, uri, target, target);
}

void get_mime_type(char *target, char *dest)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char file_ext[100], file_type[100];
    FILE *fp = fopen("./mime.types", "r");
    if (fp == NULL)
        return;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (read <= 0 || line[0] == '#')
            continue;
        sscanf(line, "%s %s", file_ext, file_type);
        if (strcmp(file_ext, target) == 0)
        {
            strcat(dest, file_type);
            if (strcmp(file_type, "text/html") == 0)
                strcat(dest, "; charset=utf-8");
            return;
        }
    }
    /* default binary */
    strcat(dest, "application/octet-stream");
}

void get_ext(char *filename)
{
    char *s;
    s = (char *)rindex(filename, '.');
    if (s != NULL)
    {
        strcpy(ext, s);
    }
}

void set_header_env()
{
    struct http_header *last = header;
    while (last != NULL)
    {
        if (strcmp(last->name, "CONTENT-TYPE")==0 || strcmp(last->name, "CONTENT_LENGTH")==0)
            continue;
        char *dash;
        char newname[MAXSIZE + 10];
        sprintf(newname, "HTTP_%s", last->name);
        while ((dash = strstr(newname, "-")) > 0)
        {
            *dash = '_';
        }
        setenv(newname, last->value, 1);
        last = last->next;
    }
}

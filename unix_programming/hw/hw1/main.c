#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <getopt.h>


#define IPv4_PRINT_LEN 22
#define IPv6_PRINT_LEN 37
#define CMD_SIZE 300

struct Socket_Id;
struct Proc_Info;
char *get_cmd(char *);
void traverse_proc(struct Proc_Info *);
void parseV4(char proto[4]);
void parseV6(char proto[3]);
char *sid2pid_pname(char sid[8]);
char *convert_hex_ip_to_present(char *, int);
struct Proc_Info *traverse_proc_dir(char *dirname);
void print_row(char *proto, 
               char *local_addr, char *local_port, 
               char *remote_addr, char *remote_port, char *sid);


struct Proc_Info *first;
char *filter = NULL;

int main(int argc, char *argv[]) {
    first = traverse_proc_dir("/proc");
    const char *optstring = "tuh";
    int c;
    int record = 0;
    struct option opts[] = {
        {"tcp", 0, NULL, 't'},
        {"udp", 0, NULL, 'u'},
        {"help", 0, NULL, 'h'}
    };
    while((c = getopt_long(argc, argv, optstring, opts, NULL)) != -1) {
        switch(c) {
            case 't':
                record += 1;
                break;
            case 'u':
                record += 2;
                break;
            case 'h':
                printf("usage: [-t|--tcp] [-u|--udp] [filter-string]\n");
                break;
            case '?':
                printf("usage: [-t|--tcp] [-u|--udp] [filter-string]\n");
                return 0;
        }
    }

    if (argc > optind) {
        int i = optind;
        filter = argv[i];
    }

    switch(record) {
        case 0:
            parseV4("tcp");
            parseV6("tcp");
            printf("\n");
            parseV4("udp");
            parseV6("udp");
            break;
        case 1:
            parseV4("tcp");
            parseV6("tcp");
            break;
        case 2:
            parseV4("udp");
            parseV6("udp");
            break;
        case 3:
            parseV4("tcp");
            parseV6("tcp");
            printf("\n");
            parseV4("udp");
            parseV6("udp");
            break;
    }
}

struct Socket_Id {
    struct Socket_Id *next;
    char id[8];
};

struct Proc_Info {
    struct Proc_Info *next;
    char d_name[10]; // the dir name (PID), number
    char cmd[CMD_SIZE]; // what command it executes
    struct Socket_Id *SocketId;
};

char *append_char(char *a, char *b) {
    if (b != NULL){
            char *final = malloc(strlen(a) + strlen(b) + 2);
            strcpy(final, a);
            strcat(final, " ");
            strcat(final, b);
            return final;
        }
    return a;
}

char *get_cmd(char *filename) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    if ((read = getline(&line, &len, fp)) != -1){
        char *r = strrchr(line, '/');
        char *nu = malloc(CMD_SIZE*2);
        strcpy(nu, line);
        for (int i = 0; i < read-3; i++) {
            if(*(line+i) == '\0'){
                strcat(nu, " ");
                if (strlen(line+i+1)+strlen(nu) < (300))
                strcat(nu, line+i+1);
            }
        }
        char *nnu = malloc(read*2);
        if (r != NULL){
            strcpy(nnu, nu+(r-line) + 1);
            return nnu;
        }
        return nu;
    }
    return "";
}

void traverse_proc(struct Proc_Info *p) {
    DIR *dir_ptr;
    struct dirent *direntp;
    // /proc/{pid}/fd + null-terminator
    char *path = malloc(9 + strlen(p->d_name) + 1);
    // assemble the path
    strcpy(path, "/proc/");
    strcat(path, p->d_name);
    strcat(path, "/fd");

    dir_ptr = opendir(path);
    if (dir_ptr == NULL) {
        closedir(dir_ptr);
        free(path);
        return;
    }
    while ((direntp = readdir(dir_ptr)) != NULL) {
        if ((direntp->d_name[0]) == '.' )
            continue;
        struct stat buf;
        char *fd_file = malloc(strlen(path) + strlen(direntp->d_name) + 1);
        strcpy(fd_file, path);
        strcat(fd_file, "/");
        strcat(fd_file, direntp->d_name);
        if (stat(fd_file, &buf) != -1) {
            // if it's a socket file
            if (S_ISSOCK(buf.st_mode) ){
                char target_path[256];
                memset(target_path, 0, sizeof(target_path));
                if (readlink(fd_file, target_path, sizeof(target_path)) != -1){
                    char socket_id_str[8];
                    int pos = 0;
                    for (int i=0; i<strlen(target_path); i++){
                        if (isdigit(target_path[i])){
                            socket_id_str[pos++] = target_path[i];
                        }
                    }
                    socket_id_str[pos] = '\0';
                    struct Socket_Id *tmp = NULL;
                    tmp = malloc( sizeof(struct Socket_Id) );
                    tmp->next = NULL;
                    strcpy(tmp->id, socket_id_str);
                    if (p->SocketId == NULL)
                        p->SocketId = tmp;
                    else {
                        struct Socket_Id *last_socket_id = p->SocketId;
                        while (1){
                            if (last_socket_id->next == NULL){
                                last_socket_id->next = tmp;
                                break;
                            } else {
                                last_socket_id = last_socket_id->next;
                            }
                        }
                    } // end else: SocketId == NULL
                } // end if readlink != -1
            } // end if S_ISSOCK()
        } // end if stat() != -1
        free(fd_file);
    }
    closedir(dir_ptr);
    free(path);
}

char *sid2pid_pname(char sid[8]) {
    struct Proc_Info *tmp_first = first;
    while(tmp_first) {
        struct Socket_Id *s = tmp_first->SocketId;
        while(s !=NULL){
            if (strcmp(s->id, sid) == 0) {
                int length = strlen(tmp_first->d_name) + strlen(tmp_first->cmd) +2;
                char *pid_pname = malloc(length);
                strcpy(pid_pname, tmp_first->d_name);
                strcat(pid_pname, "/");
                strcat(pid_pname, tmp_first->cmd);
                return pid_pname;
            }
            s = s->next;
        }
        tmp_first = tmp_first->next;
    }
    return "-";
}

char *convert_hex_ip_to_present(char *hex_ip, int buf_len) {
    char *present_ip = malloc(buf_len);
    // if IPv4
    if (buf_len == INET_ADDRSTRLEN ){
        struct in_addr s;
        size_t t;
        t = strtol(hex_ip, NULL, 16);
        s.s_addr = t;
        inet_ntop(AF_INET, &s, present_ip, INET_ADDRSTRLEN);
    // if IPv6
    } else if (buf_len == INET6_ADDRSTRLEN){
        int in6_value[16];
        for(int i=0; i<16; i++){
            char *f = malloc(3);
            strncpy(f, hex_ip+i*2,2);
            in6_value[i] = strtol(f, NULL, 16);
        }
        struct in6_addr s = {in6_value[3], in6_value[2], in6_value[1], in6_value[0],
                             in6_value[7], in6_value[6], in6_value[5], in6_value[4],
                             in6_value[11], in6_value[10], in6_value[9], in6_value[8],
                             in6_value[15], in6_value[14], in6_value[13], in6_value[12]};
        inet_ntop(AF_INET6, &s, present_ip, INET6_ADDRSTRLEN);
    }
    return present_ip;
}

void print_row(char proto[3], 
               char *local_addr, char *local_port, 
               char *remote_addr, char *remote_port, char *sid) {
    char tmp[8];
    char *local, *remote;
    int ipLen;
    if (strlen(proto) == 3) {
        local = malloc(IPv4_PRINT_LEN);
        remote = malloc(IPv4_PRINT_LEN);
        ipLen = INET_ADDRSTRLEN;
    } else if (strlen(proto) == 4) {
        local = malloc(IPv6_PRINT_LEN);
        remote = malloc(IPv6_PRINT_LEN);
        ipLen = INET6_ADDRSTRLEN;
    }
    strcpy(local, convert_hex_ip_to_present(local_addr, ipLen));
    strcat(local, ":");
    sprintf(tmp, "%lu", strtoul(local_port, NULL, 16));
    if(strcmp(tmp, "0")==0){
        strcpy(tmp, "*");
    }
    strcat(local, tmp);
    
    strcpy(remote, convert_hex_ip_to_present(remote_addr, ipLen));
    strcat(remote, ":");
    sprintf(tmp, "%lu", strtoul(remote_port, NULL, 16));
    if(strcmp(tmp, "0")==0){
        strcpy(tmp, "*");
    }
    strcat(remote, tmp);
    // filter string
    char ss[500];
    sprintf(ss, "%-5s %-38s       %-30s         %s\n",
            proto,
            local,
            remote,
            sid2pid_pname(sid)
            );
    if (filter != NULL){
        if(strstr(ss, filter) != NULL)
            printf("%s", ss);
    } else {
        printf("%s", ss);
    }
    free(local); free(remote);
}

struct Proc_Info *traverse_proc_dir(char dirname[]) {
    DIR *dir_ptr;
    struct dirent *direntp;
    struct Proc_Info *first = NULL;
    first = malloc( sizeof(struct Proc_Info) );
    first->next = NULL;
    first->SocketId = NULL;
    struct Proc_Info *prev;
    int total_process = 0;

    dir_ptr = opendir(dirname);
    if (dir_ptr == NULL) {
	    fprintf(stderr, "Can not open %s\n", dirname);
    } else {

	while ((direntp = readdir(dir_ptr)) != NULL) {
        bool found = false;
        // make sure the dirname is number, which means it contains the process information.
        for (int i=0; i< strlen(direntp->d_name); i++) {
            if (!(isdigit(direntp->d_name[i])))
                found = true;
                break;
        }
        if (!found) {
            // the first linked list item
            if (total_process++ == 0){
                strcpy(first->d_name,direntp->d_name);
                prev = first;
            // not the first item of linked list
            } else {
                struct Proc_Info *tmp = NULL;
                tmp = malloc( sizeof(struct Proc_Info) );
                strcpy(tmp->d_name,direntp->d_name);
                tmp->next = NULL;
                tmp->SocketId = NULL;
                prev->next = tmp;
                prev = tmp;
            }
            char *status_location = malloc(strlen("/proc/") + strlen(prev->d_name) + strlen("/status"));
            strcpy(status_location, "/proc/");
            strcat(status_location, prev->d_name);
            strcat(status_location, "/cmdline");
            strcpy(prev->cmd, get_cmd(status_location));
            free(status_location);
            traverse_proc(prev);
        }
	}
	closedir(dir_ptr);
    }
    return first;
}

void parseV6(char proto[3]) {
    FILE *fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char *t = malloc(5);
    if (strcmp(proto, "tcp") == 0) {
        fp = fopen("/proc/net/tcp6", "r");
        t = "tcp6";
    } else if (strcmp(proto, "udp") == 0) {
        fp = fopen("/proc/net/udp6", "r");
        t = "udp6";
    }
    
    if (fp == NULL)
        exit(EXIT_FAILURE);
    read = getline(&line, &len, fp); // ignore the first line
    while ((read = getline(&line, &len, fp)) != -1) {
        char *pch;
        pch = strtok(line, " ");
        int pos = 0;
        char local_addr[33];
        char local_port[6];
        char remote_addr[33];
        char remote_port[6];
        char socket_id[9];
        while(pch != NULL) {
            switch(pos++) {
                case 1:
                    strncpy(local_addr, pch, 32);
                    local_addr[32] = '\0';
                    strncpy(local_port, pch+33, 5);
                    local_port[5] = '\0';
                    break;
                case 2:
                    strncpy(remote_addr, pch, 32);
                    remote_addr[32] = '\0';
                    strncpy(remote_port, pch+33, 5);
                    remote_port[5] = '\0';
                    break;
                case 9:
                    strcpy(socket_id, pch);
                    socket_id[8] = '\0';
                    break;
            }
            pch = strtok(NULL, " ");
        }
        print_row(t, local_addr, local_port, remote_addr, remote_port, socket_id);
    }
    fclose(fp);
    if (line)
        free(line);
}

void parseV4(char proto[4]) {
    FILE *fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    if (strcmp(proto, "tcp") == 0){
        printf("List of TCP connections:\n");
        fp = fopen("/proc/net/tcp", "r");
    } else if ((strcmp(proto, "udp") == 0)) {
        printf("List of UDP connections:\n");
        fp = fopen("/proc/net/udp", "r");
    }
    
    printf("Proto Local Address                                Foreign Address                        PID/Program name and arguments\n");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    read = getline(&line, &len, fp); // ignore the first line
    while ((read = getline(&line, &len, fp)) != -1) {
        char *pch;
        pch = strtok(line, " ");
        int pos = 0;
        char local_addr[9];
        char local_port[6];
        char remote_addr[9];
        char remote_port[6];
        char socket_id[8];
        struct in_addr addrptr;
        while(pch != NULL) {
            switch(pos++) {
                case 1:
                    strncpy(local_addr, pch, 8);
                    local_addr[8] = '\0';
                    strncpy(local_port, pch+9, 5);
                    local_port[5] = '\0';
                    break;
                case 2:
                    strncpy(remote_addr, pch, 8);
                    remote_addr[8] = '\0';
                    strncpy(remote_port, pch+9, 5);
                    remote_port[5] = '\0';
                    break;
                case 9:
                    strcpy(socket_id, pch);
                    break;
            }
            pch = strtok(NULL, " ");
        }
        print_row(proto, local_addr, local_port, remote_addr, remote_port, socket_id);
    }
    fclose(fp);
    if (line)
        free(line);
}

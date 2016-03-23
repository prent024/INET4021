#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/uio.h>

#define SIZE 0
#define TYPE 1

// Global variables set in get_conf().
char root[1024], port_buf[10], cons_buf[10], indx[512], access_log[1024], error_log[1024];
int port=8888, cons=3;      //Gave these default values in case config file is removed
FILE *errorf, *accessf;


// Read in the config file
void read_conf() {
    FILE *conf_file;
    char conf[7], cwd[1024];
    if((getcwd(cwd, sizeof(cwd))) == NULL) {
        perror("Couldn't get working directory: ");
        exit(1);
    }
    if(cwd[strlen(cwd)-3] == 's' && cwd[strlen(cwd)-2] == 'r' && cwd[strlen(cwd)-1] == 'c') {
        // If building manually in src/ the relative path is ../conf/httpd.conf
        conf_file=fopen("../conf/httpd.conf", "r");
    } else {
        // If building automatically from Makefile the relative path is conf/httpd.conf
        conf_file=fopen("conf/httpd.conf", "r");
    }

    while (fgets(conf , 6, conf_file)) {
        if (strcmp(conf, "ROOT=") == 0){
            fgets (root, 1024, conf_file);
            strtok(root, "\n");
        }
        if (strcmp(conf, "PORT=") == 0){
            fgets (port_buf, 10, conf_file);
            port=atoi(port_buf);
        }
        if (strcmp(conf, "CONS=") == 0){
            fgets (cons_buf, 10, conf_file);
            cons=atoi(cons_buf);
        }
        if (strcmp(conf, "INDX=") == 0){
            fgets (indx, 512, conf_file);
            strtok(indx, "\n");
        }
    }
    strncpy(access_log, root, 1023);
    strncpy(error_log,  root, 1023);
    strcat(access_log, "/logs/access.log");
    strcat(error_log,  "/logs/error.log");
    fclose (conf_file);
}

// Creates and binds the server socket
int create_and_bind() {
    int s, b, yes;
    struct sockaddr_in sin;
    struct hostent *server;
    char conn_addr[INET6_ADDRSTRLEN];

    bzero((char *) &sin, sizeof(sin));

    sin.sin_addr.s_addr=INADDR_ANY;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);           // use the port specified by config file

    if((s=socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        if((errorf=fopen(error_log, "a")) == NULL) {
            perror("Could not create socket because: ");
        } else {
            fprintf(errorf, "Could not create socket.\n");
            fclose(errorf);
        }
        exit(2);
    }

    yes = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt error: ");
    }

    if((b=bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
        if((errorf=fopen(error_log, "a")) == NULL) {
            perror("Could not bind to socket because: ");
        } else {
            fprintf(errorf, "Could not bind to socket.\n");
            fclose(errorf);
        }
        exit(3);
    }
    return s;
}

char* get_request_file(char* req) {
    char* path=(char*)malloc(1024*sizeof(char));
    char* trash;
    const char s[]=" ";
    char *token;

    token=strtok(req, s);       // Should be GET or POST
    printf("ACCESS TYPE: %s\n", token);
    token=strtok(NULL, s);      // Should be the path we are looking for
    printf("TOKEN: %s\n", token);

    strncpy(path, root, 1023);
    if(strcmp(token, "/") == 0) {
        strncat(path, indx, 1023);
    } else {
        strncat(path, token, 1023);
    }

    printf("PATH: %s\n", path);
    return path;
}

char** get_file_attr(int fd, char* file) {
    struct stat st;
    char** attr=(char**)malloc(sizeof(char*)*2);
    char* size=(char*)malloc(sizeof(char)*10);
    char* type=(char*)malloc(sizeof(char)*10);
    char* ext;

    if(fstat(fd, &st) == -1) {
        if((errorf=fopen(error_log, "a")) == NULL) {
            perror("Couldn't get file size because: ");
        } else {
            fprintf(errorf, "Couldn't get file size.\n");
            fclose(errorf);
        }
        exit(7);
    }
    sprintf(size, "%jd", st.st_size);

    ext=strrchr(file, '.');
    printf("EXT: %s\n", ext);
    if(strcmp(ext, ".ico") == 0) type="image/ico";
    else if(strcmp(ext, ".jpg") == 0) type="image/jpg";
    else if(strcmp(ext, ".png") == 0) type="image/png";
    else type="text/html";

    attr[SIZE]=size;
    attr[TYPE]=type;
    return attr;
}

// Send HTTP headers to the given socket based on the type of file & file size
int send_headers(int sock, char** attr) {
    char headers[80];
    sprintf(headers, "HTTP/1.1 200 OK\nContent-length: %s\nContent-Type: %s\n\n", attr[SIZE], attr[TYPE]);

    if(send(sock, headers, strlen(headers), 0) < 0) {
        if((errorf=fopen(error_log, "a")) == NULL) {
            perror("Couldn't send headers because: ");
        } else {
            fprintf(errorf, "Couldn't send headers.\n");
            fclose(errorf);
        }
        exit(8);
    }
}

// Actions a child process runs for a particular connection
void handle_connection(int sock) {
    ssize_t ssize, bsent;
    char req[512], cgi_file[512], sock_str[50];
    char **attr, *argv[4];
    char* file;
    char *ext, *trash; 
    int fd, chars;

    if((ssize=recv(sock, &req, sizeof(req), 0)) < 0) {
        if((errorf=fopen(error_log, "a")) == NULL) {
            perror("Did not receive anything because: ");
        } else {
            fprintf(errorf, "Did not receive anything.\n");
            fclose(errorf);
        }
        exit(5);
    }
    printf("Got \"%s\" from socket\n", req);

    file=get_request_file(req);
    printf("Computed File: %s\n",file);

    ext=strrchr(file, '.');
    if(strstr(ext, ".cgi")) {
        strncpy(cgi_file, file, 512);
        trash=strrchr(cgi_file, '?');
        printf("TRASH: %s\n", trash);
        if(trash != NULL) {
            trash[0]='\0';
        } else {
            if((errorf=fopen(error_log, "a")) == NULL) {
                perror("Data wasn't passed with the GET request: ");
            } else {
                fprintf(errorf, "Data wasn't passed with the GET request.\n");
                fclose(errorf);
            }
            exit(8);
        }
        argv[0]=cgi_file;
        argv[1]=file;
        sprintf(sock_str, "%i", sock);
        argv[2]=sock_str;
        argv[3]=NULL;
         if(execvp(argv[0], argv) < 0) {
            if((errorf=fopen(error_log, "a")) == NULL) {
                perror("Exec failed: ");
            } else {
                fprintf(errorf, "Exec failed.\n");
                fclose(errorf);
            }
            exit(9);
        } else {
        }
    }

    if((fd=open(file, O_RDONLY)) < 0) {
        if((errorf=fopen(error_log, "a")) == NULL) {
            perror("Couldn't open file because: ");
        } else {
            fprintf(errorf, "Couldn't open file.\n");
            fclose(errorf);
        }
        exit(6);
    }
    attr=get_file_attr(fd,file);
    printf("file size: %s\n", attr[SIZE]);
    printf("file type: %s\n", attr[TYPE]);

    send_headers(sock, attr);
    if((bsent=sendfile(sock, fd, NULL, atoi(attr[SIZE]))) < 0) {
        if((errorf=fopen(error_log, "a")) == NULL) {
            perror("Couldn't send file because: ");
        } else {
            fprintf(errorf, "Couldn't send file.\n");
            fclose(errorf);
        }
        exit(7);
    }
    printf("Bytes sent: %i\n", (int)bsent);

    // free(file);
    // free(attr[SIZE]);
    // free(attr[TYPE]);
    // free(attr);
    close(fd);
    close(sock);
}

// Listens and accepts new connections, forks off a child to handle each connection
void listen_and_accept(int s) {
    int addrlen, new_sock;
    struct sockaddr_in address;

    listen(s, cons);                    // use the connection number specified by config file
    addrlen=sizeof(struct sockaddr_in);

    while(1) {
        if((new_sock=accept(s, (struct sockaddr *)&address, &addrlen)) < 0) {
            if((errorf=fopen(error_log, "a")) == NULL) {
                perror("Couldn't accept connection: ");
            } else {
                fprintf(errorf, "Couldn't accept connection.\n");
                fclose(errorf);
            }
            exit(4);
        }

        if(!fork()) {
            close(s);
            handle_connection(new_sock);
            exit(0);
        }

        close(new_sock);
    }
}

int main(void) {
    read_conf();
    int s=create_and_bind();
    listen_and_accept(s);
}

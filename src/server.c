#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

// Global variables set in get_conf().
char root[500], port_buf[10], cons_buf[10], indx[500];
int port=8888, cons=3;      //Gave these default values in case config file is removed

// Read in the config file
void read_conf() {
    FILE *conf_file;
    char conf[7], cwd[1024];
    if(getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Couldn't get working directory: ");
        exit(1);
    }
    printf("Working Directory: %s\n",cwd);
    if(cwd[strlen(cwd)-3] == 's' && cwd[strlen(cwd)-2] == 'r' && cwd[strlen(cwd)-1] == 'c') {
        // If building manually in src/ the relative path is ../conf/httpd.conf
        conf_file=fopen("../conf/httpd.conf", "r");
    } else {
        // If building automatically from Makefile the relative path is conf/httpd.conf
        conf_file=fopen("conf/httpd.conf", "r");
    }

    while (fgets (conf , 6, conf_file)) {
    if (strcmp (conf, "ROOT=") == 0){
        fgets (root, 500, conf_file);
        strtok(root, "\n");
    }
    if (strcmp (conf, "PORT=") == 0){
        fgets (port_buf, 10, conf_file);
        port=atoi(port_buf);
    }
    if (strcmp (conf, "CONS=") == 0){
        fgets (cons_buf, 10, conf_file);
        cons=atoi(cons_buf);
    }
    if (strcmp (conf, "INDX=") == 0){
        fgets (indx, 500, conf_file);
        strtok(indx, "\n");
    }
    } 
    fclose (conf_file);
}

// Creates and binds the server socket
int create_and_bind() {
    int s, b;
    struct sockaddr_in sin;
    struct hostent *server;
    char conn_addr[INET6_ADDRSTRLEN];

    bzero((char *) &sin, sizeof(sin));

    sin.sin_addr.s_addr=INADDR_ANY;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);           // use the port specified by config file

    s=socket(PF_INET, SOCK_STREAM, 0);
    if(s<0) {
        perror("Could not create socket because: ");
        exit(2);
    }

    b=bind(s, (struct sockaddr *)&sin, sizeof(sin));
    if(b<0) {
        perror("Could not bind to socket because: ");
        exit(3);
    }
    return s;
}

// Send HTTP headers to the given socket based on the type of file & file size
int send_headers(int sock, char* type, off_t size) {
    char head[]="HTTP/1.1 200 OK\n";
    char mid[32];
    sprintf(mid, "Content-length: %jd\n", size);
    char tail[32];
    sprintf(tail, "Content-Type: %s\n\n", type);
    
    send(sock, head, strlen(head), 0);
    send(sock, mid, strlen(mid), 0);
    send(sock, tail, strlen(tail), 0);
}

// Actions a child process runs for a particular connection
void handle_conn(int sock) {
    struct stat st;
    ssize_t ssize;
    char buf[512], file[512];
    int fd;

    ssize=recv(sock, &buf, sizeof(buf), 0);
    if(ssize<0) {
        perror("Did not receive anything because: ");
        exit(5);
    }
    fprintf(stderr, "Got \"%s\" from socket\n", buf);
    fprintf(stderr, "String length = %d\n", (int)strlen(buf));

    strncpy(file, root, sizeof(file)-1);
    strncat(file, indx, sizeof(file)-1);

    fd=open(file,O_RDONLY);
    if(fd < 0) {
        perror("Couldn't open file because: ");
        exit(6);
    }
    if(fstat(fd, &st) == -1) {
        perror("Couldn't get file size because: ");
        exit(7);
    }
    fprintf(stderr, "file size: %jd\n", st.st_size);

    send_headers(sock, "text/html", st.st_size);
    sendfile(sock, fd, NULL, 512);      // Using 512 cause I'm not sure how big the bufsize should be

    close(sock);
}

// Listens and accepts new connections, forks off a child to handle each connection
void listen_and_accept(int s) {
    int addrlen, new_sock;
    struct sockaddr_in address;

    listen(s, cons);                    // use the connection number specified by config file

    while(1) {
        addrlen=sizeof(struct sockaddr_in);
        new_sock=accept(s, (struct sockaddr *)&address, &addrlen);
        if(new_sock<0) {
            perror("Accepting connection");
            exit(4);
        }
        fprintf(stderr, "got a connection\n");

        if(!fork()) {
            close(s);
            handle_conn(new_sock);
            exit(0);
        }

        close(new_sock);
    }
}

int main(void) {
    read_conf();

    // printf("ROOT = %s\n", root);
    // printf("PORT = %i\n", port);
    // printf("CONS = %i\n", cons);
    // printf("INDX = %s\n", indx);

    int s=create_and_bind();
    listen_and_accept(s);
}
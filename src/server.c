#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

//Variables used in get_conf().
char conf[7], root[500], port_buf[10], cons_buf[10], indx[500];
int port=8888, cons=3;      //Gave these default values in case config file is removed

void allocate_mem() {
    // conf=(char *)malloc(sizeof(char)*5);
    // root=(char *)malloc(sizeof(char)*500);
    // port_buf=(char *)malloc(sizeof(char)*10);
    // cons_buf=(char *)malloc(sizeof(char)*10);
    // indx=(char *)malloc(sizeof(char)*500);
}

// Read in the config file
void get_conf() {
    FILE *conffile;
    conffile = fopen ("../conf/httpd.conf", "r");

    while (fgets (conf , 6, conffile)) {
    if (strcmp (conf, "ROOT=") == 0){
        fgets (root, 500, conffile);
        strtok(root, "\n");
    }
    if (strcmp (conf, "PORT=") == 0){
        fgets (port_buf, 10, conffile);
        port=atoi(port_buf);
    }
    if (strcmp (conf, "CONS=") == 0){
        fgets (cons_buf, 10, conffile);
        cons=atoi(cons_buf);
    }
    if (strcmp (conf, "INDX=") == 0){
        fgets (indx, 500, conffile);
        strtok(indx, "\n");
    }
    } 
    fclose (conffile);
}

void create_and_bind() {
    int s, b;
    struct sockaddr_in sin;
    struct hostent *server;
    int addrlen, new_sock;
    struct sockaddr_in address;
    char buf[512];
    ssize_t ssize;

    bzero((char *) &sin, sizeof(sin));

    sin.sin_addr.s_addr=INADDR_ANY;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(port);           // use the port specified by config file

    s=socket(PF_INET, SOCK_STREAM, 0);
    if(s<0) {
        perror("Could not create socket because: ");
        exit(1);
    }

    b=bind(s, (struct sockaddr *)&sin, sizeof(sin));
    if(b<0) {
        perror("Could not bind to socket because: ");
        exit(2);
    }

    listen(s, cons);                    // use the connection number specified by config file

    addrlen=sizeof(struct sockaddr_in);
    new_sock=accept(s, (struct sockaddr *)&address, &addrlen);
    if(new_sock<0) {
        perror("Accepting connection");
        exit(3);
    } else {
        fprintf(stderr, "got a connection\n");
    }

    ssize=recv(new_sock, &buf, sizeof(buf), 0);
    if(ssize<0) {
        perror("Did not receive anything because: ");
        exit(4);
    } else {
        fprintf(stderr, "Got \"%s\" from socket\n", buf);
        fprintf(stderr, "String length = %d\n", (int)strlen(buf));
    }
}

void free_mem() {
    // free(conf);
    // free(root);
    // free(port_buf);
    // free(cons_buf);
    // free(indx);
}

int main(void) {
    get_conf();

    printf("ROOT = %s\n", root);
    printf("PORT = %i\n", port);
    printf("CONS = %i\n", cons);
    printf("INDX = %s\n", indx);

    create_and_bind();
}
/*
 * client.c
 */
// Khushkumar Jajoo-32821756

#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

#define eos(s) ((s) + strlen(s)) 
int main(int argc, char **argv) 
{
    int clientfd;
    char *num1, *num2;
    char *host, *port;
    char *ptr;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <num1> <num2>\n", argv[0]);
        exit(0);
    }

    num1 = argv[1];
    num2 = argv[2];

    host = "localhost";
    port = "8080";

    clientfd = Open_clientfd(host, port);

    char buf[MAXLINE];
    rio_t rio;
    char buf1[MAXLINE];

    // XML body
    sprintf(buf, "<?xml version=\"1.0\"?>\r\n");
    sprintf(eos(buf), "<methodCall>\r\n");
    sprintf(eos(buf), "<methodName>sample.addmultiply</methodName>\r\n");
    sprintf(eos(buf), "<params>\r\n");
    sprintf(eos(buf), "<param><value><double>%s</double></value></param>\r\n", num1);
    sprintf(eos(buf), "<param><value><double>%s</double></value></param>\r\n", num2);
    sprintf(eos(buf), "</params>\r\n");
    sprintf(eos(buf), "</methodCall>\r\n");

    // XML header
    sprintf(buf1, "POST /RPC2 HTTP/1.1\r\n");
    sprintf(eos(buf1), "Content-Type:text/xml\r\n");
    sprintf(eos(buf1), "User-Agent:XML-RPC.NET\r\n");
    sprintf(eos(buf1), "Content-Length:%ld\r\n", strlen(buf));
    sprintf(eos(buf1), "Expect:100-continue\r\n");
    sprintf(eos(buf1), "Connection:Keep-Alive\r\n");
    sprintf(eos(buf1), "Host:localhost:8080\r\n");
    sprintf(eos(buf1), "\r\n");

    //append body to header
    sprintf(eos(buf1), buf); 
    
    Rio_writen(clientfd, buf1, sizeof(buf1));

    Rio_readinitb(&rio, clientfd);

    while(Rio_readlineb(&rio, buf, sizeof(buf1)) > 0)
    {
        if((ptr = strstr(buf, "double")) != NULL)
        {
            ptr = ptr + 7;
            while(*ptr != '<')
            {
                printf("%c", *ptr);
                ptr++;
            }
            printf("    ");
        }
    }
    printf("\n");
    
    Close(clientfd);
    exit(0);
}
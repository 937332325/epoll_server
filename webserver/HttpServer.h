//
// Created by 陈海斌 on 2019-07-11.
//

#ifndef WEBSERVER_HTTPSERVER_H
#define WEBSERVER_HTTPSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

using namespace std;

#define MAXEVENT 1024
#define MAX_DATA_SIZE 1024
class HttpServer
{
public:
    void start(int port);
    void handle(int clientfd);
private:
    struct sockaddr_in serverSocketaddr;
    struct sockaddr_in clientSocketaddr;
    int listen_fd;
    int epoll_fd;
    struct epoll_event eventList[MAXEVENT];
    int ret;
};
#endif //WEBSERVER_HTTPSERVER_H

//
// Created by 陈海斌 on 2019-07-12.
//
#include "HttpServer.h"

void HttpServer::start(int port) {
    if((listen_fd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "socket error\n" << endl;
        exit(-1);
    }
    bzero(&serverSocketaddr, sizeof(struct sockaddr_in));
    serverSocketaddr.sin_family = AF_INET;
    serverSocketaddr.sin_port = htons(port);
    serverSocketaddr.sin_addr.s_addr = htonl(0);

    if (::bind(listen_fd, (sockaddr*)&serverSocketaddr, sizeof(serverSocketaddr)) != 0)
    {
        cerr << "Error: failed to bind port (" << port << ")!" << endl;
        exit(-1);
    }

    if (listen(listen_fd, 5) < 0)
    {
        cerr << "Error: failed to listen!!!" << endl;
        exit(-1);
    }

    epoll_fd = epoll_create(MAXEVENT);
    struct epoll_event event;
    event.events = EPOLLIN|EPOLLET;
    event.data.fd = listen_fd;

    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_fd,&event)<0)
    {
        printf("epoll 加入失败 fd:%d\n",listen_fd);
        exit(-1);
    }
    //ret会返回在规定的时间内获取到IO数据的个数，并把获取到的event保存在eventList中，注意在每次执行该函数时eventList都会清空，由epoll_wait函数填写。
    //而不清除已经EPOLL_CTL_ADD到epollfd描述符的其他加入的文件描述符。这一点与select不同，select每次都要进行FD_SET，具体可看我的select讲解。
    //epoll里面的文件描述符要手动通过EPOLL_CTL_DEL进行删除。
    for (;;) {
        ret = epoll_wait(epoll_fd, eventList, MAXEVENT, -1);
        for (int n = 0; n < ret; ++n) {
            if (eventList[n].data.fd == listen_fd) {
                bzero(&clientSocketaddr, sizeof(struct sockaddr_in));
                socklen_t len = sizeof(clientSocketaddr);
                int connd = accept(listen_fd,(struct sockaddr *) &clientSocketaddr, &len);
                if (connd== -1) {
                    perror("accept");
                    exit(-1);
                }
                //设置不阻塞
                if ((fcntl(connd, F_SETFL, fcntl(connd, F_GETFD, 0) | O_NONBLOCK))>0)
                {
                    perror("set blocking fail");
                    exit(-1);
                }
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = connd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connd,&event) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
            } else {
                handle(eventList[n].data.fd);
            }
        }
    }
}

void HttpServer::handle(int clientfd)
{
     int recvLen=0;
     char recvBuf[MAX_DATA_SIZE];
     memset(recvBuf,0,sizeof(recvBuf));
     recvLen=recv(clientfd,(char *)recvBuf,MAX_DATA_SIZE,0);
     if(recvLen==0)
         return;
     else if(recvLen<0)
     {
         perror("recv Error");
       exit(-1);
     }
    //各种处理
    printf("接收到的数据:%s \n",recvBuf);
    return;
 }
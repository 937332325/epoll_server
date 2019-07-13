#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "HttpServer.h"

using namespace std;

int main(int argc, char *argv[])
{
    int port = 12345;

    HttpServer *server = new HttpServer();

    server->start(port);

    delete server;
    return 0;
}
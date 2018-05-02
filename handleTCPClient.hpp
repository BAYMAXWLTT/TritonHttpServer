/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

server:
implementation of server start up, creating thread pool.
*******************************************************************************************/
#ifndef HANDLETCPCLIENT_HPP
#define HANDLETCPCLIENT_HPP

#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <errno.h>
#include "httpd.h"
#include "framer.hpp"
#include "parser.hpp"
#include "responder.hpp"
#include "diewithmessage.hpp"

const unsigned BUFSIZE = 4096;
void HandleReq(int clntSock, std::string doc_root);
void *HandleTCPClient(void *args);

#endif // HTTPD_H

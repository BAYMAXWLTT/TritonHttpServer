/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

server:
implementation of server start up, creating thread pool.
*******************************************************************************************/
#ifndef HANDLETCPCLIENT_HPP
#define HANDLETCPCLIENT_HPP

const unsigned BUFSIZE = 4096;
void HandleReq();
void *HandleTCPClient(void *args);

#endif // HTTPD_H

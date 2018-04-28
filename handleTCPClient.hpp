/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

server:
implementation of server start up, creating thread pool.
*******************************************************************************************/
#ifndef HANDLETCPCLIENT_HPP
#define HANDLETCPCLIENT_HPP

struct ThreadArgs{
	int servSocket;
	string doc_root;
}

void *HandleTCPClient(void *args);

#endif // HTTPD_H

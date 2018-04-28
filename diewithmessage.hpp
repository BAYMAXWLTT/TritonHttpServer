/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

server:
implementation of server start up, creating thread pool.
*******************************************************************************************/
#ifndef DIEWITHMESSAGE_HPP
#define DIEWITHMESSAGE_HPP

#include <string>

using namespace std;

const unsigned CONNECTION_SIZE = 20;
const unsigned POOL_SIZE = 10;

void DiewithMessage(string msg);
void *ThreadsinPool(void *args);

#endif // HTTPD_H
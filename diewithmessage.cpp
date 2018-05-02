/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

httpd.cpp:
Concurrency:
implementation of server start up, creating thread pool.
*******************************************************************************************/
#include "diewithmessage.hpp"

void DiewithMessage(std::string msg){
  std::cerr << msg << '\n';
  exit(1);
}

void DiewithMessage_t(std::string msg){
  std::cerr << msg <<'\n';
  pthread_exit(1);
}

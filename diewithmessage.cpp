/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

httpd.cpp:
Concurrency:
implementation of server start up, creating thread pool.
*******************************************************************************************/

#include <string>
#include <iostream>
#include "diewithmessage.hpp"

void DiewithMessage(string msg){
  cerr << msg << '\n';
  exit(1);
}

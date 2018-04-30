#ifndef HTTPD_H
#define HTTPD_H

const unsigned CONNECTION_SIZE = 20;
const unsigned POOL_SIZE = 10;
const float HTTP_VER_UPPER = 1.1;

struct ThreadArgs{
	int servSocket;
	std::string doc_root;
};

void start_httpd(unsigned short port, std::string doc_root);

#endif // HTTPD_H

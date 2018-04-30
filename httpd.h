#ifndef HTTPD_H
#define HTTPD_H

struct ThreadArgs{
	int servSocket;
	string doc_root;
}

void start_httpd(unsigned short port, string doc_root);

#endif // HTTPD_H

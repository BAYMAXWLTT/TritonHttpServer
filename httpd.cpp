#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "httpd.h"
#include "server_utils.hpp"

using namespace std;

void start_httpd(unsigned short port, string doc_root)
{
	int servSocket; /*set server socket fd*/
	int clntSocket; /*set client socket fd*/
	const string root = doc_root; /*Set the document serving root*/
	sockaddr_in echoServAddr; /*Local IP address*/
	sockaddr_in echoClntAddr; /*Client IP address*/
	unsigned short serverPort = (port); /*set server port*/
	unsigned int clntLen; /*Length of client address*/

	cerr << "Starting server (port: " << port <<
		", doc_root: " << doc_root << ")" << endl;

	if((servSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		DiewithMessage("Called socket(): Socket create failed"); /*socket creation failed*/
	}

	/* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

	if(bind(servSocket, (sockaddr *)echoServAddr, sizeof(echoServAddr)) < 0){
		DiewithMessage("Called bind(): socket binding failed"); /*bind socket failed*/
	}

	if(listen(servSocket, CONNECTION_SIZE) < 0){ /*CONNECTION_SIZE can be found in server_utils*/
		DiewithMessage("Called listen(): listen failed"); /*listen on socket failed*/
	}

	while(true){
		/*Server loop to handle incoming request*/

	}



}

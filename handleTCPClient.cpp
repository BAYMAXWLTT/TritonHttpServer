#include <iostream>
#include <stdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "handleTCPClient.hpp"
#include "diewithmessage.hpp"

using namespace std;

void *HandleTCPClient(void *args){
      ThreadArgs arg = static_cast<ThreadArgs *>(args);
      int servSock = arg->servSocket;
      string doc_root = arg->doc_root;
      unsigned int clntLen; /*Length of client address*/
      int clntSock; /*set client socket fd*/
      sockaddr_in echoClntAddr; /*Client IP address*/

      while(true){
  		/* Set the size of the in-out parameter */
  		clntLen = sizeof(echoClntAddr);

  		/* Wait for a client to connect */
  		if ((clntSock = accept(servSock, (sockaddr *) &echoClntAddr, &clntLen)) < 0)
  			DiewithMessage("accept() failed");

  		/* clntSock is connected to a client! */

  		cerr << "Handling client " + inet_ntoa(echoClntAddr.sin_addr) << '\n';

      /* Start Request Handling Process*/
  		HandleReq(clntSock, doc_root);

      /* Produce Response*/
    }
}

void HandleReq(int clntSock, string doc_root){

}

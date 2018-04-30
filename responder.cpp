#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>
#include <sys/sendfile.h>
#include "responder.hpp"
#include "httpd.h"
using namespace std;

// handle reponse based on request data structure
/* P2: Implement basic URL check policy, avoid client from accessing unauthorized path */
/* Check host*/
/* P3: response with ERROR, response, response with body*/

/*******************************************
  Private Helpers:

*******************************************/

int Responder::checkFile(string path){
  string absolutePath = this->doc_root + path;
  string resolvedPath;
  realpath(absolutePath, &resolvedPath[0]);

  if(resolvedPath.find(this->doc_root) != string::npos){
    if((this->fd = open(&resolvedPath[0], O_RDONLY)) < 0){
      // file open error
      switch(errno){
        case EACCES:
        // permission error
        return FORBIDDEN;

        case ENOENT:
        // file not exist
        return NOT_FOUND;
      }
    }
  }
  return 0;
}

int Responder::setFileType(string path){
    string ext = path.substr(path.find_last_of(".") + 1);
    if(ext.compare("html")){
      this->type = TEXT;
    }else if(ext.compare("jpeg")){
      this->type = JPEG;
    }else if(ext.compare("png")){
      this->type = PNG;
    }else{
      // try to access file not ought to be sent
      return NOT_FOUND;
    }

    return 0;
}
int Responder::verifyReq(HttpInstruction req){
  /*
    verify host
  */
  if(req.host.size() == 0){
    return CLIENT_ERROR;
  }

  /*
    Check and set if file can be accessed
  */
  int file_stat = checkFile(req.url);
  if(file_stat != 0){
    return file_stat;
  }

  /*
    Check and set file extension
  */
  int ext_stat = setFileType(req.url);
  if(ext_stat != 0){
    return ext_stat;
  }

  return OK;
}

void Responder::appendInitLine(vector<char> &sendQ, int statCode){
  /* Append Initial Line: HTTP version, status code, description*/
  string resInitLine;
  /* Append HTTP/1.1 */
  resInitLine += HTTP + to_string(HTTP_VER_UPPER) + " ";
  /* Append Status code */
  resInitLine += to_string(statCode) + " ";
  /* Append description*/
  switch(statCode){
    case FORBIDDEN:
      resInitLine += FORBIDDEN_DES;
    break;

    case NOT_FOUND:
      resInitLine += NOT_FOUND_DES;
    break;

    case CLIENT_ERROR:
      resInitLine += CLIENT_ERROR_DES;
    break;

    default:
    // ok
      resInitLine += OK_DES;
  }

  resInitLine += DELIMITER;
  sendQ.insert(sendQ.end(), resInitLine.begin(), resInitLine.end());
}

void Responder::appendContentType(vector<char> &sendQ, FileType type){
  /* Append headers*/
  string cnt_type = CONTENT_TYPE;
  switch(type){
    case JPEG:
      cnt_type += CONTENT_JPEG;
    break;

    case PNG:
      cnt_type += CONTENT_PNG;
    break;

    default:
      cnt_type += CONTENT_TXT;
  }
  cnt_type += DELIMITER;
  sendQ.insert(cnt_type.begin(), cnt_type.end());
}

void Responder::appendContentLength(vector<char> &sendQ, off_t size){
  int fs_size = (int)size;
  string sz = CONTENT_LEN + to_string(fs_size);
  sz += DELIMITER;
  sendQ.insert(sz.begin(), sz.end());
}

void Responder::appendLastModified(vector<char> &sendQ, time_t mtime){
  /* Append Last-Modified*/
}

void Responder::response(int statCode, int fd, FileType type){
  off_t offset = 0;
  const int BUFSIZE = 4096;
  vector<char> sendQ;

  // add HTTP response initial line
  appendInitLine(sendQ, statCode);

  /* Append Content-Type*/
  appendContentType(sendQ, type);

  /* Append Content-Length*/
  stat fileStat;
  fstat(fd, &fileStat);
  appendContentLength(sendQ, fileStat.st_size);

  /* Append Last modified*/
  appendLastModified(sendQ, fileStat.st_mtime);

  send(clntSock, (void *)&sendQ[0], sendQ.size());

  /* Send File as body, until EOF */
  while(sendfile(clntSock, fd, &offset, BUFSIZE) > 0);
}

/************************************
  Public methods:

************************************/


int Responder::verifyandAppendReq(HttpInstruction req){
  // check if request object is valid
  if(!req){
    return CLIENT_ERROR;
  }

  return verifyReq(req);
}

void Responder::sendResponse(int status){
  switch(status){
    case OK:
      response(status, this->fd, type);
    break;

    case FORBIDDEN: // 403 forbidden
      int frbid_fd = openat(AT_FDCWD, FORBIDDEN_PATH);
      response(status, frbid_fd, TEXT);
    break;

    case NOT_FOUND: // 404 not found
      int ntFd_fd = openat(AT_FDCWD, NOT_FOUND_PATH);
      response(status, ntFd_fd, TEXT);
    break;

    default: // 400 client error
      int clnt_fd = openat(AT_FDCWD, CLIENT_ERROR_PATH);
      response(status, clnt_fd, TEXT);
  }
  return;
}

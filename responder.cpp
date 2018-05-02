#include "responder.hpp"
using namespace std;

// handle reponse based on request data structure
/* P2: Implement basic URL check policy, avoid client from accessing unauthorized path */
/* Check host*/
/* P3: response with ERROR, response, response with body*/

/*******************************************
  Private Helpers:

*******************************************/
vector<string> Responder::parseHelper(string insstr, char del){
	vector<string> res;
	unsigned begin = 0;
	unsigned end = 0;

	while(end < insstr.size()){
		if(insstr[end] == del){
			res.push_back(insstr.substr(begin, end - begin));
			begin = end + 1;
		}
			end++;
	}

	if(begin != end){
		res.push_back(insstr.substr(begin, end - begin));
	}
	return res;
}

int Responder::checkFile(string path){
  string absolutePath = this->doc_root + path;
  char *resolved_t = new char;
  realpath(&absolutePath[0], resolved_t);
  string resolvedPath(resolved_t);
  // cerr << resolvedPath << '\n';
  if(resolvedPath.find(this->doc_root) != string::npos){
    return NOT_FOUND;
  }

  if((this->fd = open(static_cast<char *>(&resolvedPath[0]), O_RDONLY)) < 0){
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
  cerr << "check file: " << file_stat <<'\n';
  if(file_stat != 0){
    return file_stat;
  }

  /*
    Check and set file extension
  */
  int ext_stat = setFileType(req.url);
  cerr << "set file type" << ext_stat << '\n';
  if(ext_stat != 0){
    return ext_stat;
  }

  return OK;
}

void Responder::appendInitLine(int statCode){
  /* Append Initial Line: HTTP version, status code, description*/
  string resInitLine;
  /* Append HTTP/1.1 */
  resInitLine += HTTP + to_string(HTTP_VER_UPPER).substr(0, 3) + " ";
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
  cerr << resInitLine << '\n';
  this->sendQ += resInitLine;
}

void Responder::appendContentType(FileType type){
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
  cerr << cnt_type << '\n';
  this->sendQ += cnt_type;
}

void Responder::appendContentLength(struct stat f_stat){
  int fs_size = (int)f_stat.st_size;
  string sz = CONTENT_LEN + to_string(fs_size);
  sz += DELIMITER;
  cerr << sz << '\n';
    this->sendQ += sz;
}

void Responder::appendLastModified(struct stat f_stat){
  /* Append Last-Modified*/
  string lm = LAST_MOD;
  /* temp format: Www MMM DD HH:MM:SS YYYY*/
  cerr << lm << '\n';
  time_t t = f_stat.st_mtime;
  struct tm* gmt = gmtime(&t);
  char gm[512];
  strftime(gm, 512, "%a, %d %b %Y %T %Z", gmt);
  string tim(gm);
  lm += tim;
  cerr << tim << '\n';
  vector<string> token = parseHelper(tim, ' ');

  /* Reformat the time string */
  // lm += token[0] + ", " + token[2] + " " + token[1] + " " + token[3] + " " + token[4] + "GMT";
  lm += DELIMITER;
    cerr << lm << '\n';
    this->sendQ += lm;
}

void Responder::appendServ(string serv){
  /* Append server version to header*/
  string sv = SERVER;
  sv += serv;
  sv += DELIMITER;
    cerr << sv << '\n';
      this->sendQ += sv;
}

void Responder::response(int statCode, int fd, FileType type){
  off_t offset = 0;
  const int BUFSIZE = 4096;
  struct stat fileStat;

  if(fstat(fd, &fileStat) < 0){
    cerr << strerror(errno) << '\n';
  }

  // add HTTP response initial line
  appendInitLine(statCode);

  /* Append Last modified*/
  cerr << "Last modified: " << fileStat.st_mtime <<'\n';
  appendLastModified(fileStat);

  /* Append Content-Type*/
  appendContentType(type);

  /* Append Content-Length*/
  appendContentLength(fileStat);

  /* Append Server Name*/
  appendServ(SERVER_VER_NAME);
  send(clntSock, (void *)&sendQ[0], sendQ.size(), 0);

  /* Send File as body, until EOF */
  while(sendfile(clntSock, fd, &offset, BUFSIZE) > 0);
}

/************************************
  Public methods:

************************************/
int Responder::verifyandAppendReq(HttpInstruction req){
  // check if request object is valid
  if(req.host.size() == 0){
    return CLIENT_ERROR;
  }
  // cerr << req.host << '\n';
  // cerr << req.http_ver << '\n';
  // cerr << req.url << '\n';
  // cerr << req.connection << '\n';
  return verifyReq(req);
}

void Responder::sendResponse(int status){
  int frbid_fd;
  int Fd_fd;
  int clnt_fd;
  switch(status){
    case OK:
      response(status, this->fd, type);
    break;

    case FORBIDDEN: // 403 forbidden
      if((frbid_fd = openat(AT_FDCWD, &FORBIDDEN_PATH[0], O_RDONLY)) < 0){
        cerr << strerror(errno) << '\n';
      }
      response(status, frbid_fd, TEXT);
    break;

    case NOT_FOUND: // 404 not found
      if((Fd_fd = openat(AT_FDCWD, &NOT_FOUND_PATH[0], O_RDONLY)) < 0){
        cerr << strerror(errno) << '\n';
      }
      response(status, Fd_fd, TEXT);
    break;

    default: // 400 client error
      if((clnt_fd = openat(AT_FDCWD, &CLIENT_ERROR_PATH[0], O_RDONLY)) < 0){
        cerr << strerror(errno) << '\n';
      }
      response(status, clnt_fd, TEXT);
  }
  return;
}

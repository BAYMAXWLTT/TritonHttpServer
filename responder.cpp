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
	char *root = &(this->doc_root[0]);
	string absolutePath(root);
	int *res_file = &(this->fd);

	absolutePath.append(path);

  char resolved_t[1024];
  realpath(&absolutePath[0], resolved_t);
  string resolvedPath(resolved_t);
	char *filePath = &resolvedPath[0];

  size_t pos = resolvedPath.find(root);
  if(pos == string::npos){
    return NOT_FOUND;
  }
	cerr << filePath << '\n';
	int filed = open(filePath, O_RDONLY);
	cerr << filed <<'\n';
  if(filed < 0){
    // file open error
    switch(errno){
      case EACCES:
      // permission error
      return FORBIDDEN;

      default: // ENOENT:
      // file not exist
      return NOT_FOUND;
    }
  }
	*res_file = filed;
	return 0;
}

int Responder::setFileType(string path){
    string ext = path.substr(path.find_last_of(".") + 1);

    if(ext.compare("html") == 0){
      this->type = TEXT;
    }else if(ext.compare("jpg") == 0){
      this->type = JPEG;
    }else if(ext.compare("png") == 0){
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
  this->sendQ += resInitLine;
}

void Responder::appendContentType(){
  /* Append headers*/
  string cnt_type = CONTENT_TYPE;
  switch(this->type){
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
  this->sendQ += cnt_type;
}

void Responder::appendContentLength(){
  struct stat f_stat;
  if(fstat(this->fd, &f_stat) < 0){
    cerr << strerror(errno) << '\n';
  }
  int fs_size = (int)f_stat.st_size;
  string sz = CONTENT_LEN + to_string(fs_size);
  sz += DELIMITER;
    this->sendQ += sz;
}

void Responder::appendLastModified(){
  /* Append Last-Modified*/
  string lm = LAST_MOD;
  struct stat f_stat;
  if(fstat(this->fd, &f_stat) < 0){
    cerr << strerror(errno) << '\n';
  }
  /* temp format: Www MMM DD HH:MM:SS YYYY*/
  time_t t = f_stat.st_mtime;

  char gm[512];
  struct tm* gmt;
  gmt = gmtime(&t);
  strftime(gm, 512, "%a, %d %b %Y %T %Z", gmt);
  string tim(gm);
  lm += tim;
  lm += DELIMITER;
	
  this->sendQ += lm;
}

void Responder::appendServ(string serv){
  /* Append server version to header*/
  string sv = SERVER;
  sv += serv;
  sv += DELIMITER;
      this->sendQ += sv;
}

void Responder::response(int statCode){
  off_t offset = 0;
  const int BUFSIZE = 4096;
  // struct stat fileStat;

  // add HTTP response initial line
  appendInitLine(statCode);

  /* Append Last modified*/
  // cerr << "Last modified: " << fileStat.st_mtime <<'\n';
  appendLastModified();

  /* Append Content-Type*/
  appendContentType();

  /* Append Content-Length*/
  appendContentLength();

  /* Append Server Name*/
  appendServ(SERVER_VER_NAME);
	char *header = &(this->sendQ[0]);
  if(send(clntSock, (void *)header, this->sendQ.size(), 0) < 0){
		cerr << strerror(errno) << '\n';
	}

  /* Send File as body, until EOF */
	// cerr << this->fd << '\n';
	// cerr << this->sendQ << '\n';
	int sent = 0;
  while((sent = sendfile(clntSock, this->fd, &offset, BUFSIZE)) > 0){
		cerr << sent << '\n';
	}
	if(sent < 0){
		cerr << strerror(errno) << '\n';
	}
}

/************************************
  Public methods:

************************************/
int Responder::verifyandAppendReq(HttpInstruction req){
  // check if request object is valid
  if(req.host.size() == 0){
    return CLIENT_ERROR;
  }

  return verifyReq(req);
}

void Responder::sendResponse(int status){
  switch(status){
    case OK:
      response(status);
    break;

    case FORBIDDEN: // 403 forbidden
      if((this->fd = openat(AT_FDCWD, &FORBIDDEN_PATH[0], O_RDONLY)) < 0){
        cerr << strerror(errno) << '\n';
      }
			this->type = TEXT;
      response(status);
    break;

    case NOT_FOUND: // 404 not found
      if((this->fd = openat(AT_FDCWD, &NOT_FOUND_PATH[0], O_RDONLY)) < 0){
        cerr << strerror(errno) << '\n';
      }
			this->type = TEXT;
      response(status);
    break;

    default: // 400 client error
      if((this->fd = openat(AT_FDCWD, &CLIENT_ERROR_PATH[0], O_RDONLY)) < 0){
        cerr << strerror(errno) << '\n';
      }
			this->type = TEXT;
      response(status);
  }

	close(this->fd);
	this->fd = -1;
	sendQ = "";
  return;
}

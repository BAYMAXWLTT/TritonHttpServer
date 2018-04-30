/******************************************************************************************
Project: UCSD CSE291C Course Project: Web Server for TritonHTTP

Author:
1. Hou Wang

Verify the header format and parse the request into valid request data structure
*******************************************************************************************/

#include <string>
#include <iostream>
#include <vector>
#include "httpd.h"
#include "responder.hpp"
#include "parser.hpp"

using namespace std;
/* Constructor */
Parser::Parser(){
		_isTerminated = false;
}

// need to modify parser to parse header
bool Parser::parse(string insstr){
	// PUT YOUR CODE HERE
	vector<string> key_val;
	// insstr.erase(remove(insstr.begin(), insstr.end(), ' '), insstr.end());
	string token = strtok(insstr, " ");
	while(token != NULL){
			key_val.push_back(token);
			token = strtok(NULL, " ");
	}

	if(key_val.size() == 3){
		/*
			This is the http initial line:
			1). verify if GET is correct
			2). verify if http version is supported
		*/

		if(key_val[0].compare("GET") != 0){
			return false;
		}

		/* Path overwrite for "/" default to index.html*/
		_req.url = key_val[1];
		if(_req.url == "/"){
			_req.url = "/index.html";
		}

		string http = strtok(key_val[2], "\\");
		if(http.compare("HTTP") != 0){
			// if HTTP version is not correctly formed
			return false;
		}
		_req.http_ver = stof(strtok(NULL, "\\"));

		/*
			verify http version is supported
		*/
		if(_req.http_ver > HTTP_VER_UPPER){
			return CLIENT_ERROR;
		}
	}else if(key_val.size() == 2){
		/*
			This is the "key: val" pair
			1). verify if this header is supported
		*/
		string key = key_val[0].substr(0, key_val[0].size() - 1); // remove ':' from end
		string val = key_val[1];

		if(key.compare("Host") == 0){
			_req.host = val;
		}else if(key.compare("Connection") == 0){
			_req.connection = val;
			if(val.compare("close") == 0){}
				_isTerminated = true;
		}else{
			/* Not supoprted headers */
			return false;
		}
	}

	return true;
}

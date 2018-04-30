#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <cstdint>
using namespace std;

typedef struct HttpInstruction_t {
	// DEFINE YOUR DATA STRUCTURE HERE
	/* Noted: the only correct method is GET, therefore no instruction is indicated here*/
	string url;
	float http_ver;
	string host;
	string connection;
} HttpInstruction;

class Parser {
public:
	/*
		Constructor, initialize private fields
	*/
	Parser();

	/*
		Parse string into valid header
		Return: true, if string contains valid header; otherwise false
	*/
	bool parse(string insstr);

	/*
		Return currently parsed http headers
	*/
	HttpInstruction getReqHeader(){
		return _req;
	}

	/*
		Return if connection needs to be terminated by client
	*/
	bool isTerminated(){
		return _isTerminated;
	}

private:
	HttpInstruction _req;
	bool _isTerminated;
};

#endif // CALCPARSER_HPP

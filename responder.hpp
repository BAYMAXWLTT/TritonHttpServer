#ifndef RESPONDER_HPP
#define RESPONDER_HPP

#include <string>

using namespace std;

class Responder {

private:
  string

public:
	/*
		Constructor, initialize private fields
	*/
	Responder();

	/*
		Parse string into valid header
		Return: true, if string contains valid header; otherwise false
	*/
	bool response(string insstr);


};

#endif // CALCPARSER_HPP

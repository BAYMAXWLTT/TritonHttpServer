#ifndef RESPONDER_HPP
#define RESPONDER_HPP

class Responder {

private:
	HttpInstruction _req;
	bool _isTerminated;

public:
	/*
		Constructor, initialize private fields
	*/
	Responder();

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
};

#endif // CALCPARSER_HPP

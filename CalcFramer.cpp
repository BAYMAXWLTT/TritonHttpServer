#include <assert.h>
#include "CalcFramer.hpp"
#include <iostream>
using namespace std;

void CalcFramer::append(string chars)
{
	// PUT YOUR CODE HERE
	for(unsigned i = 0; i < chars.length(); i++){
		_msgBuffer.push_back(chars[i]);
	}
	return;
}

bool CalcFramer::hasMessage() const
{
	// PUT YOUR CODE HERE
	if(_msgBuffer.size() == 0){
		return false;
	}

	for(auto be = CalcFramer::_msgBuffer.begin(), en = CalcFramer::_msgBuffer.end(); be != en; ++be){
		char cur = *be;
		if(cur == CalcFramer::CR && be != en){
			char next = *(be + 1);
			if(next == CalcFramer::LF){
					return true;
			}
		}
	}
	return false;
}

string CalcFramer::topMessage() const
{
	// PUT YOUR CODE HERE
	string msg;
	for(auto be = _msgBuffer.begin(), en = _msgBuffer.end(); be != en; ++be){
		char cur = *be;
		if(cur == CalcFramer::CR && be != en){
			char next = *(be + 1);
			if(next == CalcFramer::LF){
					break;
				}
			}

			if(cur != CalcFramer::CR && cur != CalcFramer::LF){
					msg += cur;
			}
		}
	return msg;
}


void CalcFramer::popMessage()
{
	// PUT YOUR CODE HERE
	for(auto be = _msgBuffer.begin(), en = _msgBuffer.end(); be != en; ++be){
		char cur = *be;
		if(cur == CalcFramer::CR && be != en){
			char next = *(be + 1);
			if(next == CalcFramer::LF){
				if(be + 1 == en){
					_msgBuffer.erase(_msgBuffer.begin(), _msgBuffer.end());
				}else{
					advance(be, 2);
					_msgBuffer.erase(_msgBuffer.begin(), be);
				}
				return;
			}
		}
	}

	return;
}

void CalcFramer::printToStream(ostream& stream) const
{
	// (OPTIONAL) PUT YOUR CODE HERE--useful for debugging
	string msg(CalcFramer::_msgBuffer.begin(), CalcFramer::_msgBuffer.end());
	stream << msg << '\n';

}

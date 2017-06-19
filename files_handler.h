#ifndef FILES_HANDLER_H
#define FILES_HANDLER_H

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "Token.h"

using std::ifstream;
using std::cout;

class Reader{
private:
  ifstream input_file;


public:
	Reader(char* filename); 
	~Reader();

	Token get();
	bool end_of_file();


};

#endif
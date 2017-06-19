#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <cstring>

using std::string;

class Token{
private:
int lookupKeyword(string word);

public:
 	static const int END = -1;
 	
 	static const int CALC = 1;
 	static const int TS = 2;
 	static const int SYSTEM = 3;
 	static const int REAGENT = 4;


 	static const int NUMBER = 10;
 	static const int TEXT  = 11;
 	static const int SECTION_KWD  = 12;
 	static const int EQUAL = 13;
 	static const int SEMICOLON = 9;






 	Token(int ttype);
 	Token(int ttype, double val);
 	Token(int ttype, string s);



	int toktype;
	string str;
	int ival;
	double dval;






};

#endif

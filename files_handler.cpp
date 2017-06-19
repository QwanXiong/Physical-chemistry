#include "files_handler.h"

#include <cctype>

Reader::Reader(char* filename)
{

	input_file.open(filename, ifstream::in);
	if (! input_file.is_open())
		throw std::runtime_error("Error opening file!");

	cout << "file opened\n";
}


Token Reader::get(){

	//Token a(2);

	char ch;

	//cout << "beta\n";

	ch = input_file.get();

	while(isspace(ch))
	{
		ch = input_file.get();

	}

	if (ch == '!')
	{
		while ( (ch = input_file.get()) != '\n');
		input_file.putback(ch);
		return this->get();

	}
	
	if (ch == '#')
	{	
		return Token(Token::SECTION_KWD);
		ch = input_file.get();
	}

	if (ch == '=')
	{	
		return Token(Token::EQUAL);
		ch = input_file.get();
	}

	if (ch == ';')
	{	
		return Token(Token::SEMICOLON);
		ch = input_file.get();
	}
	//cout << toktype << "\n";


	if (isdigit(ch) || ch == '-') {
	    int sign = 1;
	   /* if (ch == '-') {  // is negative number?
	      sign = -1;
	      *inp_ >> ch;
	      if (!isdigit(ch)) {
	        inp_->putback(ch);
	        return Token('-');
	      }
	    }*/
	    input_file.putback(ch);
	    double val;
	    input_file >> val;
	    return Token(Token::NUMBER, sign*val);
  	}
	else if (isalpha(ch) || ch == '.') {
	    string word;
	    while (isalnum(ch) || ch == '_' || ch == '.') {
	      word += tolower(ch);
	      ch = input_file.get();

	     // cout << word <<'\n';
	      //cout << ch;
	    }
	    input_file.putback(ch);
	   // cout << word <<'\n';
	   return Token(Token::TEXT, word);
   // return Token(1);
  }


	//return a;
}

bool Reader::end_of_file()
{
	return input_file.eof();
}

Reader::~Reader()
{

	input_file.close();
}
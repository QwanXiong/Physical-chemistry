#include "Token.h"

Token::Token(int ttype)
: toktype(ttype)
{

}

Token::Token(int ttype, double val)
  : toktype(ttype), str(""), dval(val)
{
}

Token::Token(int ttype, string s)
  : str(s), dval(0.0)
{
  toktype = lookupKeyword(s);
}


int Token::lookupKeyword(string word)
{
  if (word == "system")
    return SYSTEM;
  else if (word == "calc")
    return CALC;
  else if (word == "reagent")
    return REAGENT;
  else if (word == "ts")
    return TS;
  else if (word == "end")
    return END;
  else return TEXT;
 
}
#pragma once
#include <iostream>
#include <regex>
#include <vector>
#include <stdexcept>

#include "syntax.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

class Lexer{
private:
  const vector<LexDef> TokenLex;
public:
  Lexer():TokenLex(Syntax::tokens){}
  Lexer(vector<LexDef> TokenLex):TokenLex(TokenLex){}
  vector<Token> analize(string code) const;
};

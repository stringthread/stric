#pragma once
#include <iostream>
#include <regex>
#include <vector>
#include <stdexcept>

using std::string;
using std::vector;
using std::cout;
using std::endl;

const string IDENTIFIER_START_CHAR=R"***([a-zA-Z_])***";
const string IDENTIFIER_CHAR=R"***([a-zA-Z0-9_])***";

class LexDef{
private:
  static string format_regex(string& arg_ptn, bool flg_force_end);
public:
  string token;
  std::regex ptn;
  LexDef(string arg_token, string arg_ptn, bool flg_force_end=false)
    :token(arg_token), ptn(format_regex(arg_ptn, flg_force_end))
    {
      //cout<<arg_ptn<<endl;
    }
  string match(string str) const;
};

class Token{
public:
  string token;
  string value;
  Token(string arg_token, string value)
    :token(arg_token), value(value)
    {}
  operator string() const{
    return token+"    ("+value+")";
  }
};

class Lexer{
private:
  static vector<LexDef> TokenLex;

public:
  vector<Token> analize(string code) const;
};

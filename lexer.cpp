#include "lexer.h"

using namespace std;

constexpr string IDENTIFIER_START_CHAR="[a-zA-Z_]";
constexpr string IDENTIFIER_CHAR="[a-zA-Z0-9_]";

string LexDef::match(string str);{
  cmatch m;
  bool result=regex_search(str,m,ptn);
  if(!result) return "";
  return m.str();
}

vector<Token> Lexer::analize(string code){
    int pos=0;
    vector<Token> token_list;
    while(pos<len(code)){
        for(token in TokenLex){
            match=token.match(pos)
            if(match=="") continue;
            if(token.token!="") token_list.emplace_back(token.token, match);
            pos+=match.length;
            break;
        }
    }
    return token_list;
}

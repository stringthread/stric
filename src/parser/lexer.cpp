#include "lexer.h"

vector<Token> Lexer::analize(string code) const{
  int pos=0;
  bool flg_match;
  vector<Token> token_list;
  while(pos<code.size()){
    flg_match=false;
    for(const auto& token : TokenLex){
      const string match=token.match(code.substr(pos));
      if(match=="") continue;
      flg_match=true;
      if(token.token!="") token_list.emplace_back(token.token, match);
      pos+=match.size();
      break;
    }
    if(!flg_match) throw std::runtime_error("Nothing match for pos: "+std::to_string(pos));
  }
  token_list.emplace_back("EOF","");
  return token_list;
}

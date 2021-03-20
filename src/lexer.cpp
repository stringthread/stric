#include "lexer.h"

using namespace std;

string LexDef::format_regex(string& arg_ptn, bool flg_force_end){
  arg_ptn="^"+arg_ptn;
  if(flg_force_end) arg_ptn+="(?!"+IDENTIFIER_CHAR+"})";
  return arg_ptn;
}

string LexDef::match(string str) const{
  std::smatch m;
  bool result=std::regex_search(str,m,ptn);
  if(!result) return "";
  return m.str();
}

vector<LexDef> Lexer::TokenLex {
   {"", R"***( +)***"}, // whitespace: ignore
   {"", R"***(\r?\n)***"}, // EOL: ignore
   {"EOS", R"***(;)***"},
   {"NULL", "null", true},
   {"TRUE", R"***(@t)***", true},
   {"FALSE", R"***(@f)***", true},
   {"HEX_INT", R"***(0x[1-9a-fA-F][0-9a-fA-F]*)***"},
   {"FLOAT", R"***(([0-9]+\.[0-9]*|[0-9]*\.[0-9]+))***"}, // "2.", ".5", ".2"=> OK, "."=>NG
   {"INT", R"***([1-9][0-9]*)***"},
   {"TYPE_INT", R"***(int)***", true},
   {"TYPE_FLOAT", R"***(float)***", true},
   {"TYPE_BOOL", R"***(bool)***", true},
   {"PLUS", R"***(\+)***"},
   {"MINUS", R"***(-)***"},
   {"STAR", R"***(\*)***"},
   {"DIV_INT", R"***(//)***"},
   {"SLASH", R"***(/)***"},
   {"ANDAND", R"***(&&)***"},
   {"AND", R"***(&)***"},
   {"OROR", R"***(\|\|)***"},
   {"OR", R"***(\|)***"},
   {"HAT", R"***(^)***"},
   {"NOT", R"***(not|\|)***"},
   {"EQUAL", R"***(==)***"},
   {"NOT_EQUAL", R"***(!=)***"},
   {"LESS_EQUAL", R"***(<=)***"},
   {"GREATER_EQUAL", R"***(>=)***"},
   {"LEFT_ANGLE_BRACKET", R"***(<)***"},
   {"RIGHT_ANGLE_BRACKET", R"***(>)***"},
   {"LEFT_PAREN", R"***(\()***"},
   {"RIGHT_PAREN", R"***(\))***"},
   {"LEFT_BRACE", R"***(\{)***"},
   {"RIGHT_BRACE", R"***(\})***"},
   {"LEFT_BRACKET", R"***(\[)***"},
   {"RIGHT_BRACKET", R"***(\])***"},
   {"ASSIGN", R"***(=)***"},
   {"IDENTIFIER", ""+IDENTIFIER_START_CHAR+IDENTIFIER_CHAR+"*"},
};

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

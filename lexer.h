#include <iostream>
#include <regex>
#include <vector>
#include <array>

using namespace std;

constexpr string IDENTIFIER_START_CHAR="[a-zA-Z_]";
constexpr string IDENTIFIER_CHAR="[a-zA-Z0-9_]";

class LexDef{
public:
  string token;
  regex ptn;
  LexDef(string arg_token, string arg_ptn, bool flg_force_end=False){
      token=arg_token;
      if(flg_force_end) arg_ptn+="(?!{IDENTIFIER_CHAR})";
      ptn=regex(arg_ptn);
    }
  string match(string str);
};

class Token{
public:
    Token(string token, string value){
        this.token=token;
        this.value=value;
    }
    operator string(){
      return token+"    ("+value+")";
    }
};

class Lexer{
  //data_LexDefinitions: Array[Token_Name, Pattern(Uncompiled RegExp)]
  constexpr vector<array<string,2>> data_LexDefinitions {
      {"", R"( +)"}, // whitespace: ignore
      {"", R"(\r?\n)"}, // EOL: ignore
      {"EOS", R"(;)"},
      ["NULL", "null", True],
      {"TRUE", R"(@t)", True},
      {"FALSE", R"(@f)", True},
      {"HEX_INT", R"(0x[1-9a-fA-F][0-9a-fA-F]*)"},
      {"INT", R"(-?[1-9][0-9]*)"},
      {"FLOAT", R"(-?([0-9]+\.[0-9]*|[0-9]*\.[0-9]+))"}, # "2.", ".5", "-.2"=> OK, "-."=>NG
      {"TYPE_INT", R"(int)", True},
      {"TYPE_FLOAT", R"(float)", True},
      {"TYPE_BOOL", R"(bool)", True},
      {"ASSIGN", R"(=)"},
      {"PLUS", R"(+)"},
      {"MINUS", R"(-)"},
      {"STAR", R"(*)"},
      {"DIV_INT", R"(//)"},
      {"SLASH", R"(/)"},
      {"ANDAND", R"(&&)"},
      {"AND", R"(&)"},
      {"OROR", R"(||)"},
      {"OR", R"(|)"},
      {"HAT", R"(^)"},
      {"NOT", R"(not|\|)"},
      {"EQUAL", R"(==)"},
      {"NOT_EQUAL", R"(!=)"},
      {"LESS_EQUAL", R"(<=)"},
      {"GREATER_EQUAL", R"(>=)"},
      {"LEFT_ANGLE_BRACKET", R"(<)"},
      {"RIGHT_ANGLE_BRACKET", R"(>)"},
      {"LEFT_PAREN", R"(()"},
      {"RIGHT_PAREN", R"())"},
      {"LEFT_BRACE", R"({)"},
      {"RIGHT_BRACE", R"(})"},
      {"LEFT_BRACKET", R"([)"},
      {"RIGHT_BRACKET", R"(])"},
      {"IDENTIFIER", R"({IDENTIFIER_START_CHAR}{IDENTIFIER_CHAR}*)"},
  };
  array<LexDef,data_LexDefinitions.size()> TokenLex;

public:
  Lexer(){
    for(int i=0;i<data_LexDefinitions.size();i++){
      LexDef[i]=LexDef(data_LexDefinitions[i][0],data_LexDefinitions[i][1]);
    }
  }
  vector<Token> analize(string code);
};

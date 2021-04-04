#pragma once
#include <iostream>
#include <regex>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>

using std::string;
using std::cout;
using std::endl;

namespace Syntax{
  const string IDENTIFIER_START_CHAR=R"***([a-zA-Z_])***";
  const string IDENTIFIER_CHAR=R"***([a-zA-Z0-9_])***";
  const string START_SYMBOL="main";
  const string META_START_SYMBOL="META_START_SYMBOL";
  const string EOF_SYMBOL="EOF";
}

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

class SyntaxDef{
private:
  string l_token;
  std::vector<string> ptn;
public:
  SyntaxDef(const string &arg_l_token, const std::vector<string> &arg_ptn)
    :l_token(arg_l_token), ptn(arg_ptn)
    {}
  const string& get_l_token() const { return l_token; }
  const std::vector<string>& get_ptn() const { return ptn; }
  bool operator==(const SyntaxDef &t) const { return this->l_token==t.l_token && this->ptn==t.ptn; }
};

namespace Syntax{
  const std::vector<LexDef> tokens {
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
     {"ASTERISK", R"***(\*)***"},
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
}

class SymbolDiscriminator{
private:
  std::unordered_set<string> terminals, non_terminals;
public:
  SymbolDiscriminator(const std::vector<SyntaxDef> &syntax_rules);
  const std::unordered_set<string>& get_terminals() const { return terminals; }
  const std::unordered_set<string>& get_non_terminals() const { return non_terminals; }
  bool is_terminal(const string &token) const { return terminals.count(token)==1; }
  bool is_non_terminal(const string &token) const { return non_terminals.count(token)==1; }
};

class FirstSet{
private:
  class NullableSet{
  private:
    std::unordered_set<string> nulls_set;
  public:
    NullableSet(const std::vector<SyntaxDef> &syntax_rules);
    bool is_nullable(const string &token) const{
      return nulls_set.count(token)==1;
    }
  };
  NullableSet nulls;
  std::unordered_map<string, std::unordered_set<string>> first_map;
public:
  FirstSet(const std::vector<SyntaxDef> &syntax_rules, SymbolDiscriminator symbol_dis);
  const std::unordered_set<string>& get(const string &token) const;
  std::unordered_set<string> get(const std::vector<string> &tokens) const;
};

class SyntaxDB{
private:
  const SymbolDiscriminator sd;
  const FirstSet fs;
  std::unordered_map<string, int> token_id_map;
  int token_id_counter=0;
  const std::vector<SyntaxDef> &syntax_rules;
  std::unordered_map<string, std::vector<std::pair<int, SyntaxDef>>> token_def_map;
  const SyntaxDef start_rule;
  void init_token_def_map();
public:
  SyntaxDB(const std::vector<SyntaxDef> &syntax_rules);
  const SymbolDiscriminator& get_sd() const { return sd; }
  const FirstSet& get_fs() const { return fs; }
  const string& get_start_symbol() const { return Syntax::START_SYMBOL; }
  const std::vector<std::pair<int, SyntaxDef>>& find_rule(const string &token) const;
  int get_token_id(const string &token);
  const SyntaxDef& get_rule(int rule_id) const;
  int get_rule_id(const SyntaxDef &rule) const; //return -100 on not-found
};

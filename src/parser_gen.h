#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>

using std::string;
using std::cout;
using std::endl;

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
  bool operator == (const SyntaxDef &t) const { return this->l_token==t.l_token && this->ptn==t.ptn; }
};

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
  const std::unordered_set<string>& get(const std::vector<string> &tokens) const;
};

class SyntaxDB{
private:
  const SymbolDiscriminator sd;
  const FirstSet fs;
  std::unordered_map<string, int> token_id_map;
  int token_id_counter=0;
  const std::vector<SyntaxDef> &syntax_rules;
  std::unordered_map<string, std::vector<std::pair<int, SyntaxDef>>> token_def_map;
  const string start_symbol="main";
  void init_token_def_map();
public:
  SyntaxDB(const std::vector<SyntaxDef> &syntax_rules);
  const SymbolDiscriminator& get_sd() const { return sd; }
  const FirstSet& get_fs() const { return fs; }
  const string& get_start_symbol() const { return start_symbol; }
  const std::vector<std::pair<int, SyntaxDef>>& find_def(const string &token) const;
  int get_token_id(const string &token);
  const SyntaxDef& get(int rule_id) const;
};

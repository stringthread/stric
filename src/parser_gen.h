#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
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
  bool operator==(const SyntaxDef &t) const { return this->l_token==t.l_token && this->ptn==t.ptn; }
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
  const string START_SYMBOL="main";
  const string META_START_SYMBOL="META_START_SYMBOL";
  const SyntaxDef start_rule(META_START_SYMBOL, std::vector<string>({START_SYMBOL}));
  void init_token_def_map();
public:
  SyntaxDB(const std::vector<SyntaxDef> &syntax_rules);
  const SymbolDiscriminator& get_sd() const { return sd; }
  const FirstSet& get_fs() const { return fs; }
  const string& get_start_symbol() const { return start_symbol; }
  const std::vector<std::pair<int, SyntaxDef>>& find_rule(const string &token) const;
  int get_token_id(const string &token);
  const SyntaxDef& get_rule(int rule_id) const;
  int get_rule_id(const SyntaxDef &rule) const; //return -100 on not-found
};

class ClosureItem{
private:
  std::shared_ptr<SyntaxDB> db;
  int rule_id;
  int dot_index;
  std::vector<string> look_aheads;
  string lr0_hash;
  string lr1_hash;
  void sort_look_aheads();
  void update_hash();
public:
  ClosureItem(std::shared_ptr<SyntaxDB> arg_db, int arg_rule_id, int arg_dot_index, std::vector<string> arg_look_aheads);
  ClosureItem(const ClosureItem&);
  ClosureItem& operator=(const ClosureItem &ci);
  int get_rule_id() const { return rule_id; }
  int get_dot_index() const { return dot_index; }
  const std::vector<string>& get_look_aheads() const { return look_aheads; }
  const string& get_lr0_hash() const { return lr0_hash; }
  const string& get_lr1_hash() const { return lr1_hash; }
  bool is_same_lr0(const ClosureItem &c) const { return this->lr0_hash==c.lr0_hash; }
  bool is_same_lr1(const ClosureItem &c) const { return this->lr1_hash==c.lr1_hash; }
  ClosureItem merge(const ClosureItem &c) const;
  std::vector<ClosureItem> separate() const;
};

class ClosureSet{
private:
  std::shared_ptr<SyntaxDB> db;
  std::vector<ClosureItem> closure;
  string lr0_hash;
  string lr1_hash;
  void sort();
  void update_hash();
  void expand();
public:
  ClosureSet(std::shared_ptr<SyntaxDB> arg_db, std::vector<ClosureItem> arg_closure);
  ClosureSet(const ClosureSet&);
  ClosureSet& operator=(const ClosureSet&);
  int size() const { return closure.size(); }
  const std::vector<ClosureItem>& get_closure() const { return closure; }
  bool contains(const ClosureItem &item) const;
  bool is_same_lr0(const ClosureSet &c) const { return lr0_hash==c.lr0_hash; }
  bool is_same_lr1(const ClosureSet &c) const { return lr1_hash==c.lr1_hash; }
  const string& get_lr0_hash() const { return lr0_hash; }
  const string& get_lr1_hash() const { return lr1_hash; }
  ClosureSet merge(const ClosureSet &cs) const;
};

class DFA_Node{
public:
  ClosureSet cs;
  std::unordered_map<string, std::weak_ptr<DFA_Node>> edge;
  DFA_Node(ClosureSet cs, std::unordered_map<string, std::weak_ptr<DFA_Node>> edge=std::unordered_map<string, std::weak_ptr<DFA_Node>>())
    :cs(cs), edge(edge) {}
};

class DFA_Generator{
private:
  std::shared_ptr<SyntaxDB> db;
  std::vector<std::shared_ptr<DFA_Node>> lr_dfa, lalr_dfa;
  std::unordered_map<string, ClosureSet> generate_new_cs(const ClosureSet &cs) const;
  std::weak_ptr<DFA_Node> search_edge_dest(const std::vector<std::shared_ptr<DFA_Node>> &dfa, const std::shared_ptr<DFA_Node> &node) const;
  void generate_dfa();
  void merge_la();
public:
  DFA_Generator(std::shared_ptr<SyntaxDB> db);
  const std::vector<std::shared_ptr<DFA_Node>>& get_lr_dfa() const { return lr_dfa; };
  const std::vector<std::shared_ptr<DFA_Node>>& get_lalr_dfa() const { return lalr_dfa; };
};

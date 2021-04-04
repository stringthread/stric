#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>

#include "syntax.h"

using std::string;
using std::cout;
using std::endl;

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
  const std::vector<std::shared_ptr<DFA_Node>>& get_lr_dfa() const { return lr_dfa; }; // lr_dfa[0]=initial_set
  const std::vector<std::shared_ptr<DFA_Node>>& get_lalr_dfa() const { return lalr_dfa; }; // lr_dfa[0]=initial_set
};

#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

#include "dfa.h"
class IOperation;

using std::string;
using std::cout;
using std::endl;

enum class TableType{
  EMPTY, LALR1, LR1, CONFLICT,
};

using ParsingTableRow=std::unordered_map<string, std::shared_ptr<IOperation>>;
using ParsingTable = std::pair<std::unordered_map<string, ParsingTableRow>, string>;//(cs_lr1_hash->row, initial_set.lr1_hash)

class ParserGenerator{
private:
  std::shared_ptr<SyntaxDB> db;
  DFA_Generator dfa_gen;
  ParsingTable parsing_table;
  TableType table_type=TableType::EMPTY;
  std::pair<ParsingTable, bool> generate_parsing_table(std::vector<std::shared_ptr<DFA_Node>> dfa);
  void init_table();
public:
  ParserGenerator(const std::vector<SyntaxDef> &syntax_rules);
  ParserGenerator(const std::shared_ptr<SyntaxDB> db);
  // const Parser& get_parser();
  TableType get_table_type() const { return table_type; }
  bool is_conflict() const { return table_type==TableType::CONFLICT; }
  const ParsingTable& get_parsing_table() const { return parsing_table; }
  std::shared_ptr<SyntaxDB> get_db() const { return db; }
};

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

#include "dfa.h"

using std::string;
using std::cout;
using std::endl;

enum class OperationType{
  Accept, Goto, Reduce, Shift, Conflict,
};

enum class TableType{
  LALR1, LR1, CONFLICT,
};

class IOperation{
public:
  virtual OperationType type() const {};
};

class AcceptOperation : public IOperation{
public:
  virtual OperationType type() const { return OperationType::Accept; }
};
class GotoOperation : public IOperation{
public:
  virtual OperationType type() const { return OperationType::Goto; }
  const string to;
  GotoOperation(const string &to):to(to){}
};
class ReduceOperation : public IOperation{
public:
  virtual OperationType type() const { return OperationType::Reduce; }
  const int rule_id;
  ReduceOperation(int rule_id):rule_id(rule_id){}
};
class ShiftOperation : public IOperation{
public:
  virtual OperationType type() const { return OperationType::Shift; }
  const string to;
  ShiftOperation(const string &to):to(to){}
};
class ConflictOperation : public IOperation{
public:
  virtual OperationType type() const { return OperationType::Conflict; }
  ConflictOperation(const std::vector<string> &shift_to, const std::vector<int> &reduce_rule_id)
    :shift_to(shift_to), reduce_rule_id(reduce_rule_id){}
  ConflictOperation(const IOperation & op, int arg_reduce_rule_id);
  std::vector<string> shift_to;
  std::vector<int> reduce_rule_id;
};

using ParsingTableRow=std::unordered_map<string, IOperation>;
using ParsingTable = std::unordered_map<string, ParsingTableRow>;//cs_lr1_hash->row

class ParserGenerator{
private:
  std::shared_ptr<SyntaxDB> db;
  DFA_Generator dfa_gen;
  ParsingTable parsing_table;
  TableType table_type;
  std::pair<ParsingTable, bool> generate_parsing_table(std::vector<std::shared_ptr<DFA_Node>> dfa);
  void init_table();
public:
  ParserGenerator(const std::vector<SyntaxDef> &syntax_rules);
  // const Parser& get_parser();
  TableType get_table_type() const { return table_type; }
  bool is_conflict() const { return table_type==TableType::CONFLICT; }
  const ParsingTable& get_parsing_table() const { return parsing_table; }
};

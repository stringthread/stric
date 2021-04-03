#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <memory>

#include "parser.h"

using std::string;
using std::cout;
using std::endl;

enum class OperationType{
  Accept, Goto, Reduce, Shift, Conflict,
};

class AST_Node;

class IOperation{
public:
  virtual OperationType type() const=0;
  virtual bool operator==(const IOperation&) const=0;
  virtual bool operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const=0;
};

class AcceptOperation : public IOperation{
public:
  virtual OperationType type() const override { return OperationType::Accept; }
  bool operator==(const IOperation &op) const override {
    return op.type()==OperationType::Accept;
  }
  bool operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const override;
};
class GotoOperation : public IOperation{
public:
  virtual OperationType type() const override { return OperationType::Goto; }
  const string to;
  GotoOperation(const string &to):to(to){}
  bool operator==(const IOperation &op) const override {
    return op.type()==OperationType::Goto && to==dynamic_cast<const GotoOperation&>(op).to;
  }
  bool operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const override;
};
class ReduceOperation : public IOperation{
public:
  virtual OperationType type() const override { return OperationType::Reduce; }
  const int rule_id;
  ReduceOperation(int rule_id):rule_id(rule_id){}
  bool operator==(const IOperation &op) const override {
    return op.type()==OperationType::Reduce && rule_id==dynamic_cast<const ReduceOperation&>(op).rule_id;
  }
  bool operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const override;
};
class ShiftOperation : public IOperation{
public:
  virtual OperationType type() const override { return OperationType::Shift; }
  const string to;
  ShiftOperation(const string &to):to(to){}
  bool operator==(const IOperation &op) const override {
    return op.type()==OperationType::Shift && to==dynamic_cast<const ShiftOperation&>(op).to;
  }
  bool operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const override;
};
class ConflictOperation : public IOperation{
public:
  virtual OperationType type() const override { return OperationType::Conflict; }
  ConflictOperation(const std::vector<string> &shift_to, const std::vector<int> &reduce_rule_id)
    :shift_to(shift_to), reduce_rule_id(reduce_rule_id){}
  ConflictOperation(const std::shared_ptr<IOperation> op, int arg_reduce_rule_id);
  std::vector<string> shift_to;
  std::vector<int> reduce_rule_id;
  bool operator==(const IOperation &op) const override {
    return op.type()==OperationType::Conflict && shift_to==dynamic_cast<const ConflictOperation&>(op).shift_to && reduce_rule_id==dynamic_cast<const ConflictOperation&>(op).reduce_rule_id;
  }
  bool operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const override;
};

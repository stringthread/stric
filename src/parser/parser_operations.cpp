#include "parser_operations.h"

ConflictOperation::ConflictOperation(const std::shared_ptr<IOperation> op, int arg_reduce_rule_id){
  switch(op->type()){
    case OperationType::Shift:
      shift_to.push_back(std::dynamic_pointer_cast<ShiftOperation>(op)->to);
      break;
    case OperationType::Reduce:
      reduce_rule_id.push_back(std::dynamic_pointer_cast<ReduceOperation>(op)->rule_id);
      break;
    case OperationType::Conflict:
      const auto casted_op=std::dynamic_pointer_cast<ConflictOperation>(op);
      shift_to=casted_op->shift_to;
      reduce_rule_id=casted_op->reduce_rule_id;
  }
  reduce_rule_id.push_back(arg_reduce_rule_id);
}

bool AcceptOperation::operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const {
  return true;
}

bool GotoOperation::operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const {
  state_stack.push(to);
  return false;
}

bool ReduceOperation::operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const {
  const auto &rule=parser.get_db()->get_rule(rule_id);
  for(size_t i=0;i<rule.get_ptn().size();i++) state_stack.pop();
  std::vector<AST_Node> children;
  children.reserve(rule.get_ptn().size());
  for(size_t i=0;i<rule.get_ptn().size();i++){
    children.push_back(result_stack.top());
    result_stack.pop();
  }
  std::reverse(children.begin(), children.end());
  result_stack.emplace(rule.get_l_token(), "", rule_id, std::move(children));
  const auto &state=state_stack.top();
  const auto &l_token=rule.get_l_token();
  if(parser.get_parsing_table().first.at(state).count(l_token)==0){
    std::cout << "unexpected token" << '\n';
    return true;
  }
  const auto &goto_op=parser.get_parsing_table().first.at(state).at(l_token);
  if(goto_op->type()!=OperationType::Goto){
    std::cout << "expected goto operation after reduce operation" << '\n';
    return true;
  }
  goto_op->operate(state_stack, result_stack, parser, input, input_index);
  return false;
}

bool ShiftOperation::operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const {
  state_stack.push(to);
  result_stack.push(input[input_index]);
  input_index++;
  return false;
}

bool ConflictOperation::operate(std::stack<string> &state_stack, std::stack<AST_Node> &result_stack, Parser& parser, std::vector<Token> input, size_t &input_index) const {
  std::cout << "conflict found" << '\n';
  return true;
}

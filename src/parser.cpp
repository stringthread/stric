#include "parser.h"
#include "parser_operations.h"

string AST_Node::str() const{
  std::ostringstream oss;
  oss << "{" << type << ", \"" << value << "\", [\n";
  for(const AST_Node &child : children){
    oss << child.str() << '\n';
  }
  oss << "] }" << '\n';
  return oss.str();
}

AST_Node Parser::_parse(const std::vector<Token> &input){
  size_t input_index=0;
  std::stack<string> state_stack;
  state_stack.push(parsing_table.second);
  std::stack<AST_Node> result_stack;
  bool flg_err=false;
  while(input_index < input.size()){
    const string &token=input[input_index].token;
    const string &state=state_stack.top();
    if(parsing_table.first.at(state).count(token)==0){
      std::cout << "unexpected token : [ " << state << " ][ " << token << " ]\n";
      flg_err=true;
      break;
    }
    if(parsing_table.first.at(state).at(token)->operate(state_stack, result_stack, *this, input, input_index)) break;
  }
  if(flg_err){
    std::cout << "parse failed" << '\n';
  }
  if(result_stack.size()!=1){
    std::cout << "invalid ast generated" << '\n';
  }
  return result_stack.top();
}

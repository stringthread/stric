#include "executor.h"
#include "object/object.h"
#include "object/operators.h"
#include "controls.h"

#include <stdexcept>

std::unordered_set<string> Executor::operator_symbols {
  "suffix_unary_operator",
  "prefix_unary_operator",
  "math_operator_1",
  "math_operator_2",
  "compare_operator",
  "bool_operator",
  "assign_operator",
};
std::unordered_set<string> Executor::operator_terminals{
  "PLUSPLUS", "PLUS", "MINUSMINUS", "MINUS", "ASTERISK", "DIV_INT", "SLASH",
  "ANDAND", "AND", "OROR", "OR", "HAT", "NOT", "EQUAL", "NOT_EQUAL",
  "LESS_EQUAL", "GREATER_EQUAL", "LEFT_ANGLE_BRACKET", "RIGHT_ANGLE_BRACKET",
  "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
  "LEFT_BRACKET", "RIGHT_BRACKET",
};
std::unordered_map<string, exec_func_t> Executor::control_exec{
  {"PLUS", OPERATORS::unary_plus},
  {"MINUS", OPERATORS::unary_minus},
  {"PLUSPLUS", OPERATORS::prefix_increment},
  {"MINUSMINUS", OPERATORS::prefix_decrement},
  {"NOT", OPERATORS::_not},
  {"LEFT_PAREN", OPERATORS::left_paren},
  {"IF", CONTROLS::_if},
  {"ELSE", CONTROLS::_if_else},
  {"ASSIGN", OPERATORS::assign},
  {"DOT", CONTROLS::dot},
  {"WHILE", CONTROLS::_while},
};
const string& Executor::get_tokenname_from_AST(const AST_Node &node){
  if(node.children.size()==1) return get_tokenname_from_AST(node.children[0]);
  return node.type;
}
Executor::Executor(){
  Object::init();
}
obj_ptr_t Executor::eval(const AST_Node &node){
  if(node.type=="main"){
    for(const auto &block : node.children){
      eval(block);
    }
  }
  else if(node.type=="block"){
    for(const auto &sentence : node.children){
      eval(sentence);
    }
  }
  else if(node.type=="sentence"){
    for(int i=0;i<node.children.size();i++){
      const auto result=eval(node.children[i]);
      if(result) std::cout << result->print() << std::endl;
    }
  }
  else if(node.type=="EOS"){
  }
  else if(node.type=="var_def"){
    if(node.children.size()!=2){
      throw std::runtime_error("invalid children size : var_def");
      return nullptr;
    }
    auto var=def_var(node.children[0].value, node.children[1].value);//only for simple type
    if(!var) std::cout << "null after def_var" << '\n';
    return var;
  }
  else if(operator_terminals.count(node.type)!=0){
    return Object::factories["OPERATORS"]->generate(node.type);
  }
  else{
    switch(node.children.size()){
      case 0:
        //if(node.type=="IDENTIFIER") return get_var(node.value);
        return Object::factories.at(node.type)->generate(node.value);
      case 1:
        return eval(node.children[0]);
      default:
        auto itr_control=std::find_if(node.children.cbegin(), node.children.cend(), [this](const AST_Node &i){
          return operator_symbols.count(i.type)!=0||control_exec.count(i.type)!=0;
        });
        if(itr_control==node.children.cend()){
          std::cout << "at evaluating :" << node.type << '\n';
          throw std::runtime_error("Node without control token...");
          return nullptr;
        }
        const string& type_control=get_tokenname_from_AST(*itr_control);
        size_t pos_control=std::distance(node.children.cbegin(),itr_control);
        if(operator_terminals.count(type_control)!=0&&pos_control!=0){
          std::vector<obj_ptr_t> v_obj;
          for(const auto &child : node.children){
            v_obj.push_back(eval(child));
          }
          return (v_obj[0]->operators().at(type_control).at(pos_control).at(node.children.size()))(v_obj);
        }
        return (control_exec.at(type_control))(this, node.children);
    }
  }
  return nullptr;
}
void Executor::add_exec(const string &token, exec_func_t func){
  control_exec[token]=func;
}
obj_ptr_t Executor::get_var(const string &name){
  if(vars.count(name)==0){
    throw std::invalid_argument("Getting undefined identifier : " + name);
  }
  return vars.at(name);
}
obj_ptr_t Executor::def_var(const string &type, const string &name, obj_ptr_t value){
  if(Object::factories.count(type)==0){
    throw std::runtime_error("invalid typename for defining var : "+type);
  }
  vars[name]=Object::factories.at(type)->cast(value);
  return vars[name];
}

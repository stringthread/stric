#include "operators.h"
#include <stdexcept>

std::shared_ptr<OPERATORS> OPERATORS::generate(const string &val){
  return std::shared_ptr<OPERATORS>(new OPERATORS(val));
}
void OPERATORS::init(){
  generators["OPERATORS"]=[](const string &val){
    return OPERATORS::generate(val);
  };
}
obj_ptr_t OPERATORS::unary_plus(Executor *p_exec, const std::vector<AST_Node>& args){
  if(args.size()!=2){
    throw std::invalid_argument("wrong length arguments for unary_plus");
  }
  std::vector<obj_ptr_t> v_obj;
  for(const auto &elem : args){
    v_obj.push_back(p_exec->eval(elem));
  }
  return (v_obj[1]->operators->at("PLUS").at(0).at(2))(v_obj);
}

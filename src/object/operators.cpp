#include "operators.h"
#include "../executor.h"
#include <stdexcept>

std::shared_ptr<OperatorsFactory> OPERATORS::factory=std::make_shared<OperatorsFactory>();
void OPERATORS::init(){
  factories["OPERATORS"]=factory;
}
obj_ptr_t OPERATORS::unary_plus(Executor *p_exec, const std::vector<AST_Node>& args){
  if(args.size()!=2){
    throw std::invalid_argument("wrong length arguments for unary_plus");
  }
  std::vector<obj_ptr_t> v_obj;
  for(const auto &elem : args){
    v_obj.push_back(p_exec->eval(elem));
  }
  return (v_obj[1]->operators().at("PLUS").at(0).at(2))(v_obj);
}
obj_ptr_t OPERATORS::unary_minus(Executor *p_exec, const std::vector<AST_Node>& args){
  if(args.size()!=2){
    throw std::invalid_argument("wrong length arguments for unary_plus");
  }
  std::vector<obj_ptr_t> v_obj;
  for(const auto &elem : args){
    v_obj.push_back(p_exec->eval(elem));
  }
  return (v_obj[1]->operators().at("MINUS").at(0).at(2))(v_obj);
}
obj_ptr_t OPERATORS::prefix_increment(Executor *p_exec, const std::vector<AST_Node>& args){
  if(args.size()!=2){
    throw std::invalid_argument("wrong length arguments for prefix_increment");
  }
  std::vector<obj_ptr_t> v_obj;
  for(const auto &elem : args){
    v_obj.push_back(p_exec->eval(elem));
  }
  return (v_obj[1]->operators().at("PLUSPLUS").at(0).at(2))(v_obj);
}
obj_ptr_t OPERATORS::prefix_decrement(Executor *p_exec, const std::vector<AST_Node>& args){
  if(args.size()!=2){
    throw std::invalid_argument("wrong length arguments for prefix_decrement");
  }
  std::vector<obj_ptr_t> v_obj;
  for(const auto &elem : args){
    v_obj.push_back(p_exec->eval(elem));
  }
  return (v_obj[1]->operators().at("MINUSMINUS").at(0).at(2))(v_obj);
}
obj_ptr_t OPERATORS::_not(Executor *p_exec, const std::vector<AST_Node>& args){
  if(args.size()!=2){
    throw std::invalid_argument("wrong length arguments for not");
  }
  std::vector<obj_ptr_t> v_obj;
  for(const auto &elem : args){
    v_obj.push_back(p_exec->eval(elem));
  }
  return (v_obj[1]->operators().at("NOT").at(0).at(2))(v_obj);
}
obj_ptr_t OPERATORS::left_paren(Executor *p_exec, const std::vector<AST_Node>& args){
  if(args.size()!=3){
    throw std::invalid_argument("wrong length arguments for parens");
  }
  return p_exec->eval(args[1]);
}
obj_ptr_t OPERATORS::assign(Executor *p_exec, const std::vector<AST_Node>& args){
  if(args.size()!=3){
    throw std::invalid_argument("wrong length arguments for assign");
  }
  auto assigned=p_exec->eval(args[0]);
  if(!assigned){
    throw std::runtime_error("cannot assign to null");
  }
  assigned->set(p_exec->eval(args[2]));
  return assigned;
}

obj_ptr_t OperatorsFactory::generate(const string& val){
  return std::shared_ptr<OPERATORS>(new OPERATORS(val));
}
obj_ptr_t OperatorsFactory::cast(obj_ptr_t obj){
  return nullptr;
}

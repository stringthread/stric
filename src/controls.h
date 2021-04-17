#pragma once

#include <stdexcept>
#include "executor.h"
#include "object/object.h"
#include "object/meta.h"
#include "object/bool.h"

namespace CONTROLS{
  std::unordered_set<string> stoppings{
    "RETURN","BREAK",
  };
  obj_ptr_t _if(Executor *p_exec, const std::vector<AST_Node>& args){
    if(args.size()!=3){
      throw std::invalid_argument("wrong length arguments for if");
    }
    BoolUtil bool_util;
    if(bool_util.obj2val_cast(p_exec->eval(args[1]))){
      p_exec->eval(args[2]);
    }
    return nullptr;
  }
  obj_ptr_t _if_else(Executor *p_exec, const std::vector<AST_Node>& args){
    if(args.size()!=3){
      throw std::invalid_argument("wrong length arguments for if-else");
    }
    if(args[0].type!="if_statement"){
      throw std::invalid_argument("not start with if : if-else");
    }
    BoolUtil bool_util;
    if(bool_util.obj2val_cast(p_exec->eval(args[0].children[1]))){
      p_exec->eval(args[0].children[2]);
    }else{
      p_exec->eval(args[2]);
    }
    return nullptr;
  }
  obj_ptr_t dot(Executor *p_exec, const std::vector<AST_Node>& args){
    if(args.size()!=2){
      throw std::invalid_argument("wrong length arguments for dot");
    }
    try{
      return p_exec->get_var(args[1].value);
    }catch(std::invalid_argument &e){
      throw std::invalid_argument(string("@CONTROLS::dot : ")+e.what());
    }
  }
  obj_ptr_t _while(Executor *p_exec, const std::vector<AST_Node>& args){
    if(args.size()!=3){
      throw std::invalid_argument("wrong length arguments for if");
    }
    BoolUtil bool_util;
    obj_ptr_t out;
    while(bool_util.obj2val_cast(p_exec->eval(args[1]))){
      out=p_exec->eval(args[2]);
      if(out && (stoppings.count(out->type())!=0||out->type()=="CONTINUE")){//break or return
        if(out->type()=="CONTINUE"){
          out=CONTINUE::util.obj_cast(out)->get_val();
          continue;
        }
        if(out->type()=="BREAK"){
          out=BREAK::util.obj_cast(out)->get_val();
        }
        return out;
      }
    }
    return out;
  }
  obj_ptr_t _return(Executor *p_exec, const std::vector<AST_Node>& args){
    int size=args.size();
    if(size!=2&&size!=3){
      throw std::invalid_argument("wrong length arguments for return");
    }
    obj_ptr_t out=RETURN::factory->generate_from_val(nullptr);
    if(size==3){
      out->set(p_exec->eval(args[1]));
    }
    return out;
  }
  obj_ptr_t _break(Executor *p_exec, const std::vector<AST_Node>& args){
    int size=args.size();
    if(size!=2&&size!=3){
      throw std::invalid_argument("wrong length arguments for return");
    }
    obj_ptr_t out=BREAK::factory->generate_from_val(nullptr);
    if(size==3){
      out->set(p_exec->eval(args[1]));
    }
    return out;
  }
  obj_ptr_t _continue(Executor *p_exec, const std::vector<AST_Node>& args){
    int size=args.size();
    if(size!=2&&size!=3){
      throw std::invalid_argument("wrong length arguments for return");
    }
    obj_ptr_t out=CONTINUE::factory->generate_from_val(nullptr);
    if(size==3){
      out->set(p_exec->eval(args[1]));
    }
    return out;
  }
}

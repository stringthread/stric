#include "executor.h"
#include "object/object.h"

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
    for(const auto &expr : node.children){
      const auto result=eval(expr);
      if(result) std::cout << result->print() << std::endl;
    }
  }
  else if(node.type!="EOS"){
    switch(node.children.size()){
      case 0:
        if(node.type=="IDENTIFIER") return get_var(node.value);
        return (Object::generators.at(node.type))(node.value);
      case 1:
        return eval(node.children[0]);
      default:
        auto args=node.children;
        auto itr_control=std::find_if(args.begin(), args.end(), [this](const AST_Node &i){
          return operator_exec.count(i.type)!=0;
        });
        if(itr_control==args.end()){
          throw std::runtime_error("Node without control token...");
          return nullptr;
        }
        string type_control=itr_control->type;
        args.erase(itr_control);
        return (operator_exec.at(type_control))(this, args);
    }
  }
  return nullptr;
}
void Executor::add_exec(const string &token, exec_func_t func){
  operator_exec[token]=func;
}
obj_ptr_t Executor::get_var(const string &name){
  try{
    return vars.at(name);
  }catch(std::out_of_range){
    std::cout << "Getting undefined identifier : " << name << '\n';
    throw;
  }
}
obj_ptr_t Executor::def_var(const string &type, const string &name){
  vars[name]=(Object::casters.at(type))(nullptr);
}
obj_ptr_t Executor::def_var(const string &type, const string &name, obj_ptr_t value){
  vars[name]=(Object::casters.at(name))(value);
}

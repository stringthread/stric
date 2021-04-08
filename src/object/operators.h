#pragma once

#include "object.h"
#include <unordered_map>
#include <functional>

class Executor;

class OperatorsFactory : public ObjectFactory{
  obj_ptr_t generate(const string& val) override;
  obj_ptr_t cast(obj_ptr_t obj) override;
};

class OPERATORS: public Object{
private:
  string _type;
  OPERATORS(const string &_type):_type(_type){}
  static std::shared_ptr<OperatorsFactory> factory;
public:
  friend OperatorsFactory;
  void set(obj_ptr_t new_val) override {}
  string type() const override { return _type; }
  op_func_map_t& operators() const override {
    static op_func_map_t op_func_def;
    return op_func_def;
  }
  static void init();
  static obj_ptr_t unary_plus(Executor *p_exec, const std::vector<AST_Node>& args);
  static obj_ptr_t _not(Executor *p_exec, const std::vector<AST_Node>& args);
};

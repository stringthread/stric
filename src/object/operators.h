#pragma once

#include "object.h"
#include <unordered_map>
#include <functional>

class Executor;
class OPERATORS: public Object{
private:
  string _type;
  OPERATORS(const string &_type):_type(_type){}
public:
  void set(obj_ptr_t new_val) override {}
  string type() const override { return _type; }
  op_func_map_t& operators() const override {
    static op_func_map_t op_func_def;
    return op_func_def;
  }
  static std::shared_ptr<OPERATORS> generate(const string &_type);
  static void init();
  static obj_ptr_t unary_plus(Executor *p_exec, const std::vector<AST_Node>& args);
  static obj_ptr_t _not(Executor *p_exec, const std::vector<AST_Node>& args);
};

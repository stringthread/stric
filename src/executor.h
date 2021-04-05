#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <stdexcept>

#include "object/object.h"
#include "parser/parser.h"
#include "parser/syntax.h"

using std::string;
class Executor;
using exec_func_t=std::function<obj_ptr_t(Executor*, const std::vector<AST_Node>&)>;

class Executor{
private:
  static std::unordered_set<string> operator_symbols; //non-terminates
  static std::unordered_set<string> operator_terminals; //terminals
  static std::unordered_map<string, exec_func_t> control_exec; //ControlTokenName->function
  std::unordered_map<string, obj_ptr_t> vars;
  void init();
public:
  Executor();
  obj_ptr_t eval(const AST_Node &node);
  void add_exec(const string &token, exec_func_t func);
  obj_ptr_t get_var(const string &name);
  obj_ptr_t def_var(const string &type, const string &name);
  obj_ptr_t def_var(const string &type, const string &name, obj_ptr_t value);
};

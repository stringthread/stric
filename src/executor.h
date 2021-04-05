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
using exec_func_t=std::function<obj_ptr_t(Executor*, std::vector<AST_Node>&)>;

class Executor{
private:
  std::unordered_map<string, exec_func_t> operator_exec; //TokenName->function
  std::unordered_map<string, obj_ptr_t> vars;
public:
  Executor()=default;
  Executor(std::unordered_map<string, exec_func_t>&& exec_map):operator_exec(exec_map){}
  obj_ptr_t eval(const AST_Node &node);
  void add_exec(const string &token, exec_func_t func);
  obj_ptr_t get_var(const string &name);
  obj_ptr_t def_var(const string &type, const string &name);
  obj_ptr_t def_var(const string &type, const string &name, obj_ptr_t value);
};

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "../parser/parser.h"
#include "../parser/syntax.h"

using std::string;
class Object;
using obj_ptr_t=std::shared_ptr<Object>;
using op_func_t=std::function<obj_ptr_t(std::vector<obj_ptr_t>)>;
using op_func_map_1_t=std::unordered_map<int,op_func_t>;
using op_func_map_2_t=std::unordered_map<int,op_func_map_1_t>;
using op_func_map_t=std::unordered_map<string,op_func_map_2_t>;

class Object{
public:
  static std::unordered_map<string, std::function<obj_ptr_t(const string&)>> generators;
  static std::unordered_map<string, std::function<obj_ptr_t(obj_ptr_t)>> casters;
  std::shared_ptr<op_func_map_t> operators; //[operator token][op_pos(0-)][size]->fn
  static void init();
  static bool is_obj(const string &token);
  virtual string type() const=0;
  virtual void set(obj_ptr_t new_val)=0;
  virtual string print() const {return "";}
};

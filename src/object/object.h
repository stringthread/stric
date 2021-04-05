#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "../parser/parser.h"
#include "../parser/syntax.h"

using std::string;
class Object;
using obj_ptr_t=std::shared_ptr<Object>;

class Object{
public:
  static std::unordered_map<string, std::function<obj_ptr_t(const string&)>> generators;
  static std::unordered_map<string, std::function<obj_ptr_t(obj_ptr_t)>> casters;
  virtual string type() const=0;
  virtual void set(obj_ptr_t new_val)=0;
  virtual string print() const {return "";}
};

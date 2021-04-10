#pragma once

#include "value_object.h"

class IntUtil : public ValueObjectUtil<int>{
private:
  int get_default() const override { return 0; };
  int _obj2val_cast(obj_ptr_t obj) const override;
  static std::unordered_map<string, std::function<int(obj_ptr_t)>> cast_fn;//from->fn
public:
  int val_cast(const string& val) const override;
  string to_str(const int& val) const override;
};

class INT : public ValueObject<int>{
private:
  INT(int val) : ValueObject<int>(val,util){}
  static IntUtil util;
  static std::shared_ptr<ValueObjectFactory<INT, int>> factory;
  static op_func_map_t op_func_def;
public:
  friend ValueObjectFactory<INT, int>;
  string type() const override{ return "INT"; }
  op_func_map_t& operators() const override {
    return op_func_def;
  }
  string print() const override{ return std::to_string(val); }
  static void init();
};

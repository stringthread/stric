#pragma once

#include "value_object.h"

class IntUtil : public ValueObjectUtil<int>{
private:
  int get_default() const override { return 0; };
  static std::unordered_map<string, std::function<std::shared_ptr<ValueObject<int>>(obj_ptr_t)>> cast_fn;//from->fn
public:
  std::shared_ptr<ValueObject<int>> obj_cast(obj_ptr_t obj) const override;
  int val_cast(const string& val) const override;
  string to_str(const int& val) const override;
};

class INT : public ValueObject<int>{
private:
  INT(int val) : ValueObject<int>(val,util){}
  static op_func_map_t op_func_def;
public:
  friend ValueObjectFactory<INT, int>;
  static IntUtil util;
  static std::shared_ptr<ValueObjectFactory<INT, int>> factory;
  string type() const override{ return "INT"; }
  op_func_map_t& operators() const override {
    return op_func_def;
  }
  string print() const override{ return std::to_string(val); }
  static void init();
};

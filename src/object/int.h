#pragma once

#include "value_object.h"

class INT : public ValueObject<int>{
private:
  INT(int val) : ValueObject<int>(val){}
  static std::unordered_map<string, std::function<int(obj_ptr_t)>> cast_fn;//from->fn
  static op_func_map_t op_func_def;
  static int obj2val_cast(obj_ptr_t obj);
  static int val_cast(const string& val);
  static obj_ptr_t generate(const string &val);
  static obj_ptr_t cast(obj_ptr_t obj);
public:
  string type() const override{ return "INT"; }
  std::unordered_map<string, std::function<int(obj_ptr_t)>>& get_cast_fn() const override {
    return cast_fn;
  }
  void set(obj_ptr_t new_val) override;
  void set(int new_val) override;
  op_func_map_t& operators() const override {
    return op_func_def;
  }
  string print() const override{ return std::to_string(val); }
  static void init();
};

#pragma once

#include "value_object.h"

class BOOL : public ValueObject<bool>{
private:
  BOOL(bool val) : ValueObject<bool>(val){}
  static std::unordered_map<string, std::function<bool(obj_ptr_t)>> cast_fn;
  static op_func_map_t op_func_def;
  static bool obj2val_cast(obj_ptr_t obj);
  static bool val_cast(const string& val);
  static string bool2str(bool b);
  static obj_ptr_t generate(const string &val);
  static obj_ptr_t generate_from_bool(bool val);
  static obj_ptr_t cast(obj_ptr_t obj);
public:
  string type() const override{ return "BOOL"; }
  void set(obj_ptr_t new_val) override;
  void set(bool new_val) override;
  std::unordered_map<string, std::function<bool(obj_ptr_t)>>& get_cast_fn() const override { return cast_fn; }
  op_func_map_t& operators() const override { return op_func_def; }
  string print() const override { return val?"@t":"@f"; }
  static void init();
};

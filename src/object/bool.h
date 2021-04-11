#pragma once

#include "value_object.h"

class BoolUtil : public ValueObjectUtil<bool>{
private:
  bool get_default() const override { return false; };
  static std::unordered_map<string, std::function<std::shared_ptr<ValueObject<bool>>(obj_ptr_t)>> cast_fn;
public:
  std::shared_ptr<ValueObject<bool>> obj_cast(obj_ptr_t obj) const override;
  bool val_cast(const string&) const override;
  string to_str(const bool&) const override;
};

class BOOL : public ValueObject<bool>{
private:
  BOOL(bool val) : ValueObject<bool>(val, util){}
  static BoolUtil util;
  static std::shared_ptr<ValueObjectFactory<BOOL, bool>> factory;
  static op_func_map_t op_func_def;
public:
  friend ValueObjectFactory<BOOL, bool>;
  string type() const override{ return "BOOL"; }
  op_func_map_t& operators() const override { return op_func_def; }
  string print() const override { return val?"@t":"@f"; }
  static void init();
};

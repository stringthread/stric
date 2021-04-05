#pragma once

#include "object.h"
#include <unordered_map>
#include <functional>

//template<class T>
class INT: public Object{
private:
  int val;
  static const std::unordered_map<string, std::function<int(obj_ptr_t)>> cast_fn;
  static int val_cast(obj_ptr_t obj);
  INT(int val):val(val){}
public:
  string type() const override{ return "INT"; };
  void set(obj_ptr_t new_val) override;
  int get_val() const { return val; }
  string print() const override{ return std::to_string(val); }
  static std::shared_ptr<INT> generate(const string &val);
  static std::shared_ptr<INT> cast(obj_ptr_t obj);
};

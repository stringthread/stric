#pragma once

#include "object.h"
#include <unordered_map>
#include <functional>

//template<class T>
class OPERATORS: public Object{
private:
  string _type;
  OPERATORS(const string &_type):_type(_type){}
public:
  void set(obj_ptr_t new_val) override {}
  string type() const override { return _type; };
  static std::shared_ptr<OPERATORS> generate(const string &_type);
  static void init();
};

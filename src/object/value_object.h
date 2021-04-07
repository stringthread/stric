#pragma once

#include "object.h"
#include <unordered_map>
#include <functional>

template<typename T>
class ValueObject : public Object{
protected:
  T val;
  ValueObject(T val):val(val){}
  //Derived(T val):ValueObject<T>(val){}
  //static std::unordered_map<string, std::function<int(obj_ptr_t)>> Derived::cast_fn;//from->fn
  //static op_func_map_t Derived::op_func_def;//can be empty but needed
  //static obj_ptr_t Derived::generate(const string &val);
  //static obj_ptr_t Derived::cast(obj_ptr_t obj);
  static void _init(const string &type,
    std::function<obj_ptr_t(const string)> generate,
    std::function<obj_ptr_t(obj_ptr_t)> cast);
  //public: static Derived::init(){_init("TOKEN_NAME",generate,cast)}
public:
  virtual string type() const override{ return "ValueObject"; };//should be overriden
  //void Derived::set(obj_ptr_t new_val) override;
  virtual void set(T new_val)=0;
  T get_val() const { return val; }
  virtual std::unordered_map<string, std::function<T(obj_ptr_t)>>& get_cast_fn() const=0;
  //op_func_map_t& Derived::operators() const override;
  //string Derived::print() const override;
};

template<typename T>
void ValueObject<T>::_init(const string& type,
  std::function<obj_ptr_t(const string)> generate,
  std::function<obj_ptr_t(obj_ptr_t)> cast)
{
  generators[type]=generate;
  casters[type]=cast;
}

#pragma once

#include "object.h"
#include <unordered_map>
#include <functional>
#include <type_traits>

template<typename T>
class ValueObject;

//abstract
template<typename T>
class ValueObjectUtil{
protected:
  virtual T get_default() const=0;
public:
  virtual std::shared_ptr<ValueObject<T>> obj_cast(obj_ptr_t) const=0;
  virtual T obj2val_cast(obj_ptr_t obj) const{
    std::shared_ptr<ValueObject<T>> out=obj_cast(obj);
    return out?out->get_val():get_default();
  }
  virtual T val_cast(const string&) const=0;
  virtual string to_str(const T&) const=0;
};

//abstract
template<typename T>
class ValueObject : public Object{
protected:
  T val;
  const ValueObjectUtil<T> &util;
  ValueObject(T val, const ValueObjectUtil<T> &util):val(val),util(util){}
  static void _init(const string &type, std::shared_ptr<ObjectFactory> factory){
    factories[type]=factory;
  }
  //static op_func_map_t Derived::op_func_def;//can be empty but needed
public:
  virtual string type() const override{ return "ValueObject"; };//should be overriden
  virtual void set(obj_ptr_t new_val) override { val=util.obj2val_cast(new_val); }
  template<typename std::enable_if<!(std::is_same<T, obj_ptr_t>::value)>* =nullptr>//prevent overload failure when T=obj_ptr_t
  void set(T new_val) { val=new_val; }//custom with operator=() overload
  T get_val() const { return val; }
  //op_func_map_t& Derived::operators() const override { return op_func_def; }
  //string Derived::print() const override;
};

//not abstract
template<class ObjT, typename ValT>
class ValueObjectFactory:public ObjectFactory{
protected:
  const ValueObjectUtil<ValT>& util;
public:
  ValueObjectFactory(const ValueObjectUtil<ValT>& util):util(util){}
  virtual obj_ptr_t generate_from_val(const ValT& val);
  virtual obj_ptr_t generate(const string& val) override;
  virtual obj_ptr_t cast(obj_ptr_t obj) override;
};

template<class ObjT, typename ValT>
obj_ptr_t ValueObjectFactory<ObjT, ValT>::generate_from_val(const ValT& val){
  return generate(util.to_str(val));
}
template<class ObjT, typename ValT>
obj_ptr_t ValueObjectFactory<ObjT, ValT>::generate(const string& val){
  return obj_ptr_t(new ObjT(util.val_cast(val)));
}
template<class ObjT, typename ValT>
obj_ptr_t ValueObjectFactory<ObjT, ValT>::cast(obj_ptr_t obj){
  return obj_ptr_t(new ObjT(util.obj2val_cast(obj)));
}

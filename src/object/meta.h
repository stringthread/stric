#pragma once

#include "value_object.h"

//abstract
class MetaObjectUtil : public ValueObjectUtil<obj_ptr_t>{
protected:
  obj_ptr_t get_default() const override { return nullptr; }
public:
  //std::shared_ptr<ValueObject<obj_ptr_t>> obj_cast(obj_ptr_t) const override;
  obj_ptr_t val_cast(const string& str) const override{ return get_default(); } //dummy params & return
  string to_str(const obj_ptr_t& obj) const override{ return ""; } //dummy params & return
};

template<class T>
class MetaObjectFactory : public ValueObjectFactory<T, obj_ptr_t>{
public:
  MetaObjectFactory(const MetaObjectUtil& util) : ValueObjectFactory<T, obj_ptr_t>(util){}
  obj_ptr_t generate_from_val(const obj_ptr_t& val) override{ return obj_ptr_t(new T(val)); }
  obj_ptr_t generate(const string& val) override{ return generate_from_val(this->util.val_cast(val)); }//MetaObject(nullptr)
};

//abstract
class MetaObject : public ValueObject<obj_ptr_t> {
protected:
  MetaObject(obj_ptr_t obj):ValueObject<obj_ptr_t>(obj, util){}
public:
  op_func_map_t& operators() const override {
    static op_func_map_t op_func_def;
    return op_func_def;
  } //return empty
  void set(obj_ptr_t new_val) override { val=new_val; }
  //string type() const override
  string print() const override { return type()+" : "+(val?val->print():""); }
};

class ReturnUtil : public MetaObjectUtil{
private:
  static std::unordered_map<string, std::function<std::shared_ptr<ValueObject<obj_ptr_t>>(obj_ptr_t)>> cast_fn;
public:
  std::shared_ptr<ValueObject<obj_ptr_t>> obj_cast(obj_ptr_t obj) const override;
};

class RETURN : public MetaObject {
private:
  RETURN(obj_ptr_t obj):MetaObject(obj){}
public:
  friend ValueObjectFactory<RETURN,obj_ptr_t>;
  friend MetaObjectFactory<RETURN>;
  static ReturnUtil util;
  static std::shared_ptr<MetaObjectFactory<RETURN>> factory;
  string type() const override { return "RETURN"; }
  static void init();
};

class BreakUtil : public MetaObjectUtil{
private:
  static std::unordered_map<string, std::function<std::shared_ptr<ValueObject<obj_ptr_t>>(obj_ptr_t)>> cast_fn;
public:
  std::shared_ptr<ValueObject<obj_ptr_t>> obj_cast(obj_ptr_t obj) const override;
};

class BREAK : public MetaObject {
private:
  BREAK(obj_ptr_t obj):MetaObject(obj){}
public:
  friend ValueObjectFactory<BREAK,obj_ptr_t>;
  friend MetaObjectFactory<BREAK>;
  static BreakUtil util;
  static std::shared_ptr<MetaObjectFactory<BREAK>> factory;
  string type() const override { return "BREAK"; }
  static void init();
};

class ContinueUtil : public MetaObjectUtil{
private:
  static std::unordered_map<string, std::function<std::shared_ptr<ValueObject<obj_ptr_t>>(obj_ptr_t)>> cast_fn;
public:
  std::shared_ptr<ValueObject<obj_ptr_t>> obj_cast(obj_ptr_t obj) const override;
};

class CONTINUE : public MetaObject {
private:
  CONTINUE(obj_ptr_t obj):MetaObject(obj){}
public:
  friend ValueObjectFactory<CONTINUE,obj_ptr_t>;
  friend MetaObjectFactory<CONTINUE>;
  static ContinueUtil util;
  static std::shared_ptr<MetaObjectFactory<CONTINUE>> factory;
  string type() const override { return "CONTINUE"; }
  static void init();
};

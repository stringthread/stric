#include "meta.h"

std::unordered_map<string, std::function<std::shared_ptr<ValueObject<obj_ptr_t>>(obj_ptr_t)>> ReturnUtil::cast_fn {
  {"RETURN", [](obj_ptr_t obj){return std::static_pointer_cast<RETURN>(obj);}},
};
std::shared_ptr<ValueObject<obj_ptr_t>> ReturnUtil::obj_cast(obj_ptr_t obj) const {
  if(!obj) return nullptr;
  auto implicit_cast=std::static_pointer_cast<RETURN>(obj);
  if(implicit_cast) return implicit_cast;
  if(cast_fn.count(obj->type())==0){
    throw std::runtime_error("Cast failed to RETURN : "+obj->type());
  }
  return (cast_fn.at(obj->type()))(obj);
}

ReturnUtil RETURN::util;
std::shared_ptr<MetaObjectFactory<RETURN>> RETURN::factory=std::make_shared<MetaObjectFactory<RETURN>>(util);
void RETURN::init(){
  factories["RETURN"]=factory;
}

std::unordered_map<string, std::function<std::shared_ptr<ValueObject<obj_ptr_t>>(obj_ptr_t)>> BreakUtil::cast_fn {
  {"BREAK", [](obj_ptr_t obj){return std::static_pointer_cast<BREAK>(obj);}},
};
std::shared_ptr<ValueObject<obj_ptr_t>> BreakUtil::obj_cast(obj_ptr_t obj) const {
  if(!obj) return nullptr;
  auto implicit_cast=std::static_pointer_cast<BREAK>(obj);
  if(implicit_cast) return implicit_cast;
  if(cast_fn.count(obj->type())==0){
    throw std::runtime_error("Cast failed to BREAK : "+obj->type());
  }
  return (cast_fn.at(obj->type()))(obj);
}

BreakUtil BREAK::util;
std::shared_ptr<MetaObjectFactory<BREAK>> BREAK::factory=std::make_shared<MetaObjectFactory<BREAK>>(util);
void BREAK::init(){
  factories["BREAK"]=factory;
}

std::unordered_map<string, std::function<std::shared_ptr<ValueObject<obj_ptr_t>>(obj_ptr_t)>> ContinueUtil::cast_fn {
  {"CONTINUE", [](obj_ptr_t obj){return std::static_pointer_cast<CONTINUE>(obj);}},
};
std::shared_ptr<ValueObject<obj_ptr_t>> ContinueUtil::obj_cast(obj_ptr_t obj) const {
  if(!obj) return nullptr;
  auto implicit_cast=std::static_pointer_cast<CONTINUE>(obj);
  if(implicit_cast) return implicit_cast;
  if(cast_fn.count(obj->type())==0){
    throw std::runtime_error("Cast failed to CONTINUE : "+obj->type());
  }
  return (cast_fn.at(obj->type()))(obj);
}

ContinueUtil CONTINUE::util;
std::shared_ptr<MetaObjectFactory<CONTINUE>> CONTINUE::factory=std::make_shared<MetaObjectFactory<CONTINUE>>(util);
void CONTINUE::init(){
  factories["CONTINUE"]=factory;
}

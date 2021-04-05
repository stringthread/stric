#include "int.h"

int INT::val_cast(obj_ptr_t obj){
  if(cast_fn.count(obj->type())==0){
    throw std::runtime_error("Cast failed to INT : "+obj->type());
    return 0;
  }
  return (cast_fn.at(obj->type()))(obj);
}
void INT::set(obj_ptr_t new_val){
  val=val_cast(new_val);
}
std::shared_ptr<INT> INT::generate(const string &val){
  return std::shared_ptr<INT>(new INT(std::stoi(val)));
}
std::shared_ptr<INT> INT::cast(obj_ptr_t obj){
  return std::shared_ptr<INT>(new INT(val_cast(obj)));
}
const std::unordered_map<string, std::function<int(obj_ptr_t)>> INT::cast_fn {
  {"INT", [](obj_ptr_t obj){return std::static_pointer_cast<INT>(obj)->val;}},
};

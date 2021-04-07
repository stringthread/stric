#include "object.h"
#include "int.h"
#include "bool.h"
#include "operators.h"

std::unordered_map<string, std::function<obj_ptr_t(const string&)>> Object::generators;
std::unordered_map<string, std::function<obj_ptr_t(obj_ptr_t)>> Object::casters;
void Object::init(){
  INT::init();
  BOOL::init();
  OPERATORS::init();
}
bool Object::is_obj(const string &token){
  return generators.count(token)!=0;
}

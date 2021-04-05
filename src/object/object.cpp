#include "object.h"
#include "int.h"

std::unordered_map<string, std::function<obj_ptr_t(const string&)>> Object::generators {
  {"INT", [](const string &val){return INT::generate(val);}},
};
std::unordered_map<string, std::function<obj_ptr_t(obj_ptr_t)>> Object::casters {
  {"INT", [](obj_ptr_t obj){return INT::cast(obj);}},
};

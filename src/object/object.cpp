#include "object.h"
#include "meta.h"
#include "int.h"
#include "bool.h"
#include "operators.h"

std::unordered_map<string, std::shared_ptr<ObjectFactory>> Object::factories;
void Object::init(){
  RETURN::init();
  BREAK::init();
  CONTINUE::init();
  INT::init();
  BOOL::init();
  OPERATORS::init();
}
bool Object::is_obj(const string &token){
  return factories.count(token)!=0;
}

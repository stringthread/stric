#include "operators.h"

std::shared_ptr<OPERATORS> OPERATORS::generate(const string &val){
  return std::shared_ptr<OPERATORS>(new OPERATORS(val));
}
void OPERATORS::init(){
  generators["OPERATORS"]=[](const string &val){
    return OPERATORS::generate(val);
  };
}

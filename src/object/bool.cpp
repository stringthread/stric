#include "bool.h"

std::unordered_map<string, std::function<bool(obj_ptr_t)>> BoolUtil::cast_fn {
  {"BOOL", [](obj_ptr_t obj){return std::static_pointer_cast<BOOL>(obj)->get_val();}},
};
bool BoolUtil::obj2val_cast(obj_ptr_t obj) const {
  if(obj->type()=="BOOL") return std::static_pointer_cast<BOOL>(obj)->get_val();
  if(cast_fn.count(obj->type())==0){
    throw std::runtime_error("Cast failed to BOOL : "+obj->type());
    return 0;
  }
  return (cast_fn.at(obj->type()))(obj);
}
bool BoolUtil::val_cast(const string& val) const {
  return val!="" && val!="@f";
}
string BoolUtil::to_str(const bool& b) const { return b?"@t":"@f"; }

BoolUtil BOOL::util;
std::shared_ptr<ValueObjectFactory<BOOL, bool>> BOOL::factory=std::make_shared<ValueObjectFactory<BOOL, bool>>(BOOL::util);
op_func_map_t BOOL::op_func_def={
  {"NOT",
    op_func_map_2_t({
      {0, op_func_map_1_t({
        {2,
          [](std::vector<obj_ptr_t> v_obj){
            return factory->generate_from_val(!util.obj2val_cast(v_obj[1]));
          }
        },
      })},
    }),
  },
};
void BOOL::init(){
  _init("TRUE", factory);
  _init("FALSE", factory);
}

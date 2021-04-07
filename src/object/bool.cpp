#include "bool.h"

std::unordered_map<string, std::function<bool(obj_ptr_t)>> BOOL::cast_fn {
  {"BOOL", [](obj_ptr_t obj){return std::static_pointer_cast<BOOL>(obj)->val;}},
};

bool BOOL::obj2val_cast(obj_ptr_t obj){
  if(obj->type()=="BOOL") return std::static_pointer_cast<BOOL>(obj)->get_val();
  if(cast_fn.count(obj->type())==0){
    throw std::runtime_error("Cast failed to BOOL : "+obj->type());
    return 0;
  }
  return (cast_fn.at(obj->type()))(obj);
}
bool BOOL::val_cast(const string& val){
  return val!="" && val!="@f";
}
string BOOL::bool2str(bool b){ return b?"@t":"@f"; }
obj_ptr_t BOOL::generate(const string &val){
  return obj_ptr_t(new BOOL(val_cast(val)));
}
obj_ptr_t BOOL::generate_from_bool(bool val){
  return generate(bool2str(val));
}
obj_ptr_t BOOL::cast(obj_ptr_t obj){
  return obj_ptr_t(new BOOL(obj2val_cast(obj)));
}
void BOOL::set(obj_ptr_t new_val){
  val=obj2val_cast(new_val);
}
void BOOL::set(bool new_val){
  val=new_val;
}
op_func_map_t BOOL::op_func_def={
  {"NOT",
    op_func_map_2_t({
      {0, op_func_map_1_t({
        {2,
          [](std::vector<obj_ptr_t> v_obj){
            return generate_from_bool(!obj2val_cast(v_obj[1]));
          }
        },
      })},
    }),
  },
};
void BOOL::init(){
  _init("TRUE", generate, cast);
  _init("FALSE", generate, cast);
}

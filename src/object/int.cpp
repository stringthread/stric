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
void INT::set(int new_val){
  val=new_val;
}
std::shared_ptr<INT> INT::generate(const string &val){
  return std::shared_ptr<INT>(new INT(std::stoi(val)));
}
std::shared_ptr<INT> INT::cast(obj_ptr_t obj){
  return std::shared_ptr<INT>(new INT(val_cast(obj)));
}
std::unordered_map<string, std::function<int(obj_ptr_t)>> INT::cast_fn {
  {"INT", [](obj_ptr_t obj){return std::static_pointer_cast<INT>(obj)->val;}},
};
op_func_map_t INT::op_func_def {
  {"PLUS",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return INT::generate(std::to_string(std::dynamic_pointer_cast<INT>(v_obj[0])->get_val()+std::dynamic_pointer_cast<INT>(v_obj[2])->get_val()));
          }
        },
      })},
      {0, op_func_map_1_t({
        {2,
          [](std::vector<obj_ptr_t> v_obj){
            return INT::cast(v_obj[1]);
          }
        },
      })},
    }),
  },
};
void INT::init(){
  generators["INT"]=[](const string &val){
    return INT::generate(val);
  };
  casters["INT"]=[](obj_ptr_t obj){
    return INT::cast(obj);
  };
}

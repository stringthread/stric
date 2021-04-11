#include "int.h"
#include "bool.h"

std::unordered_map<string, std::function<std::shared_ptr<ValueObject<int>>(obj_ptr_t)>> IntUtil::cast_fn {
  {"INT", [](obj_ptr_t obj){return std::static_pointer_cast<INT>(obj);}},
};
std::shared_ptr<ValueObject<int>> IntUtil::obj_cast(obj_ptr_t obj) const {
  if(!obj) return nullptr;
  auto implicit_cast=std::static_pointer_cast<INT>(obj);
  if(implicit_cast) return implicit_cast;
  if(cast_fn.count(obj->type())==0){
    throw std::runtime_error("Cast failed to INT : "+obj->type());
  }
  return (cast_fn.at(obj->type()))(obj);
}
int IntUtil::val_cast(const string& val) const {
  return std::stoi(val);
}
string IntUtil::to_str(const int& val) const { return std::to_string(val); }

IntUtil INT::util;
std::shared_ptr<ValueObjectFactory<INT, int>> INT::factory=std::make_shared<ValueObjectFactory<INT, int>>(INT::util);
op_func_map_t INT::op_func_def={
  {"PLUS",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()+util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
      {0, op_func_map_1_t({
        {2,
          [](std::vector<obj_ptr_t> v_obj){
            return factory->cast(v_obj[1]);
          }
        },
      })},
    }),
  },
  {"PLUSPLUS",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {2,
          [](std::vector<obj_ptr_t> v_obj){//suffix
            int ret_val=util.obj2val_cast(v_obj[0]);
            util.obj_cast(v_obj[0])->set(ret_val+1);
            return factory->generate_from_val(ret_val);
          }
        },
      })},
      {0, op_func_map_1_t({
        {2,
          [](std::vector<obj_ptr_t> v_obj){//prefix
            int ret_val=util.obj2val_cast(v_obj[1]);
            util.obj_cast(v_obj[1])->set(ret_val+1);
            return factory->generate_from_val(ret_val+1);
          }
        },
      })},
    }),
  },
  {"MINUS",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()-util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
      {0, op_func_map_1_t({
        {2,
          [](std::vector<obj_ptr_t> v_obj){
            return factory->generate_from_val(-util.obj2val_cast(v_obj[1]));
          }
        },
      })},
    }),
  },
  {"MINUSMINUS",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {2,
          [](std::vector<obj_ptr_t> v_obj){//suffix
            int ret_val=util.obj2val_cast(v_obj[1]);
            util.obj_cast(v_obj[1])->set(ret_val-1);
            return factory->generate_from_val(ret_val);
          }
        },
      })},
      {0, op_func_map_1_t({
        {2,
          [](std::vector<obj_ptr_t> v_obj){//prefix
            int ret_val=util.obj2val_cast(v_obj[1]);
            util.obj_cast(v_obj[1])->set(ret_val-1);
            return factory->generate_from_val(ret_val-1);
          }
        },
      })},
    }),
  },
  {"ASTERISK",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()*util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
    }),
  },
  {"DIV_INT",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()/util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
    }),
  },
  {"EQUAL",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return BOOL::factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()==util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
    }),
  },
  {"NOT_EQUAL",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return BOOL::factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()!=util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
    }),
  },
  {"LEFT_ANGLE_BRACKET",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return BOOL::factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()<util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
    }),
  },
  {"RIGHT_ANGLE_BRACKET",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return BOOL::factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()>util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
    }),
  },
  {"LESS_EQUAL",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return BOOL::factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()<=util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
    }),
  },
  {"GREATER_EQUAL",
    op_func_map_2_t({
      {1, op_func_map_1_t({
        {3,
          [](std::vector<obj_ptr_t> v_obj){
            return BOOL::factory->generate_from_val(util.obj_cast(v_obj[0])->get_val()>=util.obj_cast(v_obj[2])->get_val());
          }
        },
      })},
    }),
  },
};
void INT::init(){
  _init("INT", factory);
}

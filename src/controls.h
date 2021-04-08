#include <stdexcept>
#include "executor.h"
#include "object/object.h"
#include "object/bool.h"

namespace CONTROLS{
  obj_ptr_t _if(Executor *p_exec, const std::vector<AST_Node>& args){
    if(args.size()!=3){
      throw std::invalid_argument("wrong length arguments for if");
    }
    BoolUtil bool_util;
    if(bool_util.obj2val_cast(p_exec->eval(args[1]))){
      p_exec->eval(args[2]);
    }
    return nullptr;
  }
}

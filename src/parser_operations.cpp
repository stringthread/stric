#include "parser_operations.h"

ConflictOperation::ConflictOperation(const std::shared_ptr<IOperation> op, int arg_reduce_rule_id){
  switch(op->type()){
    case OperationType::Shift:
      shift_to.push_back(std::dynamic_pointer_cast<ShiftOperation>(op)->to);
      break;
    case OperationType::Reduce:
      reduce_rule_id.push_back(std::dynamic_pointer_cast<ReduceOperation>(op)->rule_id);
      break;
    case OperationType::Conflict:
      const auto casted_op=std::dynamic_pointer_cast<ConflictOperation>(op);
      shift_to=casted_op->shift_to;
      reduce_rule_id=casted_op->reduce_rule_id;
  }
  reduce_rule_id.push_back(arg_reduce_rule_id);
}

#include "parser_gen.h"

ConflictOperation::ConflictOperation(const IOperation &op, int arg_reduce_rule_id){
  switch(op.type()){
    case OperationType::Shift:
      shift_to.push_back(dynamic_cast<const ShiftOperation&>(op).to);
      break;
    case OperationType::Reduce:
      reduce_rule_id.push_back(dynamic_cast<const ReduceOperation&>(op).rule_id);
      break;
    case OperationType::Conflict:
      const ConflictOperation casted_op=dynamic_cast<const ConflictOperation&>(op);
      shift_to=casted_op.shift_to;
      reduce_rule_id=casted_op.reduce_rule_id;
  }
  reduce_rule_id.push_back(arg_reduce_rule_id);
}

ParserGenerator::ParserGenerator(const std::vector<SyntaxDef> &syntax_rules)
  :db(std::make_shared<SyntaxDB>(syntax_rules)), dfa_gen(db)
{
  init_table();
}

std::pair<ParsingTable, bool> ParserGenerator::generate_parsing_table(std::vector<std::shared_ptr<DFA_Node>> dfa){
  ParsingTable tmp_table;
  bool flg_conflicted=false;
  for(const auto &node_ptr : dfa){
    ParsingTableRow table_row;
    for(const auto &pair_edge : node_ptr->edge){
      string item_label=pair_edge.first;
      if(db->get_sd().is_terminal(item_label)){
        table_row[item_label]=ShiftOperation(pair_edge.second.lock()->cs.get_lr1_hash());
      }else if(db->get_sd().is_non_terminal(item_label)){
        table_row[item_label]=GotoOperation(pair_edge.second.lock()->cs.get_lr1_hash());
      }
    }
    for(const auto &item : node_ptr->cs.get_closure()){
      if(item.get_dot_index()!=db->get_rule(item.get_rule_id()).get_ptn().size()) continue;
      if(item.get_rule_id()==-1){
        table_row[EOF_SYMBOL]=AcceptOperation();
        continue;
      }
      for(const auto &la_label : item.get_look_aheads()){
        if (table_row.count(la_label)==0) {
          //if not Conflicted
          table_row[la_label]=ReduceOperation(item.get_rule_id());
          continue;
        }
        //if conflicted
        flg_conflicted=true;
        table_row[la_label]=ConflictOperation(table_row[la_label], item.get_rule_id());
      }
    }
    tmp_table[node_ptr->cs.get_lr1_hash()]=std::move(table_row);
  }
  return std::pair<ParsingTable, bool>(tmp_table, flg_conflicted);
}

void ParserGenerator::init_table(){
  auto result=generate_parsing_table(dfa_gen.get_lalr_dfa());
  if(result.second){
    parsing_table=result.first;
    table_type=TableType::LALR1;
    return;
  }
  result=generate_parsing_table(dfa_gen.get_lr_dfa());
  parsing_table=result.first;
  table_type=TableType::LR1;
  if(!result.second){
    table_type=TableType::CONFLICT;
  }
}

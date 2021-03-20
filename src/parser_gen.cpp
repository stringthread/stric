#include "parser_gen.h"

SymbolDiscriminator::SymbolDiscriminator(const std::vector<SyntaxDef> &syntax_rules){
  terminals.clear();
  non_terminals.clear();
  for(const auto &def : syntax_rules){
    non_terminals.insert(def.get_l_token());
  }
  for(const auto &def : syntax_rules){
    const auto & ptn=def.get_ptn();
    for(const auto &token : ptn){
      if(!is_non_terminal(token)) terminals.insert(token);
    }
  }
}

FirstSet::NullableSet::NullableSet(const std::vector<SyntaxDef> &syntax_rules){
  nulls_set.clear();
  for(const auto &def : syntax_rules){
    if(def.get_ptn().size()==0) nulls_set.insert(def.get_l_token());
  }
  bool flg_changed=true;
  while(flg_changed){
    flg_changed=false;
    for(const auto &def : syntax_rules){
      if(nulls_set.count(def.get_l_token())==1) continue;
      bool is_nulls=true;
      const auto &ptn=def.get_ptn();
      for(const auto &token : ptn){
        if(nulls_set.count(token)!=1){
          is_nulls=false;
          break;
        }
      }
      if(is_nulls){
        nulls_set.insert(def.get_l_token());
        flg_changed=true;
      }
    }
  }
}

FirstSet::FirstSet(const std::vector<SyntaxDef> &syntax_rules, SymbolDiscriminator symbol_dis)
  :nulls(syntax_rules){
  first_map.clear();
  std::unordered_map<string, std::unordered_set<string>> first_result;
  first_result["EOF"]={"EOF"};
  const auto &terminals=symbol_dis.get_terminals();
  for(const auto &token : terminals){
    first_result[token]={token};
  }
  const auto &non_terminals=symbol_dis.get_non_terminals();
  for(const auto &token : non_terminals){
    first_result[token]={};
  }
  std::vector<std::array<string,2>> constraint {}; //[superset, subset]
  for(const auto &rule : syntax_rules){
    const string &sup=rule.get_l_token();
    const auto &ptn=rule.get_ptn();
    for(const auto &sub : ptn){
      if(sup!=sub) constraint.push_back({sup, sub});
      if(!nulls.is_nullable(sub)) break;
    }
  }
  bool flg_changed=true;
  while(flg_changed){
    flg_changed=false;
    for(const auto &pair : constraint){
      auto &supset=first_result.at(pair[0]);
      const auto &subset=first_result.at(pair[1]);
      for(const auto &token : subset){
        if(supset.count(token)==0){
          supset.insert(token);
          flg_changed=true;
        }
      }
    }
  }
  first_map=first_result;
}

const std::unordered_set<string>& FirstSet::get(const string &token) const {
  return first_map.at(token);
}

const std::unordered_set<string>& FirstSet::get(const std::vector<string> &tokens) const {
  std::unordered_set<string> result;
  result.clear();
  for(const auto &token : tokens){
    const auto &f_set_token=get(token);
    for(const auto &f_item : f_set_token){
      result.insert(f_item);
    }
    if(!nulls.is_nullable(token)) break;
  }
}

void SyntaxDB::init_token_def_map(){
  for(int i=0;i<syntax_rules.size();i++){
    const auto &rule=syntax_rules[i];
    token_def_map[rule.get_l_token()].push_back(std::pair<int,SyntaxDef>(i,rule));
  }
}

SyntaxDB::SyntaxDB(const std::vector<SyntaxDef> &arg_syntax_rules)
  :sd(arg_syntax_rules),
   fs(arg_syntax_rules, sd),
   syntax_rules(arg_syntax_rules){
  init_token_def_map();
}

int SyntaxDB::get_token_id(const string &token){
  try{
    return token_id_map.at(token);
  }
  catch(std::out_of_range&){
    token_id_counter++;
    token_id_map[token]=token_id_counter;
    return token_id_counter;
  }
}

const std::vector<std::pair<int, SyntaxDef>>& SyntaxDB::find_def(const string &token) const {
  return token_def_map.at(token);
}

const SyntaxDef& SyntaxDB::get(int rule_id) const {
  return syntax_rules[rule_id];
}

#include "parser_gen.h"
#include <utility>
#include <algorithm>

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
  :nulls(syntax_rules)
{
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

std::unordered_set<string> FirstSet::get(const std::vector<string> &tokens) const {
  std::unordered_set<string> result;
  result.clear();
  for(const auto &token : tokens){
    const auto &f_set_token=get(token);
    for(const auto &f_item : f_set_token){
      result.insert(f_item);
    }
    if(!nulls.is_nullable(token)) break;
  }
  return std::move(result);
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
   syntax_rules(arg_syntax_rules),
   start_rule(META_START_SYMBOL, std::vector<string>({START_SYMBOL}))
{
  get_token_id(META_START_SYMBOL); // add to token_id_map
  get_token_id(START_SYMBOL); //add to token_id_map
  init_token_def_map();
}

int SyntaxDB::get_token_id(const string &token){
  try{
    return token_id_map.at(token);
  }
  catch(std::out_of_range&){
    token_id_map[token]=token_id_counter;
    token_id_counter++;
    return token_id_map[token];
  }
}

const std::vector<std::pair<int, SyntaxDef>>& SyntaxDB::find_rule(const string &token) const {
  return token_def_map.at(token);
}

const SyntaxDef& SyntaxDB::get_rule(int rule_id) const {
  if(rule_id==-1) return start_rule;
  return syntax_rules[rule_id];
}

int SyntaxDB::get_rule_id(const SyntaxDef &rule) const {
  if(rule==start_rule) return -1;
  const auto& itr=std::find(syntax_rules.begin(), syntax_rules.end(), rule);
  if(itr==syntax_rules.end()) return -100;
  return std::distance(syntax_rules.begin(), itr);
}

ClosureItem::ClosureItem(std::shared_ptr<SyntaxDB> arg_db, int arg_rule_id, int arg_dot_index, std::vector<string> arg_look_aheads)
  :db(arg_db),
   rule_id(arg_rule_id),
   dot_index(arg_dot_index),
   look_aheads(arg_look_aheads)
{
  sort_look_aheads();
  update_hash();
}

ClosureItem::ClosureItem(const ClosureItem &ci)
  :db(ci.db),
   rule_id(ci.rule_id),
   dot_index(ci.dot_index),
   look_aheads(ci.look_aheads),
   lr0_hash(ci.lr0_hash),
   lr1_hash(ci.lr1_hash){}

ClosureItem& ClosureItem::operator=(const ClosureItem &ci){
  if(&ci!=this){
    db=ci.db;
    rule_id=ci.rule_id;
    dot_index=ci.dot_index;
    look_aheads=ci.look_aheads;
    lr0_hash=ci.lr0_hash;
    lr1_hash=ci.lr1_hash;
  }
  return *this;
}

void ClosureItem::sort_look_aheads(){
  std::sort(look_aheads.begin(), look_aheads.end(), [this](string token1, string token2){
    return db->get_token_id(token1) > db->get_token_id(token2);
  });
}

void ClosureItem::update_hash(){
  lr0_hash=std::to_string(rule_id)+','+std::to_string(dot_index);
  lr1_hash=lr0_hash+'[';
  for(const auto &token : look_aheads){
    lr1_hash+=std::to_string(db->get_token_id(token))+',';
  }
  lr1_hash+=']';
  //std::cout << "lr0: " << lr0_hash << " , lr1: " << lr1_hash << '\n';
}

ClosureItem ClosureItem::merge(const ClosureItem& c) const {
  if(!is_same_lr0(c)||is_same_lr1(c)) return *this;
  std::unordered_set<string> look_aheads_set(look_aheads.begin(), look_aheads.end());
  for(const auto &la_item : c.look_aheads){
    look_aheads_set.insert(la_item);
  }
  std::vector<string> new_look_aheads(look_aheads_set.begin(), look_aheads_set.end());
  return ClosureItem(db, rule_id, dot_index, new_look_aheads);
}

std::vector<ClosureItem> ClosureItem::separate() const {
  std::vector<ClosureItem> items;
  for(const auto &la : look_aheads){
    items.emplace_back(db, rule_id, dot_index, std::vector<string>({la}));
  }
  return items;
}

ClosureSet::ClosureSet(std::shared_ptr<SyntaxDB> arg_db, std::vector<ClosureItem> arg_closure)
  :db(arg_db), closure(arg_closure)
{
  expand();
  sort();
  update_hash();
}

ClosureSet::ClosureSet(const ClosureSet &cs)
  :db(cs.db),
   closure(cs.closure),
   lr0_hash(cs.lr0_hash),
   lr1_hash(cs.lr1_hash){}

ClosureSet& ClosureSet::operator=(const ClosureSet &cs){
  if(&cs!=this){
    db=cs.db;
    closure=cs.closure;
    lr0_hash=cs.lr0_hash;
    lr1_hash=cs.lr1_hash;
  }
  return *this;
}

void ClosureSet::sort(){
  std::sort(closure.begin(),closure.end(),[this](const auto &i1, const auto &i2){
    return i1.get_lr1_hash() < i2.get_lr1_hash();
  });
}

void ClosureSet::update_hash(){
  lr0_hash="";
  lr1_hash="";
  for(const auto &item : closure){
    lr0_hash+=item.get_lr0_hash()+"|";
    lr1_hash+=item.get_lr1_hash()+"|";
  }
}

bool ClosureSet::contains(const ClosureItem &item) const {
  int min=0, max=size()-1, mid;
  while(min<=max){
    mid = min + (max-min)/2;
    if(item.get_lr1_hash()<closure[mid].get_lr1_hash()) max=mid-1;
    else if(item.get_lr1_hash()>closure[mid].get_lr1_hash()) min=mid+1;
    else return true;
  }
  return false;
}

ClosureSet ClosureSet::merge(const ClosureSet &c) const {
  if(!is_same_lr0(c)||is_same_lr1(c)) return *this;
  std::vector<ClosureItem> new_closure;
  const auto &c_closure=c.get_closure();
  for (int i=0; i<closure.size(); i++) {
    new_closure.push_back(closure[i].merge(c_closure[i]));
  }
  return ClosureSet(db, new_closure);
}

void ClosureSet::expand(){
  std::vector<ClosureItem> tmp_set=closure;
  closure.clear();
  for(const auto &ci : tmp_set){
    const auto tmp_items=ci.separate();
    for(const auto separated_item : tmp_items){
      closure.push_back(separated_item);
    }
  }
  sort();
  bool flg_changed=true;
  while(flg_changed){
    flg_changed=false;
    for(int i=0;i<closure.size();i++){
      const auto &ci=closure[i];
      const auto &ptn=db->get_rule(ci.get_rule_id()).get_ptn();
      const int dot_index=ci.get_dot_index();
      if(dot_index>=ptn.size()){
        continue;
      }
      const string follow=ptn.at(dot_index);
      if(!db->get_sd().is_non_terminal(follow)){
        continue;
      }

      //get look-ahead
      std::vector<string> first_index; // Token sequence to search first-tmp_set
      // first_index=ptn[ci.dot_index+1 : -1]
      for(auto itr=ptn.cbegin()+dot_index+1; itr<ptn.cend(); itr++){
        first_index.push_back(*itr);
      }
      first_index.push_back(ci.get_look_aheads().at(0));
      auto look_aheads=db->get_fs().get(first_index);

      //add ClosureItem
      const auto &rules=db->find_rule(follow);
      for(const auto &rule : rules){
        for(const auto &la : look_aheads){
          ClosureItem new_ci(db, rule.first, 0, std::vector<string>({la}));
          bool is_unique=!std::any_of(closure.begin(), closure.end(), [&new_ci](const auto &existing_item){
            return new_ci.is_same_lr1(existing_item);
          });
          if(is_unique){
            closure.push_back(new_ci);
            flg_changed=true;
          }
        }
      }
    }
  }
  sort();

  //merge ClosureItem
  tmp_set=closure;
  closure.clear();
  std::vector<string> merged_look_aheads;
  for(auto itr=tmp_set.cbegin();itr!=tmp_set.cend();itr++){
    merged_look_aheads.push_back(itr->get_look_aheads().at(0));
    if(itr+1==tmp_set.cend() || !itr->is_same_lr0(*(itr+1))){
      closure.emplace_back(db, itr->get_rule_id(), itr->get_dot_index(), merged_look_aheads);
      merged_look_aheads.clear();
    }
  }
}

DFA_Generator::DFA_Generator(std::shared_ptr<SyntaxDB> db)
  :db(db)
{
  generate_dfa();
  merge_la();
}

std::unordered_map<string, ClosureSet> DFA_Generator::generate_new_cs(const ClosureSet &cs) const {
  std::unordered_map<string, std::vector<ClosureItem>> items;
  for(const auto &ci : cs.get_closure()){
    int rule_id=ci.get_rule_id();
    int dot_index=ci.get_dot_index();
    const auto &look_aheads=ci.get_look_aheads();
    const auto &ptn=db->get_rule(rule_id).get_ptn();
    if(dot_index>=ptn.size()) continue;
    const string edge_label=ptn.at(dot_index);
    items[edge_label].emplace_back(db, rule_id, dot_index+1, look_aheads);
  }
  std::unordered_map<string, ClosureSet> result;
  for(const auto &pair : items){
    result.emplace(pair.first,ClosureSet(db, pair.second));
  }
  return result;
}

std::weak_ptr<DFA_Node> DFA_Generator::search_edge_dest(const std::vector<std::shared_ptr<DFA_Node>> &dfa, const std::shared_ptr<DFA_Node> &target) const{
  for(const auto &cand : dfa){
    if(target->cs.is_same_lr1(cand->cs)) return std::weak_ptr<DFA_Node>(cand);
  }
  return std::weak_ptr<DFA_Node>(target);
}

void DFA_Generator::generate_dfa(){
  ClosureItem initial_item(db, -1, 0, std::vector<string>({"EOF"}));
  ClosureSet initial_set(db, std::vector<ClosureItem>({std::move(initial_item)}));
  std::vector<std::shared_ptr<DFA_Node>> dfa {std::make_shared<DFA_Node>(std::move(initial_set))};
  bool flg_changed=true;
  while(flg_changed){
    flg_changed=false;
    for(int i=0;i<dfa.size();i++){
      const ClosureSet &cs=dfa[i]->cs;
      auto &edge=dfa[i]->edge;
      const auto &new_sets=generate_new_cs(cs);
      for(const auto &pair : new_sets){
        std::shared_ptr<DFA_Node> new_node=std::make_shared<DFA_Node>(pair.second);
        std::weak_ptr<DFA_Node> edge_to=search_edge_dest(dfa, new_node);
        if(edge_to.lock()==new_node){
          dfa.push_back(new_node);
          flg_changed=true;
        }
        if(edge.count(pair.first)==0){
          edge[pair.first]=edge_to;
          flg_changed=true;
        }
      }
    }
  }
  lr_dfa=dfa;
}

void DFA_Generator::merge_la(){
  if(lalr_dfa.size()!=0 || lr_dfa.size()==0) return;
  std::unordered_map<string, std::shared_ptr<DFA_Node>> edge_changes; // lr1_hash->weak_ptr<DFA_Node>
  auto tmp_dfa=lr_dfa;
  for(auto itr=tmp_dfa.begin();itr<tmp_dfa.end()-1;itr++){
    if(!(*itr)) continue;
    *itr=std::make_shared<DFA_Node>(**itr);
    edge_changes[(*itr)->cs.get_lr1_hash()]=*itr;
    for(auto itr_inner=itr+1;itr_inner<tmp_dfa.end();itr_inner++){
      if(!(*itr_inner)) continue;
      if((*itr)->cs.is_same_lr0((*itr_inner)->cs)){
        (*itr)->cs=(*itr)->cs.merge((*itr_inner)->cs);
        edge_changes[(*itr_inner)->cs.get_lr1_hash()]=*itr;
        itr_inner->reset();
      }
    }
  }
  lalr_dfa.clear();
  for(const auto &node_ptr : tmp_dfa){
    if(!node_ptr) continue;
    auto &new_edge=node_ptr->edge;
    for(auto &pair : new_edge){
      if(edge_changes.count(pair.second.lock()->cs.get_lr1_hash())){
        pair.second=edge_changes.at(pair.second.lock()->cs.get_lr1_hash());
      }
    }
    lalr_dfa.push_back(node_ptr);
  }
}

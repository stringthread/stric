#include "syntax.h"

const std::vector<LexDef> Syntax::tokens {
   {"", R"***( +)***"}, // whitespace: ignore
   {"", R"***(\r?\n)***"}, // EOL: ignore
   {"EOS", R"***(;)***"},
   {"COMMA", R"***(,)***"},
   {"DOT", R"***(\.)***"},
   {"NULL", R"***(null)***", true},
   {"RETURN", R"***(return)***", true},
   {"BREAK", R"***(break)***", true},
   {"CONTINUE", R"***(continue)***", true},
   {"IF", R"***(if)***", true},
   {"ELSE", R"***(else)***", true},
   {"WHILE", R"***(while)***", true},
   {"TRUE", R"***(@t)***", true},
   {"FALSE", R"***(@f)***", true},
   {"HEX_INT", R"***(0x[1-9a-fA-F][0-9a-fA-F]*)***"},
   {"FLOAT", R"***(([0-9]+\.[0-9]*|[0-9]*\.[0-9]+))***"}, // "2.", ".5", ".2"=> OK, "."=>NG
   {"INT", R"***(([1-9][0-9]*|0))***"},
   {"PLUSPLUS", R"***(\+\+)***"},
   {"PLUS", R"***(\+)***"},
   {"MINUSMINUS", R"***(--)***"},
   {"MINUS", R"***(-)***"},
   {"ASTAST", R"***(\*\*)***"},
   {"ASTERISK", R"***(\*)***"},
   {"DIV_INT", R"***(//)***"},
   {"SLASH", R"***(/)***"},
   {"ANDAND", R"***(&&)***"},
   {"AND", R"***(&)***"},
   {"OROR", R"***(\|\|)***"},
   {"OR", R"***(\|)***"},
   {"HAT", R"***(\^)***"},
   {"EQUAL", R"***(==)***"},
   {"NOT_EQUAL", R"***(\!=)***"},
   {"NOT", R"***(\!)***"},
   {"NOT", R"***(not)***",true},
   {"LESS_EQUAL", R"***(<=)***"},
   {"GREATER_EQUAL", R"***(>=)***"},
   {"LEFT_ANGLE_BRACKET", R"***(<)***"},
   {"RIGHT_ANGLE_BRACKET", R"***(>)***"},
   {"LEFT_PAREN", R"***(\()***"},
   {"RIGHT_PAREN", R"***(\))***"},
   {"LEFT_BRACE", R"***(\{)***"},
   {"RIGHT_BRACE", R"***(\})***"},
   {"LEFT_BRACKET", R"***(\[)***"},
   {"RIGHT_BRACKET", R"***(\])***"},
   {"ASSIGN", R"***(=)***"},
   {"IDENTIFIER", ""+IDENTIFIER_START_CHAR+IDENTIFIER_CHAR+"*"},
};
const std::vector<SyntaxDef> Syntax::syntax_rules {
  {"main", std::vector<string>({"main","block"})},
  {"main", std::vector<string>({"block"})},
  {"block", std::vector<string>({"sentence"})},
  {"block", std::vector<string>({"LEFT_BRACE","main","RIGHT_BRACE"})},
  {"sentence", std::vector<string>({"value_expr","EOS"})},
  {"sentence", std::vector<string>({"if_statement"})},
  {"sentence", std::vector<string>({"if_else_statement"})},
  {"sentence", std::vector<string>({"while_statement"})},
  {"sentence", std::vector<string>({"return_statement"})},
  {"sentence", std::vector<string>({"break_statement"})},
  {"sentence", std::vector<string>({"continue_statement"})},
  {"if_statement", std::vector<string>({"IF","atom_value","block"})},
  {"if_else_statement", std::vector<string>({"if_statement","ELSE","block"})},
  {"while_statement", std::vector<string>({"WHILE","atom_value","block"})},
  {"return_statement", std::vector<string>({"RETURN","EOS"})},
  {"return_statement", std::vector<string>({"RETURN","value_expr","EOS"})},
  {"break_statement", std::vector<string>({"BREAK","EOS"})},
  {"break_statement", std::vector<string>({"BREAK","value_expr","EOS"})},
  {"continue_statement", std::vector<string>({"CONTINUE","EOS"})},
  {"continue_statement", std::vector<string>({"CONTINUE","value_expr","EOS"})},
  // add for ... to sentence
  {"atom_value", std::vector<string>({"DOT","IDENTIFIER"})},
  {"atom_value", std::vector<string>({"atom_value","DOT","IDENTIFIER"})},
  {"atom_value", std::vector<string>({"literal"})},
  {"literal", std::vector<string>({"NULL"})},
  {"literal", std::vector<string>({"TRUE"})},
  {"literal", std::vector<string>({"FALSE"})},
  {"literal", std::vector<string>({"INT"})},
  {"literal", std::vector<string>({"HEX_INT"})},
  {"literal", std::vector<string>({"FLOAT"})},
  {"term_1", std::vector<string>({"atom_value"})},
  {"term_1", std::vector<string>({"term_1","suffix_unary_operator"})},
  {"suffix_unary_operator", std::vector<string>({"PLUSPLUS"})},
  {"suffix_unary_operator", std::vector<string>({"MINUSMINUS"})},
  {"term_2", std::vector<string>({"term_1"})},
  {"term_2", std::vector<string>({"prefix_unary_operator","term_2"})},
  {"prefix_unary_operator", std::vector<string>({"PLUSPLUS"})},
  {"prefix_unary_operator", std::vector<string>({"MINUSMINUS"})},
  {"prefix_unary_operator", std::vector<string>({"PLUS"})},
  {"prefix_unary_operator", std::vector<string>({"MINUS"})},
  {"prefix_unary_operator", std::vector<string>({"NOT"})},
  {"term_3", std::vector<string>({"term_2"})},
  {"term_3", std::vector<string>({"term_3","math_operator_1","term_2"})},
  {"math_operator_1", std::vector<string>({"ASTERISK"})},
  {"math_operator_1", std::vector<string>({"SLASH"})},
  {"math_operator_1", std::vector<string>({"DIV_INT"})},
  {"term_4", std::vector<string>({"term_3"})},
  {"term_4", std::vector<string>({"term_4","math_operator_2","term_3"})},
  {"math_operator_2", std::vector<string>({"PLUS"})},
  {"math_operator_2", std::vector<string>({"MINUS"})},
  {"term_5", std::vector<string>({"term_4"})},
  {"term_5", std::vector<string>({"term_4","compare_operator","term_5"})},
  {"compare_operator", std::vector<string>({"EQUAL"})},
  {"compare_operator", std::vector<string>({"NOT_EQUAL"})},
  {"compare_operator", std::vector<string>({"LEFT_ANGLE_BRACKET"})},
  {"compare_operator", std::vector<string>({"RIGHT_ANGLE_BRACKET"})},
  {"compare_operator", std::vector<string>({"LESS_EQUAL"})},
  {"compare_operator", std::vector<string>({"GREATER_EQUAL"})},
  {"term_6", std::vector<string>({"term_5"})},
  {"term_6", std::vector<string>({"term_5","bool_operator","term_6"})},
  {"bool_operator", std::vector<string>({"ANDAND"})},
  {"bool_operator", std::vector<string>({"OROR"})},
  {"term_7", std::vector<string>({"term_6"})},
  {"term_7", std::vector<string>({"term_7","assign_operator","term_6"})},
  //{"type", std::vector<string>({"LEFT_ANGLE_BRACKET","IDENTIFIER","RIGHT_ANGLE_BRACKET"})},
  {"var_def", std::vector<string>({"IDENTIFIER","IDENTIFIER"})},
  {"term_7", std::vector<string>({"var_def"})},
  {"assign_operator", std::vector<string>({"ASSIGN"})},
  /*{"binary_operator_3", std::vector<string>({"math_operator_1","ASSIGN"})},
  {"binary_operator_3", std::vector<string>({"math_operator_2","ASSIGN"})},*/
  {"value_expr", std::vector<string>({"term_7"})},
  {"atom_value", std::vector<string>({"LEFT_PAREN","term_7","RIGHT_PAREN"})},
};
const std::shared_ptr<SyntaxDB> Syntax::db=std::make_shared<SyntaxDB>(syntax_rules);

string LexDef::format_regex(string& arg_ptn, bool flg_force_end){
  arg_ptn=string("^")+arg_ptn;
  if(flg_force_end) arg_ptn+="(?!"+Syntax::IDENTIFIER_CHAR+")";
  return arg_ptn;
}

string LexDef::match(string str) const{
  std::smatch m;
  bool result=std::regex_search(str,m,ptn);
  if(!result) return "";
  return m.str();
}

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
  first_result[Syntax::EOF_SYMBOL]={Syntax::EOF_SYMBOL};
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
   start_rule(Syntax::META_START_SYMBOL, std::vector<string>({Syntax::START_SYMBOL}))
{
  get_token_id(Syntax::META_START_SYMBOL); // add to token_id_map
  get_token_id(Syntax::START_SYMBOL); //add to token_id_map
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

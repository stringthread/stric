#include <gtest/gtest.h>
#include "parser_gen.h"

#include <initializer_list>

TEST(ParserGenTest, SyntaxDBTest){
  std::vector<SyntaxDef> syntax_rules;
  syntax_rules.emplace_back("main", std::vector<string>({"main", "PLUS", "TERM"}));
  syntax_rules.emplace_back("main", std::vector<string>({"TERM"}));
  syntax_rules.emplace_back("TERM", std::vector<string>({"TERM", "ASTERISK", "ATOM"}));
  syntax_rules.emplace_back("TERM", std::vector<string>({"ATOM"}));
  syntax_rules.emplace_back("ATOM", std::vector<string>({"DIGITS"}));
  syntax_rules.emplace_back("ATOM", std::vector<string>({"LPAREN", "main", "RPAREN"}));
  SyntaxDB db(syntax_rules);
  //SymbolDiscriminator
  std::unordered_set<string> expect_terminals {"PLUS", "ASTERISK", "DIGITS", "LPAREN", "RPAREN"};
  EXPECT_EQ(db.get_sd().get_terminals()==expect_terminals, true);
  //FirstSet
  std::unordered_map<string,std::unordered_set<string>> expected_first;
  expected_first["main"]={"DIGITS","LPAREN"};
  expected_first["TERM"]={"DIGITS","LPAREN"};
  expected_first["ATOM"]={"DIGITS","LPAREN"};
  for(const auto &pair : expected_first){
    EXPECT_EQ(db.get_fs().get(pair.first)==pair.second, true);
  }
  //SyntaxDB::get_start_symbol
  EXPECT_EQ(db.get_start_symbol(), "main");
  //SyntaxDB::get_token_id
  EXPECT_EQ(db.get_token_id("main"),1);
  EXPECT_EQ(db.get_token_id("TERM"),2);
  EXPECT_EQ(db.get_token_id("main"),1);
  EXPECT_EQ(db.get_token_id("PLUS"),3);
  //SyntaxDB::get
  for (int i=0; i<syntax_rules.size(); i++) {
    const auto &rule=db.get(i);
    EXPECT_EQ(rule==syntax_rules[i], true);
  }
  //SyntaxDB::find_def
  auto &find_def_result=db.find_def("main");
  for(const auto &pair : find_def_result){
    EXPECT_EQ(pair.second==syntax_rules[pair.first], true);
  }
}

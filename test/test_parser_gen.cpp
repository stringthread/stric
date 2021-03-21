#include <gtest/gtest.h>
#include "parser_gen.h"

class ParserGenTest : public ::testing::Test {
protected:
  virtual void SetUp() {
      syntax_rules.emplace_back("main", std::vector<string>({"main", "PLUS", "TERM"}));
      syntax_rules.emplace_back("main", std::vector<string>({"TERM"}));
      syntax_rules.emplace_back("TERM", std::vector<string>({"TERM", "ASTERISK", "ATOM"}));
      syntax_rules.emplace_back("TERM", std::vector<string>({"ATOM"}));
      syntax_rules.emplace_back("ATOM", std::vector<string>({"DIGITS"}));
      syntax_rules.emplace_back("ATOM", std::vector<string>({"LPAREN", "main", "RPAREN"}));
      db=std::make_shared<SyntaxDB>(syntax_rules);
  }
public:
  std::vector<SyntaxDef> syntax_rules;
  std::shared_ptr<SyntaxDB> db;
};

TEST_F(ParserGenTest, SyntaxDBTest){
  //SymbolDiscriminator
  std::unordered_set<string> expect_terminals {"PLUS", "ASTERISK", "DIGITS", "LPAREN", "RPAREN"};
  EXPECT_EQ(db->get_sd().get_terminals(), expect_terminals);
  //FirstSet
  std::unordered_map<string,std::unordered_set<string>> expected_first;
  expected_first["main"]={"DIGITS","LPAREN"};
  expected_first["TERM"]={"DIGITS","LPAREN"};
  expected_first["ATOM"]={"DIGITS","LPAREN"};
  for(const auto &pair : expected_first){
    EXPECT_EQ(db->get_fs().get(pair.first), pair.second);
  }
  std::vector<string> test_get_vec {"main", "TERM"};
  std::unordered_set<string> expect_get_vec {"DIGITS", "LPAREN"};
  auto result_get_vec=db->get_fs().get(test_get_vec);
  EXPECT_EQ(result_get_vec, expect_get_vec);
  //SyntaxDB::get_start_symbol
  EXPECT_EQ(db->get_start_symbol(), "main");
  //SyntaxDB::get_token_id
  EXPECT_EQ(db->get_token_id("main"),0);
  EXPECT_EQ(db->get_token_id("TERM"),1);
  EXPECT_EQ(db->get_token_id("main"),0);
  EXPECT_EQ(db->get_token_id("PLUS"),2);
  //SyntaxDB::get
  for (int i=0; i<syntax_rules.size(); i++) {
    const auto &rule=db->get_rule(i);
    EXPECT_EQ(rule, syntax_rules[i]);
  }
  //SyntaxDB::find_rule
  auto &find_rule_result=db->find_rule("main");
  for(const auto &pair : find_rule_result){
    EXPECT_EQ(pair.second, syntax_rules[pair.first]);
  }
}

TEST_F(ParserGenTest, ClosureItemTest){
  ClosureItem ci00(db, 0, 0, std::vector<string>({"DIGITS", "LPAREN"}));
  ClosureItem ci00_part(db, 0, 0, std::vector<string>({"DIGITS"}));
  ClosureItem ci00_part_2(db, 0, 0, std::vector<string>({"LPAREN"}));
  ClosureItem ci01(db, 0, 1, std::vector<string>({"DIGITS", "LPAREN"}));
  ClosureItem ci10(db, 1, 0, std::vector<string>({"DIGITS", "LPAREN"}));
  //ClosureItem::hashes
  EXPECT_EQ(ci00.get_lr0_hash(), ci00_part.get_lr0_hash());
  EXPECT_TRUE(ci00.is_same_lr0(ci00_part));
  EXPECT_NE(ci00.get_lr1_hash(), ci00_part.get_lr1_hash());
  EXPECT_FALSE(ci00.is_same_lr1(ci00_part));
  EXPECT_NE(ci00.get_lr0_hash(), ci01.get_lr0_hash());
  EXPECT_FALSE(ci00.is_same_lr0(ci01));
  EXPECT_NE(ci00.get_lr0_hash(), ci10.get_lr0_hash());
  EXPECT_FALSE(ci00.is_same_lr0(ci10));
  //ClosureItem::merge
  EXPECT_TRUE(ci00_part.merge(ci00_part_2).is_same_lr1(ci00));
  //Closureitem::separate
  std::vector<ClosureItem> separated=ci00.separate();
  for(auto &item : separated){
    EXPECT_TRUE(item.is_same_lr1(ci00_part)||item.is_same_lr1(ci00_part_2));
  }
}

TEST_F(ParserGenTest, ClosureSetTest){
  ClosureItem ci30(db, 3, 0, std::vector<string>({"DIGITS", "LPAREN"}));
  ClosureSet cs(db, std::vector<ClosureItem>({ci30}));
  cout<<"End ClosureSet::ClosureSet"<<endl;
  std::vector<ClosureItem> expected_items {
    {db, 3, 0, std::vector<string>({"DIGITS", "LPAREN"})},
    {db, 4, 0, std::vector<string>({"DIGITS", "LPAREN"})},
    {db, 5, 0, std::vector<string>({"DIGITS", "LPAREN"})},
  };
  for(const auto &item : expected_items){
    EXPECT_TRUE(cs.contains(item))<<item.get_lr1_hash()<<endl;
  }
}

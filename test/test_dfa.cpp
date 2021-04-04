#include <gtest/gtest.h>
#include "parser/dfa.h"

class ClosureTest : public ::testing::Test {
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

TEST_F(ClosureTest, SyntaxDBTest){
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
  EXPECT_EQ(db->get_token_id("main"),1);
  EXPECT_EQ(db->get_token_id("TERM"),2);
  EXPECT_EQ(db->get_token_id("main"),1);
  EXPECT_EQ(db->get_token_id("PLUS"),3);
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

TEST_F(ClosureTest, ClosureItemTest){
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

TEST_F(ClosureTest, ClosureSetTest){
  ClosureItem ci10(db, 1, 0, std::vector<string>({"DIGITS", "LPAREN"}));
  ClosureSet cs(db, std::vector<ClosureItem>({ci10}));
  std::vector<ClosureItem> expected_items {
    {db, 1, 0, std::vector<string>({"DIGITS", "LPAREN"})},
    {db, 2, 0, std::vector<string>({"DIGITS", "LPAREN", "ASTERISK"})},
    {db, 3, 0, std::vector<string>({"DIGITS", "LPAREN", "ASTERISK"})},
    {db, 4, 0, std::vector<string>({"DIGITS", "LPAREN", "ASTERISK"})},
    {db, 5, 0, std::vector<string>({"DIGITS", "LPAREN", "ASTERISK"})},
  };
  EXPECT_EQ(cs.size(), expected_items.size());
  for(const auto &item : expected_items){
    EXPECT_TRUE(cs.contains(item))<<item.get_lr1_hash()<<endl;
  }
}

TEST_F(ClosureTest, ClosureSetMergeTest){
  ClosureItem ci_part_1_1(db, 4, 0, std::vector<string>({"DIGITS"}));
  ClosureItem ci_part_1_2(db, 5, 0, std::vector<string>({"DIGITS"}));
  ClosureItem ci_part_2_1(db, 4, 0, std::vector<string>({"LPAREN"}));
  ClosureSet cs_part_1(db, std::vector<ClosureItem>({ci_part_1_1, ci_part_1_2}));
  ClosureSet cs_part_2(db, std::vector<ClosureItem>({ci_part_2_1, ci_part_1_2}));
  ClosureSet cs_merged=cs_part_1.merge(cs_part_2);
  ClosureItem ci_expected_1(db, 4, 0, std::vector<string>({"DIGITS", "LPAREN"}));
  ClosureSet cs_expected(db, std::vector<ClosureItem>({ci_expected_1, ci_part_1_2}));
  ASSERT_TRUE(cs_merged.is_same_lr1(cs_expected));
}

class DFATest : public ::testing::Test {
protected:
  virtual void SetUp() {
      syntax_rules.emplace_back("main", std::vector<string>({"ATOM"}));
      syntax_rules.emplace_back("ATOM", std::vector<string>({"DIGITS"}));
      syntax_rules.emplace_back("ATOM", std::vector<string>({"LPAREN", "ATOM", "RPAREN"}));
      db=std::make_shared<SyntaxDB>(syntax_rules);
  }
public:
  std::vector<SyntaxDef> syntax_rules;
  std::shared_ptr<SyntaxDB> db;
};

TEST_F(DFATest, LR_DFATest){
  // Expected ClosureItem
  ClosureItem ci0_1(db, -1, 0, std::vector<string>({"EOF"}));
  ClosureItem ci0_2(db, 0, 0, std::vector<string>({"EOF"}));
  ClosureItem ci0_3(db, 1, 0, std::vector<string>({"EOF"}));
  ClosureItem ci0_4(db, 2, 0, std::vector<string>({"EOF"}));
  ClosureItem ci1_1(db, -1, 1, std::vector<string>({"EOF"}));
  ClosureItem ci2_1(db, 0, 1, std::vector<string>({"EOF"}));
  ClosureItem ci3_1(db, 1, 1, std::vector<string>({"EOF"}));
  ClosureItem ci4_1(db, 2, 1, std::vector<string>({"EOF"}));
  ClosureItem ci4_2(db, 1, 0, std::vector<string>({"RPAREN"}));
  ClosureItem ci4_3(db, 2, 0, std::vector<string>({"RPAREN"}));
  ClosureItem ci5_1(db, 2, 2, std::vector<string>({"EOF"}));
  ClosureItem ci6_1(db, 1, 1, std::vector<string>({"RPAREN"}));
  ClosureItem ci7_1(db, 2, 1, std::vector<string>({"RPAREN"}));
  ClosureItem ci7_2(db, 1, 0, std::vector<string>({"RPAREN"}));
  ClosureItem ci7_3(db, 2, 0, std::vector<string>({"RPAREN"}));
  ClosureItem ci8_1(db, 2, 3, std::vector<string>({"EOF"}));
  ClosureItem ci9_1(db, 2, 2, std::vector<string>({"RPAREN"}));
  ClosureItem ci10_1(db, 2, 3, std::vector<string>({"RPAREN"}));
  // Expected ClosureSet
  ClosureSet cs_expected_initial(db, std::vector<ClosureItem>({ci0_1}));
  ClosureSet cs0(db, std::vector<ClosureItem>({ci0_1, ci0_2, ci0_3, ci0_4}));
  ClosureSet cs1(db, std::vector<ClosureItem>({ci1_1}));
  ClosureSet cs2(db, std::vector<ClosureItem>({ci2_1}));
  ClosureSet cs3(db, std::vector<ClosureItem>({ci3_1}));
  ClosureSet cs4(db, std::vector<ClosureItem>({ci4_1, ci4_2, ci4_3}));
  ClosureSet cs5(db, std::vector<ClosureItem>({ci5_1}));
  ClosureSet cs6(db, std::vector<ClosureItem>({ci6_1}));
  ClosureSet cs7(db, std::vector<ClosureItem>({ci7_1, ci7_2, ci7_3}));
  ClosureSet cs8(db, std::vector<ClosureItem>({ci8_1}));
  ClosureSet cs9(db, std::vector<ClosureItem>({ci9_1}));
  ClosureSet cs10(db, std::vector<ClosureItem>({ci10_1}));
  // Actual process
  DFA_Generator dfa_g(db);
  const auto result=dfa_g.get_lr_dfa();
  // Assertions
  EXPECT_TRUE(cs0.is_same_lr1(cs_expected_initial));
  ASSERT_TRUE(result[0]->cs.is_same_lr1(cs0)) <<result[0]->cs.get_lr1_hash()<<" (expected: "<<cs0.get_lr1_hash()<<" )";
  EXPECT_NE(result[0]->edge.count("main"),0);
  std::shared_ptr<DFA_Node> result_1;
  if(result[0]->edge.count("main")!=0){
    result_1=result[0]->edge["main"].lock();
    ASSERT_TRUE(result_1)<<"shared_ptr expired";
    EXPECT_TRUE(result_1->cs.is_same_lr1(cs1)) <<result_1->cs.get_lr1_hash()<<" (expected: "<<cs1.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result[0]->edge.count("ATOM"),0);
  std::shared_ptr<DFA_Node> result_2;
  if(result[0]->edge.count("ATOM")!=0){
    result_2=result[0]->edge["ATOM"].lock();
    ASSERT_TRUE(result_2)<<"shared_ptr expired";
    EXPECT_TRUE(result_2->cs.is_same_lr1(cs2)) <<result_2->cs.get_lr1_hash()<<" (expected: "<<cs2.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result[0]->edge.count("DIGITS"),0);
  std::shared_ptr<DFA_Node> result_3;
  if(result[0]->edge.count("DIGITS")!=0){
    result_3=result[0]->edge["DIGITS"].lock();
    ASSERT_TRUE(result_3)<<"shared_ptr expired";
    EXPECT_TRUE(result_3->cs.is_same_lr1(cs3)) <<result_3->cs.get_lr1_hash()<<" (expected: "<<cs3.get_lr1_hash()<<" )";
  }
  ASSERT_NE(result[0]->edge.count("LPAREN"),0);
  std::shared_ptr<DFA_Node> result_4;
  if(result[0]->edge.count("LPAREN")!=0){
    result_4=result[0]->edge["LPAREN"].lock();
    ASSERT_TRUE(result_4)<<"shared_ptr expired";
    EXPECT_TRUE(result_4->cs.is_same_lr1(cs4)) <<result_4->cs.get_lr1_hash()<<" (expected: "<<cs4.get_lr1_hash()<<" )";
  }
  ASSERT_NE(result_4->edge.count("ATOM"),0);
  std::shared_ptr<DFA_Node> result_5;
  if(result_4->edge.count("ATOM")!=0){
    result_5=result_4->edge["ATOM"].lock();
    ASSERT_TRUE(result_5)<<"shared_ptr expired";
    EXPECT_TRUE(result_5->cs.is_same_lr1(cs5)) <<result_5->cs.get_lr1_hash()<<" (expected: "<<cs5.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result_4->edge.count("DIGITS"),0);
  std::shared_ptr<DFA_Node> result_6;
  if(result_4->edge.count("DIGITS")!=0){
    result_6=result_4->edge["DIGITS"].lock();
    ASSERT_TRUE(result_6)<<"shared_ptr expired";
    EXPECT_TRUE(result_6->cs.is_same_lr1(cs6)) <<result_6->cs.get_lr1_hash()<<" (expected: "<<cs6.get_lr1_hash()<<" )";
  }
  ASSERT_NE(result_4->edge.count("LPAREN"),0);
  std::shared_ptr<DFA_Node> result_7;
  if(result_4->edge.count("LPAREN")!=0){
    result_7=result_4->edge["LPAREN"].lock();
    ASSERT_TRUE(result_7)<<"shared_ptr expired";
    EXPECT_TRUE(result_7->cs.is_same_lr1(cs7)) <<result_7->cs.get_lr1_hash()<<" (expected: "<<cs7.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result_5->edge.count("RPAREN"),0);
  std::shared_ptr<DFA_Node> result_8;
  if(result_5->edge.count("RPAREN")!=0){
    result_8=result_5->edge["RPAREN"].lock();
    ASSERT_TRUE(result_8)<<"shared_ptr expired";
    EXPECT_TRUE(result_8->cs.is_same_lr1(cs8)) <<result_8->cs.get_lr1_hash()<<" (expected: "<<cs8.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result_7->edge.count("DIGITS"),0);
  std::shared_ptr<DFA_Node> result_6_2;
  if(result_7->edge.count("DIGITS")!=0){
    result_6_2=result_7->edge["DIGITS"].lock();
    ASSERT_TRUE(result_6_2)<<"shared_ptr expired";
    EXPECT_TRUE(result_6_2->cs.is_same_lr1(cs6)) <<result_6_2->cs.get_lr1_hash()<<" (expected: "<<cs6.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result_7->edge.count("LPAREN"),0);
  std::shared_ptr<DFA_Node> result_7_2;
  if(result_7->edge.count("LPAREN")!=0){
    result_7_2=result_7->edge["LPAREN"].lock();
    ASSERT_TRUE(result_7_2)<<"shared_ptr expired";
    EXPECT_TRUE(result_7_2->cs.is_same_lr1(cs7)) <<result_7_2->cs.get_lr1_hash()<<" (expected: "<<cs7.get_lr1_hash()<<" )";
  }
  ASSERT_NE(result_7->edge.count("ATOM"),0);
  std::shared_ptr<DFA_Node> result_9;
  if(result_7->edge.count("ATOM")!=0){
    result_9=result_7->edge["ATOM"].lock();
    ASSERT_TRUE(result_9)<<"shared_ptr expired";
    EXPECT_TRUE(result_9->cs.is_same_lr1(cs9)) <<result_9->cs.get_lr1_hash()<<" (expected: "<<cs9.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result_9->edge.count("RPAREN"),0);
  std::shared_ptr<DFA_Node> result_10;
  if(result_9->edge.count("RPAREN")!=0){
    result_10=result_9->edge["RPAREN"].lock();
    ASSERT_TRUE(result_10)<<"shared_ptr expired";
    EXPECT_TRUE(result_10->cs.is_same_lr1(cs10)) <<result_10->cs.get_lr1_hash()<<" (expected: "<<cs10.get_lr1_hash()<<" )";
  }
}

TEST_F(DFATest, LALR_DFATest){
  // Expected ClosureItem
  ClosureItem ci0_1(db, -1, 0, std::vector<string>({"EOF"}));
  ClosureItem ci0_2(db, 0, 0, std::vector<string>({"EOF"}));
  ClosureItem ci0_3(db, 1, 0, std::vector<string>({"EOF"}));
  ClosureItem ci0_4(db, 2, 0, std::vector<string>({"EOF"}));
  ClosureItem ci1_1(db, -1, 1, std::vector<string>({"EOF"}));
  ClosureItem ci2_1(db, 0, 1, std::vector<string>({"EOF"}));
  ClosureItem ci11_1(db, 2, 1, std::vector<string>({"EOF", "RPAREN"}));
  ClosureItem ci11_2(db, 1, 0, std::vector<string>({"RPAREN"}));
  ClosureItem ci11_3(db, 2, 0, std::vector<string>({"RPAREN"}));
  ClosureItem ci12_1(db, 1, 1, std::vector<string>({"EOF", "RPAREN"}));
  ClosureItem ci13_1(db, 2, 2, std::vector<string>({"EOF", "RPAREN"}));
  ClosureItem ci14_1(db, 2, 3, std::vector<string>({"EOF", "RPAREN"}));
  // Expected ClosureSet
  ClosureSet cs0(db, std::vector<ClosureItem>({ci0_1, ci0_2, ci0_3, ci0_4}));
  ClosureSet cs1(db, std::vector<ClosureItem>({ci1_1}));
  ClosureSet cs2(db, std::vector<ClosureItem>({ci2_1}));
  ClosureSet cs11(db, std::vector<ClosureItem>({ci11_1, ci11_2, ci11_3}));
  ClosureSet cs12(db, std::vector<ClosureItem>({ci12_1}));
  ClosureSet cs13(db, std::vector<ClosureItem>({ci13_1}));
  ClosureSet cs14(db, std::vector<ClosureItem>({ci14_1}));
  // Actual process
  DFA_Generator dfa_g(db);
  const auto result=dfa_g.get_lalr_dfa();
  // Assertions
  ASSERT_NE(result.size(), 0);
  ASSERT_TRUE(result[0]->cs.is_same_lr1(cs0)) <<result[0]->cs.get_lr1_hash()<<" (expected: "<<cs0.get_lr1_hash()<<" )";
  EXPECT_NE(result[0]->edge.count("main"),0);
  std::shared_ptr<DFA_Node> result_1;
  if(result[0]->edge.count("main")!=0){
    result_1=result[0]->edge["main"].lock();
    ASSERT_TRUE(result_1)<<"shared_ptr expired";
    EXPECT_TRUE(result_1->cs.is_same_lr1(cs1)) <<result_1->cs.get_lr1_hash()<<" (expected: "<<cs1.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result[0]->edge.count("ATOM"),0);
  std::shared_ptr<DFA_Node> result_2;
  if(result[0]->edge.count("ATOM")!=0){
    result_2=result[0]->edge["ATOM"].lock();
    ASSERT_TRUE(result_2)<<"shared_ptr expired";
    EXPECT_TRUE(result_2->cs.is_same_lr1(cs2)) <<result_2->cs.get_lr1_hash()<<" (expected: "<<cs2.get_lr1_hash()<<" )";
  }
  ASSERT_NE(result[0]->edge.count("LPAREN"),0);
  std::shared_ptr<DFA_Node> result_11;
  if(result[0]->edge.count("LPAREN")!=0){
    result_11=result[0]->edge["LPAREN"].lock();
    ASSERT_TRUE(result_11)<<"shared_ptr expired";
    EXPECT_TRUE(result_11->cs.is_same_lr1(cs11)) <<result_11->cs.get_lr1_hash()<<" (expected: "<<cs11.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result[0]->edge.count("DIGITS"),0);
  std::shared_ptr<DFA_Node> result_12;
  if(result[0]->edge.count("DIGITS")!=0){
    result_12=result[0]->edge["DIGITS"].lock();
    ASSERT_TRUE(result_12)<<"shared_ptr expired";
    EXPECT_TRUE(result_12->cs.is_same_lr1(cs12)) <<result_12->cs.get_lr1_hash()<<" (expected: "<<cs12.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result_11->edge.count("LPAREN"),0);
  std::shared_ptr<DFA_Node> result_11_2;
  if(result_11->edge.count("LPAREN")!=0){
    result_11_2=result_11->edge["LPAREN"].lock();
    ASSERT_TRUE(result_11_2)<<"shared_ptr expired";
    EXPECT_TRUE(result_11_2->cs.is_same_lr1(cs11)) <<result_11_2->cs.get_lr1_hash()<<" (expected: "<<cs11.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result_11->edge.count("DIGITS"),0);
  std::shared_ptr<DFA_Node> result_12_2;
  if(result_11->edge.count("DIGITS")!=0){
    result_12_2=result_11->edge["DIGITS"].lock();
    ASSERT_TRUE(result_12_2)<<"shared_ptr expired";
    EXPECT_TRUE(result_12_2->cs.is_same_lr1(cs12)) <<result_12_2->cs.get_lr1_hash()<<" (expected: "<<cs12.get_lr1_hash()<<" )";
  }
  ASSERT_NE(result_11->edge.count("ATOM"),0);
  std::shared_ptr<DFA_Node> result_13;
  if(result_11->edge.count("ATOM")!=0){
    result_13=result_11->edge["ATOM"].lock();
    ASSERT_TRUE(result_13)<<"shared_ptr expired";
    EXPECT_TRUE(result_13->cs.is_same_lr1(cs13)) <<result_13->cs.get_lr1_hash()<<" (expected: "<<cs13.get_lr1_hash()<<" )";
  }
  EXPECT_NE(result_13->edge.count("RPAREN"),0);
  std::shared_ptr<DFA_Node> result_14;
  if(result_13->edge.count("RPAREN")!=0){
    result_14=result_13->edge["RPAREN"].lock();
    ASSERT_TRUE(result_14)<<"shared_ptr expired";
    EXPECT_TRUE(result_14->cs.is_same_lr1(cs14)) <<result_14->cs.get_lr1_hash()<<" (expected: "<<cs14.get_lr1_hash()<<" )";
  }
}

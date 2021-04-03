#include <gtest/gtest.h>
#include "parser_gen.h"

class ParserGenTest : public ::testing::Test {
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

void check_table(const ParsingTable &result_table, const ClosureSet &expected_cs, const ParsingTableRow &expected_row){
  ASSERT_NE(result_table.first.count(expected_cs.get_lr1_hash()), 0)<<"Row key not exist: "<<expected_cs.get_lr1_hash();
  const ParsingTableRow &result_row=result_table.first.at(expected_cs.get_lr1_hash());
  ASSERT_EQ(result_row.size(), expected_row.size());
  for(const auto &pair : expected_row){
    ASSERT_NE(result_row.count(pair.first), 0)<<"Empty cell: ("<<expected_cs.get_lr1_hash()<<" , "<<pair.first<<")";
    EXPECT_EQ(*(pair.second), *(result_row.at(pair.first)))<<" : "<<expected_cs.get_lr1_hash();
  }
}

TEST_F(ParserGenTest, ParsingTableTest){
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
  std::vector<ClosureSet> expected_cs_vec { cs0, cs1, cs2, cs11, cs12, cs13, cs14 };
  // Expected ParsingTable
  ParsingTableRow row0={
    {"DIGITS", std::make_shared<ShiftOperation>(cs12.get_lr1_hash())},
    {"LPAREN", std::make_shared<ShiftOperation>(cs11.get_lr1_hash())},
    {"main", std::make_shared<GotoOperation>(cs1.get_lr1_hash())},
    {"ATOM", std::make_shared<GotoOperation>(cs2.get_lr1_hash())},
  };
  ParsingTableRow row1={
    {"EOF", std::make_shared<AcceptOperation>()},
  };
  ParsingTableRow row2={
    {"EOF", std::make_shared<ReduceOperation>(0)},
  };
  ParsingTableRow row11={
    {"DIGITS", std::make_shared<ShiftOperation>(cs12.get_lr1_hash())},
    {"LPAREN", std::make_shared<ShiftOperation>(cs11.get_lr1_hash())},
    {"ATOM", std::make_shared<GotoOperation>(cs13.get_lr1_hash())},
  };
  ParsingTableRow row12={
    {"RPAREN", std::make_shared<ReduceOperation>(1)},
    {"EOF", std::make_shared<ReduceOperation>(1)},
  };
  ParsingTableRow row13={
    {"RPAREN", std::make_shared<ShiftOperation>(cs14.get_lr1_hash())},
  };
  ParsingTableRow row14={
    {"RPAREN", std::make_shared<ReduceOperation>(2)},
    {"EOF", std::make_shared<ReduceOperation>(2)},
  };
  std::vector<ParsingTableRow> expected_row_vec { row0, row1, row2, row11, row12, row13, row14 };
  // Actual process
  ParserGenerator parser_gen(db);
  const ParsingTable &result_table=parser_gen.get_parsing_table();
  //Assertions
  EXPECT_EQ(parser_gen.get_table_type(), TableType::LALR1);
  EXPECT_NE(parser_gen.get_table_type(), TableType::EMPTY);
  EXPECT_NE(parser_gen.get_table_type(), TableType::LR1);
  EXPECT_NE(parser_gen.get_table_type(), TableType::CONFLICT);
  EXPECT_EQ(result_table.second, cs0.get_lr1_hash());
  ASSERT_EQ(expected_row_vec.size(), result_table.first.size());
  for (size_t i = 0; i < expected_cs_vec.size(); i++) {
    check_table(result_table, expected_cs_vec[i], expected_row_vec[i]);
  }
}

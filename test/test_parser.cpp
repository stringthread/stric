#include <gtest/gtest.h>
#include "parser.h"
#include "lexer.h"
#include "parser_gen.h"

class ParserTest : public ::testing::Test {
protected:
  virtual void SetUp() {
      syntax_rules.emplace_back("main", std::vector<string>({"ATOM"}));
      syntax_rules.emplace_back("ATOM", std::vector<string>({"INT"}));
      syntax_rules.emplace_back("ATOM", std::vector<string>({"LEFT_PAREN", "ATOM", "RIGHT_PAREN"}));
      db=std::make_shared<SyntaxDB>(syntax_rules);
  }
public:
  std::vector<SyntaxDef> syntax_rules;
  std::shared_ptr<SyntaxDB> db;
};

TEST_F(ParserTest, ParseTest){
  //Expected output
  AST_Node expect("main","",std::vector<AST_Node>({
    AST_Node("ATOM","",std::vector<AST_Node>({
      AST_Node("LEFT_PAREN","("),
      AST_Node("ATOM","",std::vector<AST_Node>({
        AST_Node("LEFT_PAREN","("),
        AST_Node("ATOM","",std::vector<AST_Node>({
          AST_Node("INT","12"),
        })),
        AST_Node("RIGHT_PAREN",")"),
      })),
      AST_Node("RIGHT_PAREN",")"),
    })),
  }));
  // Actual process
  Lexer lex;
  ParserGenerator gen(db);
  ParsingTable parsing_table=gen.get_parsing_table();
  Parser parser(lex, db, parsing_table);
  AST_Node result=parser.parse("((12))");
  // Assertions
  std::cout << result.str() << '\n';
  EXPECT_EQ(result, expect);
}

class ComplexedSyntaxTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    syntax_rules.emplace_back("main", std::vector<string>({"main","PLUS","TERM"}));
    syntax_rules.emplace_back("main", std::vector<string>({"TERM"}));
    syntax_rules.emplace_back("TERM", std::vector<string>({"TERM","ASTERISK","ATOM"}));
    syntax_rules.emplace_back("TERM", std::vector<string>({"ATOM"}));
    syntax_rules.emplace_back("ATOM", std::vector<string>({"INT"}));
    syntax_rules.emplace_back("ATOM", std::vector<string>({"LEFT_PAREN", "main", "RIGHT_PAREN"}));
    db=std::make_shared<SyntaxDB>(syntax_rules);
  }
public:
  std::vector<SyntaxDef> syntax_rules;
  std::shared_ptr<SyntaxDB> db;
};

TEST_F(ComplexedSyntaxTest, ParseTest){
  //Expected output
  AST_Node expect("main","",std::vector<AST_Node>({
    AST_Node("TERM","",std::vector<AST_Node>({
      AST_Node("TERM","",std::vector<AST_Node>({
        AST_Node("ATOM","",std::vector<AST_Node>({
          AST_Node("LEFT_PAREN","("),
          AST_Node("main","",std::vector<AST_Node>({
            AST_Node("main","",std::vector<AST_Node>({
              AST_Node("TERM","",std::vector<AST_Node>({
                AST_Node("ATOM","",std::vector<AST_Node>({
                  AST_Node("INT","11"),
                })),
              })),
            })),
            AST_Node("PLUS","+"),
            AST_Node("TERM","",std::vector<AST_Node>({
              AST_Node("TERM","",std::vector<AST_Node>({
                AST_Node("ATOM","",std::vector<AST_Node>({
                  AST_Node("INT","3"),
                })),
              })),
              AST_Node("ASTERISK","*"),
              AST_Node("ATOM","",std::vector<AST_Node>({
                AST_Node("INT","2"),
              })),
            })),
          })),
          AST_Node("RIGHT_PAREN",")"),
        })),
      })),
      AST_Node("ASTERISK","*"),
      AST_Node("ATOM","",std::vector<AST_Node>({
        AST_Node("INT","3"),
      })),
    })),
  }));
  // Actual process
  Lexer lex;
  ParserGenerator gen(db);
  ParsingTable parsing_table=gen.get_parsing_table();
  Parser parser(lex, db, parsing_table);
  AST_Node result=parser.parse("(11+3*2)*3");
  // Assertions
  std::cout << result.str() << '\n';
  EXPECT_EQ(result, expect);
}

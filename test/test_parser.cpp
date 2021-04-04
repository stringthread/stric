#include <gtest/gtest.h>
#include "parser/parser.h"
#include "parser/syntax.h"
#include "parser/lexer.h"
#include "parser/parser_gen.h"

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
  AST_Node expect("main","",0,std::vector<AST_Node>({
    AST_Node("ATOM","",2,std::vector<AST_Node>({
      AST_Node("LEFT_PAREN","(",-2),
      AST_Node("ATOM","",2,std::vector<AST_Node>({
        AST_Node("LEFT_PAREN","(",-2),
        AST_Node("ATOM","",1,std::vector<AST_Node>({
          AST_Node("INT","12",-2),
        })),
        AST_Node("RIGHT_PAREN",")",-2),
      })),
      AST_Node("RIGHT_PAREN",")",-2),
    })),
  }));
  // Actual process
  Lexer lex;
  ParserGenerator gen(db);
  ParsingTable parsing_table=gen.get_parsing_table();
  Parser parser(lex, db, parsing_table);
  AST_Node result=parser.parse("((12))");
  // Assertions
  EXPECT_EQ(result, expect)<< result.str();
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
  AST_Node expect("main","",1,std::vector<AST_Node>({
    AST_Node("TERM","",2,std::vector<AST_Node>({
      AST_Node("TERM","",3,std::vector<AST_Node>({
        AST_Node("ATOM","",5,std::vector<AST_Node>({
          AST_Node("LEFT_PAREN","(",-2),
          AST_Node("main","",0,std::vector<AST_Node>({
            AST_Node("main","",1,std::vector<AST_Node>({
              AST_Node("TERM","",3,std::vector<AST_Node>({
                AST_Node("ATOM","",4,std::vector<AST_Node>({
                  AST_Node("INT","11",-2),
                })),
              })),
            })),
            AST_Node("PLUS","+",-2),
            AST_Node("TERM","",2,std::vector<AST_Node>({
              AST_Node("TERM","",3,std::vector<AST_Node>({
                AST_Node("ATOM","",4,std::vector<AST_Node>({
                  AST_Node("INT","3",-2),
                })),
              })),
              AST_Node("ASTERISK","*",-2),
              AST_Node("ATOM","",4,std::vector<AST_Node>({
                AST_Node("INT","2",-2),
              })),
            })),
          })),
          AST_Node("RIGHT_PAREN",")",-2),
        })),
      })),
      AST_Node("ASTERISK","*",-2),
      AST_Node("ATOM","",4,std::vector<AST_Node>({
        AST_Node("INT","3",-2),
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
  EXPECT_EQ(result, expect)<< result.str();
}

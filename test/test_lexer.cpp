#include <gtest/gtest.h>
#include "lexer.h"

TEST(LexerTest, LexerAnalizeTest){
  Lexer lexer;
  const string test="-(32+4.-2)+1>=var;";
  const vector<string> expect_tokens {
    "MINUS", //-
    "LEFT_PAREN", //(
    "INT", //32
    "PLUS", //+
    "FLOAT", //4.
    "MINUS", //-
    "INT", //2
    "RIGHT_PAREN", //)
    "PLUS", //+
    "INT", //1
    "GREATER_EQUAL", //>=
    "IDENTIFIER", //var
    "EOS", //;
    "EOF"
  };
  vector<Token> token_list=lexer.analize(test);
  EXPECT_EQ(token_list.size(), expect_tokens.size());
  for(int i=0;i<token_list.size();i++){
    // cout<<static_cast<string>(token_list[i])<<endl;
    EXPECT_EQ(token_list[i].token,expect_tokens[i])
      <<"(index: "+std::to_string(i)+")Unexpected token '" <<token_list[i].token<<"' expected '"<<expect_tokens[i]<<"'"<<endl;
  }
}

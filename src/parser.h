#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <utility>
#include <sstream>

#include "lexer.h"
#include "dfa.h"
#include "parser_gen.h"

using std::string;
using std::cout;
using std::endl;

class AST_Node{
public:
  string type;
  string value;
  std::vector<AST_Node> children;
  AST_Node()=default;
  AST_Node(const Token &token):type(token.token),value(token.value){}
  AST_Node(const string &type, const string &value):type(type),value(value){}
  AST_Node(const string &type, const string &value, std::vector<AST_Node> &&children):type(type),value(value),children(children){}
  bool operator==(const AST_Node& node) const noexcept{
    return type==node.type && value==node.value && children==node.children;
  }
  string str() const;
};

class Parser{
private:
  Lexer lex;
  std::shared_ptr<SyntaxDB> db;
  ParsingTable parsing_table;
  AST_Node _parse(const std::vector<Token> &input);
public:
  Parser(Lexer lex, std::shared_ptr<SyntaxDB> db, ParsingTable parsing_table)
    :lex(lex),db(db),parsing_table(parsing_table){}
  AST_Node parse(const string &code){
    return _parse(lex.analize(code));
  }
  const ParsingTable& get_parsing_table() const { return parsing_table; }
  const std::shared_ptr<SyntaxDB> get_db() const { return db; }
};

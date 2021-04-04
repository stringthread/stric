#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <utility>
#include <sstream>

#include "lexer.h"
#include "syntax.h"
#include "parser_gen.h"

using std::string;
using std::cout;
using std::endl;

class AST_Node{
public:
  string type;
  string value;
  int rule_id; // -2 for terminals
  std::vector<AST_Node> children;
  AST_Node()=default;
  AST_Node(const Token &token)
  :type(token.token),value(token.value),rule_id(-2){}
  AST_Node(const string &type, const string &value, int rule_id)
  :type(type),value(value),rule_id(rule_id){}
  AST_Node(const string &type, const string &value, int rule_id, std::vector<AST_Node> &&children)
  :type(type),value(value),rule_id(rule_id),children(children){}
  bool operator==(const AST_Node& node) const noexcept{
    return type==node.type
        && value==node.value
        && rule_id==node.rule_id
        && children==node.children;
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
  static Parser create(Lexer lex, std::shared_ptr<SyntaxDB> db);
  static Parser create();
  static Parser create(Lexer lex);
  static Parser create(std::shared_ptr<SyntaxDB> db);
};

#include <iostream>
#include <string>

#include "parser/parser.h"
#include "parser/syntax.h"
#include "executor.h"

using std::string;

int main(){
  string code;
  std::cin>>code;
  Parser parser=Parser::create();
  AST_Node result=parser.parse(code);
  Executor exec;
  exec.eval(result);
  return 0;
}

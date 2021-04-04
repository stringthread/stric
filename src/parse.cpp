#include <iostream>
#include <string>

#include "parser/parser.h"
#include "parser/syntax.h"

using std::string;

int main(){
  string code;
  std::cin>>code;
  Parser parser=Parser::create();
  AST_Node result=parser.parse(code);
  std::cout << result.str() << '\n';
  return 0;
}
